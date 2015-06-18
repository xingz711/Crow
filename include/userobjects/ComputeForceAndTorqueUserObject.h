/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/


#ifndef COMPUTEFORCEANDTORQUEUSEROBJECT_H
#define COMPUTEFORCEANDTORQUEUSEROBJECT_H

#include "ElementUserObject.h"

//Forward Declarations
class ComputeForceAndTorqueUserObject;

template<>
InputParameters validParams<ComputeForceAndTorqueUserObject>();

/* This class is here to combine the Postprocessor interface and the
 * base class Postprocessor object along with adding MooseObject to the inheritance tree*/
class ComputeForceAndTorqueUserObject :  public ElementUserObject
{
public:
  ComputeForceAndTorqueUserObject(const std::string & name, InputParameters parameters);

  //virtual ~ComputeForceAndTorqueUserObject() {}

  virtual void initialize();
  virtual void execute();
  virtual void finalize();
  virtual void threadJoin(const UserObject & y);

  virtual RealGradient computeQpIntegral(unsigned int i);
  virtual RealGradient computeIntegral(unsigned int i);

protected:

  unsigned int _qp;
  std::string _dF_name;
  const MaterialProperty<std::vector<RealGradient> > & _dF;
  std::vector<RealGradient> _force_value;
  std::vector<RealGradient> _torque_value;

  unsigned int _ncrys;

  const ComputeGrainCenterUserObject & _grain_center;
};

#endif //COMPUTEFORCEANDTORQUEUSEROBJECT_H
