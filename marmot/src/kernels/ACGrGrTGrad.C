#include "ACGrGrTGrad.h"
#include "Material.h"

template<>
InputParameters validParams<ACGrGrTGrad>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("T", "intermediate parameter--temperature");
  params.addRequiredParam<Real>("wGB", "Diffuse GB width in the units of the simulation");
  params.addCoupledVar("c", 0.0, "concentration representing a void, bubble or precipitate");
  return params;
}

ACGrGrTGrad::ACGrGrTGrad(const InputParameters & parameters):
    ACBulk(parameters),
    _entropy_diff(getMaterialPropertyByName<Real>("entropy_diff")),
    _molar_volume(getMaterialPropertyByName<Real>("molar_volume")),
    _act_wGB(getMaterialPropertyByName<Real>("act_wGB")),
    _T_var(coupled("T")),
    _T(coupledValue("T")),
    _grad_T(coupledGradient("T")),
    _wGB(getParam<Real>("wGB")),
    _c(coupledValue("c"))
{
}

Real
ACGrGrTGrad::computeDFDOP(PFFunctionType type)
{
  Real mult = 3.0/4.0 * _entropy_diff[_qp] * _wGB * _act_wGB[_qp] / _molar_volume[_qp];

  // Zero out the driving force on interfaces with bubbles, void, precipitates, etc.
  mult *= (1.0 - _c[_qp]);

  switch (type)
  {
    case Residual:
      return mult * _grad_u[_qp] * _grad_T[_qp];

    case Jacobian:
      return mult * _grad_phi[_j][_qp] * _grad_T[_qp];
  }

  mooseError("Invalid type passed in");
}
