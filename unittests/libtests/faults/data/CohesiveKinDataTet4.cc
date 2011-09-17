// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2011 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

/* Original mesh
 *
 * Cells are 0-1, vertices are 2-6.
 *
 * 2   3,4,5  6
 *
 *     ^^^^^ Face in x-y plane
 *
 * After adding cohesive elements
 *
 * Cells are 0-1,13, vertices are 2-9.
 *
 * 2   3,4,5  7,8,9   6
 *             10,11,12
 *     ^^^^^^^^^^^^ Cohesive element in x-y plane.
 */

#include "CohesiveKinDataTet4.hh"

const char* pylith::faults::CohesiveKinDataTet4::_meshFilename =
  "data/tet4.mesh";

const int pylith::faults::CohesiveKinDataTet4::_spaceDim = 3;

const int pylith::faults::CohesiveKinDataTet4::_cellDim = 2;

const int pylith::faults::CohesiveKinDataTet4::_numBasis = 3;

const int pylith::faults::CohesiveKinDataTet4::_numQuadPts = 1;

const PylithScalar pylith::faults::CohesiveKinDataTet4::_quadPts[] = {
  -3.33333333e-01,  -3.33333333e-01,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_quadWts[] = {
  2.0,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_basis[] = {
  3.33333333e-01,  3.33333333e-01,
  3.33333333e-01,};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_basisDeriv[] = {
 -0.50000000e+00, -0.50000000e+00,
  0.50000000e+00,  0.00000000e+00,
  0.00000000e+00,  0.50000000e+00,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_verticesRef[] = {
 -1.00000000e+00, -1.00000000e+00,
  1.00000000e+00, -1.00000000e+00,
 -1.00000000e+00,  1.00000000e+00,
};

const int pylith::faults::CohesiveKinDataTet4::_id = 10;

const char* pylith::faults::CohesiveKinDataTet4::_label = "fault";

const char* pylith::faults::CohesiveKinDataTet4::_finalSlipFilename = 
  "data/tet4_finalslip.spatialdb";

const char* pylith::faults::CohesiveKinDataTet4::_slipTimeFilename = 
  "data/tet4_sliptime.spatialdb";

const char* pylith::faults::CohesiveKinDataTet4::_riseTimeFilename = 
  "data/tet4_risetime.spatialdb";

const PylithScalar pylith::faults::CohesiveKinDataTet4::_fieldT[] = {
  7.1, 8.1, 9.1,
  7.2, 8.2, 9.2, // 3
  7.3, 8.3, 9.3, // 4
  7.4, 8.4, 9.4, // 5
  7.5, 8.5, 9.5,
  7.6, 8.6, 9.6, // 7
  7.8, 8.8, 9.8, // 8
  7.0, 8.0, 9.0, // 9
  7.7, 8.7, 9.7, // 10
  7.9, 8.9, 9.9, // 11
  7.1, 8.1, 9.1, // 12
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_fieldIncr[] = {
  3.1, 4.1, 5.1,
  3.2, 4.2, 5.2, // 3
  3.3, 4.3, 5.3, // 4
  3.4, 4.4, 5.4, // 5
  3.5, 4.5, 5.5,
  3.6, 4.6, 5.6, // 7
  3.8, 4.8, 5.8, // 8
  3.0, 4.0, 5.0, // 9
  3.7, 4.7, 5.7, // 10
  3.9, 4.9, 5.9, // 11
  3.1, 4.1, 5.1, // 12
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_jacobianLumped[] = {
  1.1, 1.1, 1.1,
  1.2, 1.2, 1.2, // 3
  1.3, 1.3, 1.3, // 4
  1.4, 1.4, 1.4, // 5
  1.5, 1.5, 1.5,
  1.6, 1.6, 1.6, // 7
  1.8, 1.8, 1.8, // 8
  1.0, 1.0, 1.0, // 9
  1.7, 1.7, 1.7, // 10
  1.9, 1.9, 1.9, // 11
  1.1, 1.1, 1.1, // 12
};

const int pylith::faults::CohesiveKinDataTet4::_numFaultVertices = 3;
const int pylith::faults::CohesiveKinDataTet4::_verticesFault[] = {
  2, 1, 3
};
const int pylith::faults::CohesiveKinDataTet4::_verticesLagrange[] = {
  11, 10, 12
};
const int pylith::faults::CohesiveKinDataTet4::_verticesNegative[] = {
  4, 3, 5
};
const int pylith::faults::CohesiveKinDataTet4::_verticesPositive[] = {
  8, 7, 9
};

const int pylith::faults::CohesiveKinDataTet4::_numCohesiveCells = 1;
const int pylith::faults::CohesiveKinDataTet4::_cellMappingFault[] = {
  0
};
const int pylith::faults::CohesiveKinDataTet4::_cellMappingCohesive[] = {
  13
};



const PylithScalar pylith::faults::CohesiveKinDataTet4::_orientation[] = {
  0.0, +1.0, 0.0,    0.0, 0.0, +1.0,    +1.0, 0.0, 0.0,
  0.0, +1.0, 0.0,    0.0, 0.0, +1.0,    +1.0, 0.0, 0.0,
  0.0, +1.0, 0.0,    0.0, 0.0, +1.0,    +1.0, 0.0, 0.0,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_area[] = {
  1.0/3.0, 
  1.0/3.0, 
  1.0/3.0,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_residual[] = {
  0.0,  0.0,  0.0,
  9.7,  7.7,  8.7, // 3
  9.9,  7.9,  8.9, // 4
  9.1,  7.1,  8.1, // 5
  0.0,  0.0,  0.0,
 -9.7, -7.7, -8.7, // 7
 -9.9, -7.9, -8.9, // 4
 -9.1, -7.1, -8.1, // 5
 -0.4+1.82575588523, -0.4+-0.55566483464, -0.4+0.07938069066, // 10
 -0.5+1.69682900001, -0.5+-0.56560966667, -0.5+0.14140241667, // 11
  0.4+1.51709826228,  0.4+-0.54615537442,  0.4+0.18205179147, // 12
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_residualIncr[] = {
  0.0,  0.0,  0.0,
  9.7,  7.7,  8.7, // 3
  9.9,  7.9,  8.9, // 4
  9.1,  7.1,  8.1, // 5
  0.0,  0.0,  0.0,
 -9.7, -7.7, -8.7, // 7
 -9.9, -7.9, -8.9, // 4
 -9.1, -7.1, -8.1, // 5
 -0.4+1.82575588523, -0.4+-0.55566483464, -0.4+0.07938069066, // 10
 -0.5+1.69682900001, -0.5+-0.56560966667, -0.5+0.14140241667, // 11
  0.4+1.51709826228,  0.4+-0.54615537442,  0.4+0.18205179147, // 12
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_jacobian[] = {
  0.0, 0.0, 0.0, // 2x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 2y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 2z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 3x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,-1.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 3y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 -1.0, 0.0, 0.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 3z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,-1.0, 0.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 4x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,-1.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 4y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 -1.0, 0.0, 0.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 4z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,-1.0, 0.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 5x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,-1.0, // 12
  0.0, 0.0, 0.0, // 5y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 -1.0, 0.0, 0.0, // 12
  0.0, 0.0, 0.0, // 5z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,-1.0, 0.0, // 12
  0.0, 0.0, 0.0, // 6x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 6y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 6z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 7x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 7y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 7z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 10
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 8x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 8y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 8z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 11
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 9x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 12
  0.0, 0.0, 0.0, // 9y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 12
  0.0, 0.0, 0.0, // 9z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 12
  0.0, 0.0, 0.0, // 10x
  0.0,-1.0, 0.0, // 3
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 7
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 10y
  0.0, 0.0,-1.0, // 3
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 7
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 10z
 -1.0, 0.0, 0.0, // 3
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 7
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 11x
  0.0, 0.0, 0.0,
  0.0,-1.0, 0.0, // 4
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 8
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 11y
  0.0, 0.0, 0.0,
  0.0, 0.0,-1.0, // 4
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 8
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 11z
  0.0, 0.0, 0.0,
 -1.0, 0.0, 0.0, // 4
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 8
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 12x
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,-1.0, 0.0, // 5
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0,+1.0, 0.0, // 9
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 12y
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,-1.0, // 5
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0,+1.0, // 9
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, // 12z
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 -1.0, 0.0, 0.0, // 5
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
 +1.0, 0.0, 0.0, // 9
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
};

const PylithScalar pylith::faults::CohesiveKinDataTet4::_fieldIncrAdjusted[] = {
  3.1, 4.1, 5.1,
  -4.70012229942, -3.03138431537, -1.50390580878, // 3
  -4.10716666377, -2.47185604467, -0.927412749378, // 4
  -3.34252157978, -1.47021951405, -0.324816213039, // 5
  3.5, 4.5, 5.5,
  9.52509172457, 10.0235382365, 10.6279293566, // 7
  9.14962036828, 9.69078492115, 10.2975758746, // 8
  12.4395302117, 12.2183073197, 13.0147426983, // 9
  -8.67766117844, -8.04468697053, -9.48014675931, // 10
  -8.80341285807, -8.09563657419, -9.62931666291, // 11
  -8.21830731966, -8.01474269826, -9.43953021169, // 12
};

pylith::faults::CohesiveKinDataTet4::CohesiveKinDataTet4(void)
{ // constructor
  meshFilename = const_cast<char*>(_meshFilename);
  spaceDim = _spaceDim;
  cellDim = _cellDim;
  numBasis = _numBasis;
  numQuadPts = _numQuadPts;
  quadPts = const_cast<PylithScalar*>(_quadPts);
  quadWts = const_cast<PylithScalar*>(_quadWts);
  basis = const_cast<PylithScalar*>(_basis);
  basisDeriv = const_cast<PylithScalar*>(_basisDeriv);
  verticesRef = const_cast<PylithScalar*>(_verticesRef);
  id = _id;
  label = const_cast<char*>(_label);
  finalSlipFilename = const_cast<char*>(_finalSlipFilename);
  slipTimeFilename = const_cast<char*>(_slipTimeFilename);
  riseTimeFilename = const_cast<char*>(_riseTimeFilename);
  fieldT = const_cast<PylithScalar*>(_fieldT);
  fieldIncr = const_cast<PylithScalar*>(_fieldIncr);
  jacobianLumped = const_cast<PylithScalar*>(_jacobianLumped);
  orientation = const_cast<PylithScalar*>(_orientation);
  area = const_cast<PylithScalar*>(_area);
  residualIncr = const_cast<PylithScalar*>(_residualIncr);
  residual = const_cast<PylithScalar*>(_residual);
  jacobian = const_cast<PylithScalar*>(_jacobian);
  fieldIncrAdjusted = const_cast<PylithScalar*>(_fieldIncrAdjusted);
  verticesFault = const_cast<int*>(_verticesFault);
  verticesLagrange = const_cast<int*>(_verticesLagrange);
  verticesNegative = const_cast<int*>(_verticesNegative);
  verticesPositive = const_cast<int*>(_verticesPositive);
  numFaultVertices = _numFaultVertices;  
  cellMappingFault = const_cast<int*>(_cellMappingFault);
  cellMappingCohesive = const_cast<int*>(_cellMappingCohesive);
  numCohesiveCells = _numCohesiveCells;  
} // constructor

pylith::faults::CohesiveKinDataTet4::~CohesiveKinDataTet4(void)
{}


// End of file
