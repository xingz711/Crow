#include "ACBulkZrH.h"

template<>
InputParameters validParams<ACBulkZrH>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("eta2","Order parameter 2");
  params.addRequiredCoupledVar("eta3","Order parameter 3");
  params.addRequiredCoupledVar("C","Concentration of Hydrogen");
  params.addParam<Real>("C_m",0.05,"Concentration of Hydrogen in Matrix");
  params.addParam<Real>("C_gamma",0.5,"Concentration of Hydrogen in Hydride");
  params.addParam<Real>("b",-1,"double well function coefficient");
  params.addParam<Real>("c",10,"double well function coefficient");
  params.addParam<Real>("d",-1,"double well function coefficient");
  return params;
}

ACBulkZrH::ACBulkZrH(const InputParameters & parameters) :
    ACBulk(parameters),
    _eta2(coupledValue("eta2")),
    _eta2_var(coupled("eta2")),
    _eta3(coupledValue("eta3")),
    _eta3_var(coupled("eta3")),
    _C(coupledValue("C")),
    _C_var(coupled("C")),
    _G(getMaterialPropertyByName<Real>("G")),
    _b(getParam<Real>("b")),
    _c(getParam<Real>("c")),
    _d(getParam<Real>("d")),
    _C_m(getParam<Real>("C_m")),
    _C_gamma(getParam<Real>("C_gamma"))
{
}

Real
ACBulkZrH::computeDFDOP(PFFunctionType type)
{
  Real C_diff = _C_gamma - _C_m;
  Real Part_1 = _b * (_C[_qp] - _C_m) / C_diff * (_C[_qp] - _C_m) / C_diff + _c * (_C[_qp] - _C_m) / C_diff + _d;
  Real Square_sum = _eta2[_qp]*_eta2[_qp] + _eta3[_qp]*_eta3[_qp];
  Real u_cubed = _u[_qp]*_u[_qp]*_u[_qp];
  Real u_squared = _u[_qp]*_u[_qp];

  switch (type)
  {
    case Residual:
      return  _G[_qp] * (Part_1 * (- 1 * _u[_qp] + 1 * u_cubed + 16 * _u[_qp] * Square_sum))/*that is dfdeta, now add dEdeta*/;

    case Jacobian:
      return _phi[_j][_qp] * ( _G[_qp] * (Part_1 * (- 1 + 3 * u_squared + 16 * Square_sum))/*that is dfdeta, now add dEdeta*/);
  }

  mooseError("Invalid type passed in");
}

Real
ACBulkZrH::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real C_diff = _C_gamma - _C_m;
  Real Part_1 = _b * (_C[_qp] - _C_m) / (C_diff) * (_C[_qp] - _C_m) / (C_diff) + _c * (_C[_qp] - _C_m) / (C_diff) + _d;
  Real Square_sum = _eta2[_qp] * _eta2[_qp]+_eta3[_qp] * _eta3[_qp];
  Real u_cubed = _u[_qp] * _u[_qp] * _u[_qp];
  Real Part_2 = (- 1 * _u[_qp] + 1 * u_cubed + 16 * _u[_qp] * Square_sum);

  if (jvar == _C_var)
    return _phi[_j][_qp] * _test[_i][_qp] * _G[_qp] * (_b * 2 * ((_C[_qp] - _C_m) / (C_diff * C_diff)) + _c*((1) / (C_diff))) * Part_2;
  else if (jvar == _eta2_var)
    return _phi[_j][_qp] * _test[_i][_qp] * _G[_qp] * (Part_1 * (16 * _u[_qp] * (2 * _eta2[_qp])));
  else if (jvar == _eta3_var)
    return _phi[_j][_qp] * _test[_i][_qp] * _G[_qp] * (Part_1 * (16 * _u[_qp] * (2 * _eta3[_qp])));

  return 0.0;
}
