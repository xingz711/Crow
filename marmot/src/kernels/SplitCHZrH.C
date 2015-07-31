#include "SplitCHZrH.h"
// The couple, SplitCHMath and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
template<>
InputParameters validParams<SplitCHZrH>()
{
  InputParameters params = validParams<SplitCHCRes>();

  params.addRequiredCoupledVar("eta1","Order parameter 1");
  params.addRequiredCoupledVar("eta2","Order parameter 2");
  params.addRequiredCoupledVar("eta3","Order parameter 3");
  params.addParam<Real>("C_m",0.05,"Concentration of Hydrogen in Matrix");
  params.addParam<Real>("C_gamma",0.5,"Concentration of Hydrogen in Hydride");
  params.addParam<Real>("b",-1,"double well function coefficient");
  params.addParam<Real>("c",10,"double well function coefficient");
  params.addParam<Real>("a",1,"double well function coefficient");

  return params;
}

SplitCHZrH::SplitCHZrH(const InputParameters & parameters) :
  SplitCHCRes(parameters),
    _eta1(coupledValue("eta1")),
    _eta1_var(coupled("eta1")),
    _eta2(coupledValue("eta2")),
    _eta2_var(coupled("eta2")),
    _eta3(coupledValue("eta3")),
    _eta3_var(coupled("eta3")),
    _G(getMaterialProperty<Real>("G")),
    _b(getParam<Real>("b")),
    _c(getParam<Real>("c")),
    _a(getParam<Real>("a")),
    _C_m(getParam<Real>("C_m")),
    _C_gamma(getParam<Real>("C_gamma"))

{
}

Real
SplitCHZrH::computeDFDC(PFFunctionType type)
{

  Real C_diff = _C_gamma - _C_m;
  Real eta1_fourth = _eta1[_qp] * _eta1[_qp] * _eta1[_qp] * _eta1[_qp];
  Real eta2_fourth = _eta2[_qp] * _eta2[_qp] * _eta2[_qp] * _eta2[_qp];
  Real eta3_fourth = _eta3[_qp] * _eta3[_qp] * _eta3[_qp] * _eta3[_qp];
  Real eta1_square = _eta1[_qp] * _eta1[_qp];
  Real eta2_square = _eta2[_qp] * _eta2[_qp];
  Real eta3_square = _eta3[_qp] * _eta3[_qp];
  Real Part_2 = (- 0.5 * (eta1_square + eta2_square + eta3_square) + 0.25 * (eta1_fourth + eta2_fourth + eta3_fourth) + 8 * (eta1_square * eta2_square + eta1_square * eta3_square + eta2_square * eta3_square));


  switch (type)
  {
    case Residual:
      return _G[_qp] * (-20 * exp(-20 * (_u[_qp] + 0.2)) /*+ 20 * exp(20 * (_u[_qp] - 0.7)) */+ 2 * _a * ((_u[_qp] - _C_m) / (C_diff * C_diff)) + (2 * _b * ((_u[_qp] - _C_m) / (C_diff * C_diff)) + _c * (1 / C_diff)) * Part_2); // return Residual value

    case Jacobian:
      return _phi[_j][_qp] * ( _G[_qp] * (400 * exp(-20 * (_u[_qp] + 0.2)) /*+ 400 * exp(20 * (_u[_qp] - 0.7)) */+ 2 * _a * (1 / (C_diff * C_diff)) + (2 * _b * (1 / (C_diff * C_diff))) * Part_2)); // return Jacobian value
  }

  mooseError("Invalid type passed in");
}

Real
SplitCHZrH::computeQpOffDiagJacobian(unsigned int jvar)
{

  Real C_diff = _C_gamma - _C_m;
  Real eta1_square = _eta1[_qp]*_eta1[_qp];
  Real eta2_square = _eta2[_qp]*_eta2[_qp];
  Real eta3_square = _eta3[_qp]*_eta3[_qp];

  if (jvar == _eta1_var)
    return _phi[_j][_qp] * _test[_i][_qp] * (_G[_qp]
* ((2 * _b * ((_u[_qp] - _C_m) / (C_diff * C_diff)) + _c * (1 / C_diff))
* (- 0.5 * (2 * _eta1[_qp]) + 0.25 * (4 * _eta1[_qp] * _eta1[_qp] * _eta1[_qp]) + 8 * (2 * _eta1[_qp] * eta2_square + _eta1[_qp] * 2 * eta3_square))));

  if (jvar == _eta2_var)
    return _phi[_j][_qp] * _test[_i][_qp] * _G[_qp]
      * ((2 * _b * ((_u[_qp] - _C_m) / (C_diff * C_diff)) + _c * (1 / C_diff))
* (- 0.5 * (2 * _eta2[_qp]) + 0.25 * (4 * _eta2[_qp] * _eta2[_qp] * _eta2[_qp]) + 8 * (eta1_square * 2 * _eta2[_qp] + 2 * _eta2[_qp] * eta3_square)));

  if (jvar == _eta3_var)
    return _phi[_j][_qp] * _test[_i][_qp] * _G[_qp]
* ((2 * _b * ((_u[_qp] - _C_m) / (C_diff * C_diff)) + _c * (1 / C_diff))
* (- 0.5 * (2 * _eta3[_qp]) + 0.25 * (4 * _eta3[_qp] * _eta3[_qp] * _eta3[_qp]) + 8 * (eta1_square * 2 * _eta3[_qp] + 2 * eta2_square * _eta3[_qp])));

  return SplitCHCRes::computeQpOffDiagJacobian(jvar);
}
