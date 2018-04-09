#!/usr/bin/env python
# ----------------------------------------------------------------------
#
# Brad T. Aagaard, U.S. Geological Survey
# Charles A. Williams, GNS Science
# Matthew G. Knepley, University of Chicago
#
# This code was developed as part of the Computational Infrastructure
# for Geodynamics (http://geodynamics.org).
#
# Copyright (c) 2010-2015 University of California, Davis
#
# See COPYING for license information.
#
# ----------------------------------------------------------------------
#
# Initial attempt to compute governing equations for generalized Maxwell
# plane strain material and then run assumed solution through these equations.
# PREREQUISITES:  sympy
# ----------------------------------------------------------------------
#
# import pdb
# pdb.set_trace()
import sympy
import sympy.tensor
import sympy.tensor.array
# ----------------------------------------------------------------------
ndim = 2
numComps = 2
ndimRange = range(ndim)
numCompsRange = range(numComps)
outFile = 'mms_genmaxplanestrain.txt'

zero = sympy.sympify(0)
one = sympy.sympify(1)
two = sympy.sympify(2)
three = sympy.sympify(3)

f = open(outFile, 'w')

# ----------------------------------------------------------------------
def printTensor(tensor, tensorName):
  """
  Function to print components of a tensor.
  For now, assume a rank 1 or 2 tensor.
  """
  
  rank = tensor.rank()
  ndim = tensor.shape[0]
  simpTensor = sympy.simplify(tensor)
  
  for i in range(ndim):
    if (rank == 2):
      for j in range(ndim):
        line = tensorName + '_%d%d = %s\n' % (i+1, j+1, simpTensor[i,j])
        f.write(line)
    else:
      line = tensorName + '_%d = %s\n' % (i+1, simpTensor[i])
      f.write(line)

  f.write('\n')

  return
# ----------------------------------------------------------------------
  
# Define basis and displacement vector.
from sympy.abc import x, y, t
u1, u2 = sympy.symbols('u1 u2', type="Function")
X = sympy.tensor.array.Array([x, y])

# Material constants.
(bulkModulus, shearModulus) = sympy.symbols('bulkModulus shearModulus')
(maxwellTime_1, maxwellTime_2, maxwellTime_3) = sympy.symbols(
  'maxwellTime_1 maxwellTime_2 maxwellTime_3')
(shearModulusRatio_1, shearModulusRatio_2, shearModulusRatio_3) = sympy.symbols(
  'shearModulusRatio_1 shearModulusRatio_2 shearModulusRatio_3')

# Assumed displacements:  second-order spatial variation for now.
a, b, c, d, e = sympy.symbols('a b c d e')
u1 = (x * x * a + two * x * y * b + y * y * c) * \
     (shearModulusRatio_1 * sympy.exp(-t/maxwellTime_1) + \
      shearModulusRatio_2 * sympy.exp(-t/maxwellTime_2) + \
      shearModulusRatio_3 * sympy.exp(-t/maxwellTime_3))
u2 = (x * x * c + two * y * x * b + y * y * a) * \
     (shearModulusRatio_1 * sympy.exp(-t/maxwellTime_1) + \
      shearModulusRatio_2 * sympy.exp(-t/maxwellTime_2) + \
      shearModulusRatio_3 * sympy.exp(-t/maxwellTime_3))
U = sympy.tensor.array.Array([u1, u2])
Udot = U.diff(t)

# Deformation gradient, transpose, and strain tensor.
defGrad = sympy.tensor.array.derive_by_array(U, X)
defGradTranspose = sympy.tensor.array.Array(defGrad.tomatrix().transpose())
strain = (defGrad + defGradTranspose)/two
strainRate = strain.diff(t)
strain3d = sympy.tensor.array.Array([[strain[0,0],strain[0,1],zero],
                                     [strain[1,0],strain[1,1],zero],
                                     [zero,zero,zero]])
strainRate3d = strain3d.diff(t)

# Define volumetric strain and deviatoric strain.
volStrain = sympy.tensor.array.tensorcontraction(strain, (0, 1))
volStrainArr = sympy.tensor.array.tensorproduct(volStrain, sympy.eye(ndim))
devStrain = strain - volStrainArr/two
devStrainRate = devStrain.diff(t)
volStrain3d = sympy.tensor.array.tensorcontraction(strain3d, (0, 1))
volStrainArr3d = sympy.tensor.array.tensorproduct(volStrain3d, sympy.eye(3))
devStrain3d = strain3d - volStrainArr3d/three
devStrainRate3d = devStrain3d.diff(t)

# Assumed viscous strains.
visStrain_1 = devStrain * (one - sympy.exp(-t/maxwellTime_1))
visStrain3d_1 = devStrain3d * (one - sympy.exp(-t/maxwellTime_1))
visStrain_2 = devStrain * (one - sympy.exp(-t/maxwellTime_2))
visStrain3d_2 = devStrain3d * (one - sympy.exp(-t/maxwellTime_2))
visStrain_3 = devStrain * (one - sympy.exp(-t/maxwellTime_3))
visStrain3d_3 = devStrain3d * (one - sympy.exp(-t/maxwellTime_3))

# Define viscous strain rate and stress function.
visStrainRate_1 = visStrain_1.diff(t)
visStrainFunc_1 = maxwellTime_1 * (devStrainRate - visStrainRate_1)
visStrainRate_2 = visStrain_2.diff(t)
visStrainFunc_2 = maxwellTime_2 * (devStrainRate - visStrainRate_2)
visStrainRate_3 = visStrain_3.diff(t)
visStrainFunc_3 = maxwellTime_3 * (devStrainRate - visStrainRate_3)
visStrainRate3d_1 = visStrain3d_1.diff(t)
visStrainFunc3d_1 = maxwellTime_1 * (devStrainRate3d - visStrainRate3d_1)
visStrainRate3d_2 = visStrain3d_2.diff(t)
visStrainFunc3d_2 = maxwellTime_2 * (devStrainRate3d - visStrainRate3d_2)
visStrainRate3d_3 = visStrain3d_3.diff(t)
visStrainFunc3d_3 = maxwellTime_3 * (devStrainRate3d - visStrainRate3d_3)

# Define deviatoric stress and mean stress.
shearModulusRatio_0 = one - shearModulusRatio_1 - shearModulusRatio_2 - \
                      shearModulusRatio_3
devStress = two * shearModulus * (shearModulusRatio_0 * devStrain + \
                                  shearModulusRatio_1 * visStrainFunc_1 + \
                                  shearModulusRatio_2 * visStrainFunc_2 + \
                                  shearModulusRatio_3 * visStrainFunc_3)
meanStressArr = bulkModulus * volStrainArr
stress = meanStressArr + devStress
devStress3d = two * shearModulus * (shearModulusRatio_0 * devStrain3d + \
                                    shearModulusRatio_1 * visStrainFunc3d_1 + \
                                    shearModulusRatio_2 * visStrainFunc3d_2 + \
                                    shearModulusRatio_3 * visStrainFunc3d_3)
meanStressArr3d = bulkModulus * volStrainArr3d
stress3d = meanStressArr3d + devStress3d

# Equilibrium equation.
equilDeriv = sympy.tensor.array.derive_by_array(stress, X)
equil = sympy.tensor.array.tensorcontraction(equilDeriv, (1,2))

# Write results to file.
f.write('Solution variables:\n')
printTensor(U, 's')
printTensor(Udot, 's_t')
printTensor(defGrad, 's_x')

f.write('\nAuxiliary variables:\n')
printTensor(strain, 'totalStrain')
printTensor(strainRate, 'totalStrain_t')
printTensor(visStrain_1, 'visStrain_1')
printTensor(visStrainRate_1, 'visStrain_1_t')
printTensor(visStrain_2, 'visStrain_2')
printTensor(visStrainRate_2, 'visStrain_2_t')
printTensor(visStrain_3, 'visStrain_3')
printTensor(visStrainRate_3, 'visStrain_3_t')

f.write('\nAuxiliary variables (3d):\n')
printTensor(strain3d, 'totalStrain3d')
printTensor(strainRate3d, 'totalStrain3d_t')
printTensor(visStrain3d_1, 'visStrain3d_1')
printTensor(visStrainRate3d_1, 'visStrain3d_1_t')
printTensor(visStrain3d_2, 'visStrain3d_2')
printTensor(visStrainRate3d_2, 'visStrain3d_2_t')
printTensor(visStrain3d_3, 'visStrain3d_3')
printTensor(visStrainRate3d_3, 'visStrain3d_3_t')

f.write('\nEquilibrium:\n')
printTensor(equil, 'equil')

f.write('\nAdditional:\n')
printTensor(devStrain, 'devStrain')
printTensor(devStrainRate, 'devStrain_t')
printTensor(stress, 'stress')
printTensor(devStress, 'devStress')
printTensor(visStrainFunc_1, 'visStrainFunc_1')
printTensor(visStrainFunc_2, 'visStrainFunc_2')
printTensor(visStrainFunc_3, 'visStrainFunc_3')

f.write('\nAdditional (3d):\n')
printTensor(devStrain3d, 'devStrain3d')
printTensor(devStrainRate3d, 'devStrain3d_t')
printTensor(stress3d, 'stress3d')
printTensor(devStress3d, 'devStress3d')
printTensor(visStrainFunc3d_1, 'visStrainFunc3d_1')
printTensor(visStrainFunc3d_2, 'visStrainFunc3d_2')
printTensor(visStrainFunc3d_3, 'visStrainFunc3d_3')

f.close()