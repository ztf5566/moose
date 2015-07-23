/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef COMPUTEAXISYMMETRICRZSMALLSTRAIN_H
#define COMPUTEAXISYMMETRICRZSMALLSTRAIN_H

#include "Compute2DSmallStrain.h"

/**
 * ComputeAxisymmetricRZSmallStrain defines small strains in an Axisymmetric system.
 * The COORD_TYPE in the Problem block must be set to RZ.
 */
class ComputeAxisymmetricRZSmallStrain : public Compute2DSmallStrain
{
public:
  ComputeAxisymmetricRZSmallStrain(const InputParameters & parameters);
  ComputeAxisymmetricRZSmallStrain(const std::string & deprecated_name, InputParameters parameters); // DEPRECATED CONSTRUCTOR

protected:
  virtual Real computeStrainZZ();
};

#endif //COMPUTEAXISYMMETRICRZSMALLSTRAIN_H
