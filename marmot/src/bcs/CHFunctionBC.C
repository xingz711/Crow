#include "CHFunctionBC.h"
#include "Function.h"

template<>
InputParameters validParams<CHFunctionBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<FunctionName>("function", "The function.");
  return params;
}

CHFunctionBC::CHFunctionBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _func(getFunction("function"))
{
}

Real
CHFunctionBC::computeQpResidual()
{
  return   -_func.value(_t, _q_point[_qp]) * (_grad_test[_i][_qp] * _normals[_qp]);
}
