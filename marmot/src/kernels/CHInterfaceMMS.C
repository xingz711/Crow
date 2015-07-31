#include "CHInterfaceMMS.h"

template<>
InputParameters validParams<CHInterfaceMMS>()
{
  InputParameters params = validParams<Kernel>();
  return params;
}

CHInterfaceMMS::CHInterfaceMMS(const InputParameters & parameters) :
    Kernel(parameters),
    _second_u(second()),
    _second_test(secondTest()),
    _second_phi(secondPhi())
{
}

Real
CHInterfaceMMS::computeQpResidual()
{
  return   _second_u[_qp].tr() * (_second_test[_i][_qp].tr())
         + (3.0 * _u[_qp]*_u[_qp] * _grad_u[_qp] - _grad_u[_qp])
         * _grad_test[_i][_qp];
}

Real
CHInterfaceMMS::computeQpJacobian()
{
  return   _second_phi[_j][_qp].tr() * _second_test[_i][_qp].tr()
         + (   6.0 * _u[_qp] * _phi[_j][_qp] * _grad_u[_qp]
             + 3.0 * _u[_qp] * _u[_qp] * _grad_phi[_j][_qp]
             - _grad_phi[_j][_qp]
           )
         * _grad_test[_i][_qp];
}
