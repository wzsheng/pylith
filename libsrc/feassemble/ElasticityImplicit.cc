// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

#include <portinfo>

#include "ElasticityImplicit.hh" // implementation of class methods

#include "Quadrature.hh" // USES Quadrature
#include "CellGeometry.hh" // USES CellGeometry

#include "pylith/materials/ElasticMaterial.hh" // USES ElasticMaterial
#include "pylith/topology/FieldsManager.hh" // USES FieldsManager
#include "pylith/utils/array.hh" // USES double_array
#include "pylith/utils/macrodefs.h" // USES CALL_MEMBER_FN

#include "petscmat.h" // USES PetscMat
#include "spatialdata/spatialdb/SpatialDB.hh"

#include <assert.h> // USES assert()
#include <stdexcept> // USES std::runtime_error

#define FASTER

// ----------------------------------------------------------------------
// Constructor
pylith::feassemble::ElasticityImplicit::ElasticityImplicit(void) :
  _dtm1(-1.0)
{ // constructor
} // constructor

// ----------------------------------------------------------------------
// Destructor
pylith::feassemble::ElasticityImplicit::~ElasticityImplicit(void)
{ // destructor
} // destructor
  
// ----------------------------------------------------------------------
// Set time step for advancing from time t to time t+dt.
void
pylith::feassemble::ElasticityImplicit::timeStep(const double dt)
{ // timeStep
  if (_dt != -1.0)
    _dtm1 = _dt;
  else
    _dtm1 = dt;
  _dt = dt;
  assert(_dt == _dtm1); // For now, don't allow variable time step
  if (0 != _material)
    _material->timeStep(_dt);
} // timeStep

// ----------------------------------------------------------------------
// Set flag for setting constraints for total field solution or
// incremental field solution.
void
pylith::feassemble::ElasticityImplicit::useSolnIncr(const bool flag)
{ // useSolnIncr
  assert(0 != _material);
  _useSolnIncr = flag;
  _material->useElasticBehavior(!_useSolnIncr);
} // useSolnIncr

// ----------------------------------------------------------------------
// Integrate constributions to residual term (r) for operator.
void
pylith::feassemble::ElasticityImplicit::integrateResidual(
			      const ALE::Obj<real_section_type>& residual,
			      const double t,
			      topology::FieldsManager* const fields,
			      const ALE::Obj<Mesh>& mesh)
{ // integrateResidual
  /// Member prototype for _elasticityResidualXD()
  typedef void (pylith::feassemble::ElasticityImplicit::*elasticityResidual_fn_type)
    (const double_array&);
  
  assert(0 != _quadrature);
  assert(0 != _material);
  assert(!residual.isNull());
  assert(0 != fields);
  assert(!mesh.isNull());

  static PetscEvent setupEvent = 0, cellGeomEvent = 0, stateVarsEvent = 0, restrictEvent = 0, computeEvent = 0, updateEvent = 0, stressEvent;

  if (!setupEvent)
    PetscLogEventRegister(&setupEvent, "IRSetup", 0);
  if (!cellGeomEvent)
    PetscLogEventRegister(&cellGeomEvent, "IRCellGeom", 0);
  if (!stateVarsEvent)
    PetscLogEventRegister(&stateVarsEvent, "IRProperties", 0);
  if (!restrictEvent)
    PetscLogEventRegister(&restrictEvent, "IRRestrict", 0);
  if (!computeEvent)
    PetscLogEventRegister(&computeEvent, "IRCompute", 0);
  if (!updateEvent)
    PetscLogEventRegister(&updateEvent, "IRUpdate", 0);
  if (!stressEvent)
    PetscLogEventRegister(&stressEvent, "IRMaterialStress", 0);

  const Obj<sieve_type>& sieve = mesh->getSieve();

  PetscLogEventBegin(setupEvent,0,0,0,0);
  // Set variables dependent on dimension of cell
  const int cellDim = _quadrature->cellDim();
  int tensorSize = 0;
  totalStrain_fn_type calcTotalStrainFn;
  elasticityResidual_fn_type elasticityResidualFn;
  if (1 == cellDim) {
    tensorSize = 1;
    elasticityResidualFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityResidual1D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain1D;
  } else if (2 == cellDim) {
    tensorSize = 3;
    elasticityResidualFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityResidual2D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain2D;
  } else if (3 == cellDim) {
    tensorSize = 6;
    elasticityResidualFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityResidual3D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain3D;
  } else
    assert(0);

  // Get cell information
  const int materialId = _material->id();
  const ALE::Obj<ALE::Mesh::label_sequence>& cells = 
    mesh->getLabelStratum("material-id", materialId);
  assert(!cells.isNull());
  const Mesh::label_sequence::iterator  cellsEnd = cells->end();

  // Get sections
  const ALE::Obj<real_section_type>& coordinates = 
    mesh->getRealSection("coordinates");
  assert(!coordinates.isNull());
  const ALE::Obj<real_section_type>& dispTBctpdt = 
    fields->getReal("dispTBctpdt");
  assert(!dispTBctpdt.isNull());

  // Get cell geometry information that doesn't depend on cell
  const int numQuadPts = _quadrature->numQuadPts();
  const double_array& quadWts = _quadrature->quadWts();
  assert(quadWts.size() == numQuadPts);
  const int numBasis = _quadrature->numBasis();
  const int spaceDim = _quadrature->spaceDim();

#ifdef FASTER
  fields->createCustomAtlas("material-id", materialId);
  const int dispAtlasTag = 
    fields->getFieldAtlasTag("dispTBctpdt", materialId);
  const int residualAtlasTag = 
    fields->getFieldAtlasTag("residual", materialId);
#endif

  // Precompute the geometric and function space information
  _quadrature->precomputeGeometry(mesh, coordinates, cells);

  // Allocate vector for cell values.
  _initCellVector();
  const int cellVecSize = numBasis*spaceDim;
  double_array dispTBctpdtCell(cellVecSize);
  //double_array gravCell(cellVecSize);

  // Allocate vector for total strain
  double_array totalStrain(numQuadPts*tensorSize);
  totalStrain = 0.0;
  PetscLogEventEnd(setupEvent,0,0,0,0);

  // Loop over cells
  int c_index = 0;
  for (Mesh::label_sequence::iterator c_iter=cells->begin();
       c_iter != cellsEnd;
       ++c_iter, ++c_index) {
    // Compute geometry information for current cell
    PetscLogEventBegin(cellGeomEvent,0,0,0,0);
    _quadrature->retrieveGeometry(mesh, coordinates, *c_iter, c_index);
    PetscLogEventEnd(cellGeomEvent,0,0,0,0);

    // Get state variables for cell.
    PetscLogEventBegin(stateVarsEvent,0,0,0,0);
    _material->getPropertiesCell(*c_iter, numQuadPts);
    PetscLogEventEnd(stateVarsEvent,0,0,0,0);

    // Reset element vector to zero
    _resetCellVector();

    // Restrict input fields to cell
    PetscLogEventBegin(restrictEvent,0,0,0,0);
#ifdef FASTER
    mesh->restrict(dispTBctpdt, dispAtlasTag, c_index, &dispTBctpdtCell[0], 
		   cellVecSize);
#else
    mesh->restrict(dispTBctpdt, *c_iter, &dispTBctpdtCell[0], cellVecSize);
#endif
    PetscLogEventEnd(restrictEvent,0,0,0,0);

    // Get cell geometry information that depends on cell
    const double_array& basis = _quadrature->basis();
    const double_array& basisDeriv = _quadrature->basisDeriv();
    const double_array& jacobianDet = _quadrature->jacobianDet();

    if (cellDim != spaceDim)
      throw std::logic_error("Not implemented yet.");

#if 0
    // Comment out gravity section for now, until we figure out how to deal
    // with gravity vector.

    // Get density at quadrature points for this cell
    const double_array& density = _material->calcDensity();

    // Compute action for element body forces
    if (!grav.isNull()) {
      mesh->restrict(grav, *c_iter, &gravCell[0], cellVecSize);
      for (int iQuad=0; iQuad < numQuadPts; ++iQuad) {
	const double wt = quadWts[iQuad] * jacobianDet[iQuad] * density[iQuad];
	for (int iBasis=0, iQ=iQuad*numBasis*cellDim;
	     iBasis < numBasis; ++iBasis) {
	  const double valI = wt*basis[iQ+iBasis];
	  for (int iDim=0; iDim < spaceDim; ++iDim) {
	    _cellVector[iBasis*spaceDim+iDim] += valI*gravCell[iDim];
	  } // for
	} // for
      } // for
      PetscLogFlopsNoCheck(numQuadPts*(2+numBasis*(2+2*spaceDim)));
    } // if
#endif

    // Compute B(transpose) * sigma, first computing strains
    PetscLogEventBegin(stressEvent,0,0,0,0);
    calcTotalStrainFn(&totalStrain, basisDeriv, dispTBctpdtCell, 
		      numBasis, numQuadPts);
    const double_array& stress = _material->calcStress(totalStrain);
    PetscLogEventEnd(stressEvent,0,0,0,0);

    PetscLogEventBegin(computeEvent,0,0,0,0);
    CALL_MEMBER_FN(*this, elasticityResidualFn)(stress);
    PetscLogEventEnd(computeEvent,0,0,0,0);

#if 0
    std::cout << "Updating residual for cell " << *c_iter << std::endl;
    for(int i = 0; i < _quadrature->spaceDim() * _quadrature->numBasis(); ++i) {
      std::cout << "  v["<<i<<"]: " << _cellVector[i] << std::endl;
    }
#endif
    // Assemble cell contribution into field
    PetscLogEventBegin(updateEvent,0,0,0,0);
#ifdef FASTER
    mesh->updateAdd(residual, residualAtlasTag, c_index, _cellVector);
#else
    mesh->updateAdd(residual, *c_iter, _cellVector);
#endif
    PetscLogEventEnd(updateEvent,0,0,0,0);
  } // for
} // integrateResidual


// ----------------------------------------------------------------------
// Compute stiffness matrix.
void
pylith::feassemble::ElasticityImplicit::integrateJacobian(
					PetscMat* mat,
					const double t,
					topology::FieldsManager* fields,
					const ALE::Obj<Mesh>& mesh)
{ // integrateJacobian
  /// Member prototype for _elasticityJacobianXD()
  typedef void (pylith::feassemble::ElasticityImplicit::*elasticityJacobian_fn_type)
    (const double_array&);

  assert(0 != _quadrature);
  assert(0 != _material);
  assert(0 != mat);
  assert(0 != fields);
  assert(!mesh.isNull());

  // Set variables dependent on dimension of cell
  const int cellDim = _quadrature->cellDim();
  int tensorSize = 0;
  totalStrain_fn_type calcTotalStrainFn;
  elasticityJacobian_fn_type elasticityJacobianFn;
  if (1 == cellDim) {
    tensorSize = 1;
    elasticityJacobianFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityJacobian1D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain1D;
  } else if (2 == cellDim) {
    tensorSize = 3;
    elasticityJacobianFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityJacobian2D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain2D;
  } else if (3 == cellDim) {
    tensorSize = 6;
    elasticityJacobianFn = 
      &pylith::feassemble::ElasticityImplicit::_elasticityJacobian3D;
    calcTotalStrainFn = 
      &pylith::feassemble::IntegratorElasticity::_calcTotalStrain3D;
  } else
    assert(0);

  // Get cell information
  const int materialId = _material->id();
  const ALE::Obj<ALE::Mesh::label_sequence>& cells = 
    mesh->getLabelStratum("material-id", materialId);
  assert(!cells.isNull());
  const Mesh::label_sequence::iterator  cellsEnd = cells->end();

  // Get sections
  const ALE::Obj<real_section_type>& coordinates = 
    mesh->getRealSection("coordinates");
  assert(!coordinates.isNull());
  const ALE::Obj<real_section_type>& dispTBctpdt = 
    fields->getReal("dispTBctpdt");
  assert(!dispTBctpdt.isNull());

  // Get parameters used in integration.
  const double dt = _dt;
  assert(dt > 0);

  // Get cell geometry information that doesn't depend on cell
  const int numQuadPts = _quadrature->numQuadPts();
  const double_array& quadWts = _quadrature->quadWts();
  const int numBasis = _quadrature->numBasis();
  const int spaceDim = _quadrature->spaceDim();
  
  if (cellDim != spaceDim)
    throw std::logic_error("Don't know how to integrate elasticity " \
			   "contribution to Jacobian matrix for cells with " \
			   "different dimensions than the spatial dimension.");

  // Precompute the geometric and function space information
  _quadrature->precomputeGeometry(mesh, coordinates, cells);

  // Allocate matrix and vectors for cell values.
  _initCellMatrix();
  const int cellVecSize = numBasis*spaceDim;
  double_array dispTBctpdtCell(cellVecSize);

  // Allocate vector for total strain
  double_array totalStrain(numQuadPts*tensorSize);
  totalStrain = 0.0;

#ifdef FASTER
  fields->createCustomAtlas("material-id", materialId);
  const int dispAtlasTag = fields->getFieldAtlasTag("dispTBctpdt", materialId);
#endif

  // Loop over cells
  int c_index = 0;
  for (Mesh::label_sequence::iterator c_iter=cells->begin();
       c_iter != cellsEnd;
       ++c_iter, ++c_index) {
    // Compute geometry information for current cell
    _quadrature->retrieveGeometry(mesh, coordinates, *c_iter, c_index);

    // Get state variables for cell.
    _material->getPropertiesCell(*c_iter, numQuadPts);

    // Reset element vector to zero
    _resetCellMatrix();

    // Restrict input fields to cell
#ifdef FASTER
    mesh->restrict(dispTBctpdt, dispAtlasTag, c_index, &dispTBctpdtCell[0], 
		   cellVecSize);
#else
    mesh->restrict(dispTBctpdt, *c_iter, &dispTBctpdtCell[0], cellVecSize);
#endif

    // Get cell geometry information that depends on cell
    const double_array& basis = _quadrature->basis();
    const double_array& basisDeriv = _quadrature->basisDeriv();
    const double_array& jacobianDet = _quadrature->jacobianDet();

    // Compute strains
    calcTotalStrainFn(&totalStrain, basisDeriv, dispTBctpdtCell, 
		      numBasis, numQuadPts);
      
    // Get "elasticity" matrix at quadrature points for this cell
    const double_array& elasticConsts = 
      _material->calcDerivElastic(totalStrain);

    CALL_MEMBER_FN(*this, elasticityJacobianFn)(elasticConsts);

    // Assemble cell contribution into field.  Not sure if this is correct for
    // global stiffness matrix.
    const ALE::Obj<Mesh::order_type>& globalOrder = 
      mesh->getFactory()->getGlobalOrder(mesh, "default", dispTBctpdt);
    PetscErrorCode err = updateOperator(*mat, mesh, dispTBctpdt, globalOrder,
					*c_iter, _cellMatrix, ADD_VALUES);
    if (err)
      throw std::runtime_error("Update to PETSc Mat failed.");
  } // for
  _needNewJacobian = false;
  _material->resetNeedNewJacobian();
} // integrateJacobian


// End of file 
