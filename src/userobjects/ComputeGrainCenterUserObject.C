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

#include "ComputeGrainCenterUserObject.h"

template<>
InputParameters validParams<ComputeGrainCenterUserObject>()
{
  InputParameters params = validParams<ElementUserObject>();
  params.addRequiredParam<std::vector<std::string> >("etas", "order parameters");
  return params;
}

ComputeGrainCenterUserObject::ComputeGrainCenterUserObject(const std::string & name, InputParameters parameters) :
    ElementUserObject(name, parameters),
    _qp(0)

{
    _nargs = coupledComponents("etas"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
    _vals.resize(_nargs); //Size variable arrays
    _grad_vals.resize(_nargs);
    _vals_var.resize(_nargs);
    _grain_volume.resize(_nargs);

    for (unsigned int i = 0; i < _nargs; ++i)
    {
      _vals[i] = &coupledValue("etas", i);
      _grad_vals[i] = &coupledGradient("etas", i);
      _vals_var[i] = coupled("etas", i);
    }
}

void
ComputeGrainCenterUserObject::initialize()
{
  for (unsigned int i = 0; i < _nargs; ++i)
  _grain_volume[i] = 0;
}

void
ComputeGrainCenterUserObject::execute()
{
  for (unsigned int i = 0; i < _nargs; ++i)
  _grain_volume[i] += computeIntegral();
}

Real
ComputeGrainCenterUserObject::getValue()
{
  for (unsigned int i = 0; i < _nargs; ++i)
  return _grain_volume[i];
}

void
ComputeGrainCenterUserObject::finalize()
{
  for (unsigned int i = 0; i < _nargs; ++i)
  _grain_volume[i] += 0.0 ;
}

void
ComputeGrainCenterUserObject::threadJoin(const UserObject & y)
{
  const ComputeGrainCenterUserObject & pps = static_cast<const ComputeGrainCenterUserObject &>(y);
  for (unsigned int i = 0; i < _nargs; ++i)
  _grain_volume [i] += pps._grain_volume[i];
}

Real
ComputeGrainCenterUserObject::computeIntegral()
{
  std::vector<Real> integral.resize(_nargs);
  for (unsigned int i = 0; i < _nargs; ++i)
  {
    integral[i] = 0.0;
    for (_qp=0; _qp<_qrule->n_points(); _qp++)
    integral[i] += _JxW[_qp]*_coord[_qp]*computeQpIntegral();
    return integral[i];
  }
}

Real
ComputeGrainCenterUserObject::computeQpIntegral()
{
  for (unsigned int i = 0; i < _nargs; ++i)
   return (*_vals[i])[_qp];
}
