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

#include "ComputeForceAndTorqueUserObject.h"

template<>
InputParameters validParams<ComputeForceAndTorqueUserObject>()
{
  InputParameters params = validParams<UserObject>();
  params.addRequiredParam<std::string>("force_density", "Force density material");
  param.addParam<UserObjectName>("grain_center","center of mass of grains");
  return params;
}

ComputeForceAndTorqueUserObject::ComputeForceAndTorqueUserObject(const std::string & name, InputParameters parameters) :
    ElementUserObject(name, parameters),
    _dF_name(getParam<std::string>("force_density")),
    _dF(getMaterialProperty<std::vector<RealGradient> >(_dF_name)),
    _grain_center(getUserObject<ComputeGrainCenterUserObject>("grain_center"))

{
    _ncrys = _dF.size();
    _force_value.resize(_ncrys);
    _grain_center.resize(_ncrys);
    _torque_value.resize(_ncrys);
}

void
ComputeForceAndTorqueUserObject::initialize()
{
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _force_value[i] = 0;
    _torque_value[i] = 0;
  }
}

void
ComputeForceAndTorqueUserObject::execute()
{
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _force_value[i] = computeIntegral(i);
    _torque_value[i] = computeIntegral(i);
  }
}

void
ComputeForceAndTorqueUserObject::finalize()
{
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    gatherSum(_force_value[i]);
  }
}

void
ComputeForceAndTorqueUserObject::threadJoin(const UserObject & y)
{
  const ComputeForceAndTorqueUserObject & pps = static_cast<const ComputeForceAndTorqueUserObject &>(y);
  for (unsigned int i = 0; i < _ncrys; ++i)
    _force_value[i] += pps._force_value[i];
}

RealGradient
ComputeForceAndTorqueUserObject::computeIntegral(unsigned int i)
{
  for (_qp=0; _qp<_qrule->n_points(); _qp++)
    {
    _force_value[i] += _JxW[_qp]*_coord[_qp]*_dF[i][_qp];
    _torque_value[i] +=_JxW[_qp]*_coord[_qp]*(_q_point[_qp]-_grain_center[i]).cross(_dF[i][_qp]);
    }
  return  _force_value[i];
}
