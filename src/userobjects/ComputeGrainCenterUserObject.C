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
  params.addRequiredParam<std::vector<VariableName> >("etas", "order parameters");
  return params;
}

ComputeGrainCenterUserObject::ComputeGrainCenterUserObject(const std::string & name, InputParameters parameters) :
    ElementUserObject(name, parameters),
    MooseVariableInterface(parameters, false),
    _var(_subproblem.getVariable(_tid, parameters.get<std::vector<VariableName> >("etas")))

{
    _ncrys = coupledComponents("etas"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
    _vals.resize(_ncrys); //Size variable arrays

    _ncomp = 4*_ncrys;
    _grain_center_output.resize(_ncomp);

    for (unsigned int i = 0; i < _ncrys; ++i)
    _var[i](_subproblem.getVariable(_tid, parameters.get<std::vector<VariableName> >("etas")))

      addMooseVariableDependency(mooseVariable());
}

void
ComputeGrainCenterUserObject::initialize()
{
  for (unsigned int i = 0; i < _ncomp; ++i)
  {
    _grain_center_output[i] = 0;
  }
}

void
ComputeGrainCenterUserObject::execute()
{
  _grain_center_output += computeIntegral();
}

void
ComputeGrainCenterUserObject::finalize()
{
   gatherSum(_grain_center_output);

   for (unsigned int i = 0; i < _ncrys; ++i)
    {
      _grain_center[i](0) = _grain_center_output[4*i+1]/_grain_center_output[4*i+0];
      _grain_center[i](1) = _grain_center_output[4*i+2]/_grain_center_output[4*i+0];
      _grain_center[i](2) = _grain_center_output[4*i+3]/_grain_center_output[4*i+0];
    }
}

void
ComputeGrainCenterUserObject::threadJoin(const UserObject & y)
{
  const ComputeGrainCenterUserObject & pps = static_cast<const ComputeGrainCenterUserObject &>(y);
    _grain_center_output += pps._grain_center_output;
}

Real
ComputeGrainCenterUserObject::computeIntegral()
{
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    for (_qp=0; _qp<_qrule->n_points(); _qp++)
    {
      _grain_center_output[4*i+0] += _JxW[_qp]*_coord[_qp]*(*_vals[i])[_qp];
      _grain_center_output[4*i+1] += _JxW[_qp]*_coord[_qp]*_q_point[_qp](0)*(*_vals[i])[_qp];
      _grain_center_output[4*i+2] += _JxW[_qp]*_coord[_qp]*_q_point[_qp](1)*(*_vals[i])[_qp];
      _grain_center_output[4*i+3] += _JxW[_qp]*_coord[_qp]*_q_point[_qp](2)*(*_vals[i])[_qp];
    }
  }

  return _grain_center_output;
}
