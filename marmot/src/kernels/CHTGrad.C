#include "CHTGrad.h"

template<>
InputParameters validParams<CHTGrad>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVar("T", "Temperature");
  return params;
}

CHTGrad::CHTGrad(const InputParameters & parameters) :
    CHBulk(parameters),
    _Qstar(getMaterialPropertyByName<Real>("Qstar")),
    _T(coupledValue("T")),
    _grad_T(coupledGradient("T"))
{
}

RealGradient
CHTGrad::computeGradDFDCons(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return -_Qstar[_qp]*(_u[_qp]/_T[_qp])*_grad_T[_qp];

    case Jacobian:
      return -_Qstar[_qp]*_phi[_j][_qp]/_T[_qp]*_grad_T[_qp]; // return Off-diag Jacobian value
  }

  mooseError("Invalid type passed in");
}
