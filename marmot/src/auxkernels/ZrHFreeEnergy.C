
#include "ZrHFreeEnergy.h"

template<>
InputParameters validParams<ZrHFreeEnergy>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("eta1", "Order parameter 1");
  params.addRequiredCoupledVar("eta2", "Order parameter 2");
  params.addRequiredCoupledVar("eta3", "Order parameter 3");
  params.addRequiredCoupledVar("C", "Order parameter 3");
  params.addParam<Real>("C_m", 0.05, "Concentration of Hydrogen in Matrix");
  params.addParam<Real>("C_gamma", 0.5, "Concentration of Hydrogen in Hydride");
  params.addParam<Real>("b", -1.0, "double well function coefficient");
  params.addParam<Real>("c", 10.0, "double well function coefficient");
  params.addParam<Real>("a",  1.0, "double well function coefficient");
  params.addParam<Real>("d", -1.0, "double well function coefficient");
  return params;
}

ZrHFreeEnergy::ZrHFreeEnergy(const InputParameters & parameters) :
    AuxKernel(parameters),
    _eta1(coupledValue("eta1")),
    _eta2(coupledValue("eta2")),
    _eta3(coupledValue("eta3")),
    _C(coupledValue("C")),
    _G(getMaterialProperty<Real>("G")),
    _b(getParam<Real>("b")),
    _c(getParam<Real>("c")),
    _a(getParam<Real>("a")),
    _d(getParam<Real>("d")),
    _C_m(getParam<Real>("C_m")),
    _C_gamma(getParam<Real>("C_gamma"))
{
}

Real
ZrHFreeEnergy::computeValue()
{
  Real C_diff = _C_gamma - _C_m;
  Real eta1_square = _eta1[_qp] * _eta1[_qp];
  Real eta2_square = _eta2[_qp] * _eta2[_qp];
  Real eta3_square = _eta3[_qp] * _eta3[_qp];
  Real eta1_fourth = eta1_square * eta1_square;
  Real eta2_fourth = eta2_square * eta2_square;
  Real eta3_fourth = eta3_square * eta3_square;

  Real Part_2 =    0.5 * (eta1_square + eta2_square + eta3_square)
                + 0.25 * (eta1_fourth + eta2_fourth + eta3_fourth)
                + 0.25 * (eta1_square * eta2_square + eta1_square * eta3_square + eta2_square * eta3_square);

  Real Part_1 =   _b * (_C[_qp] - _C_m) / C_diff * (_C[_qp] - _C_m) / C_diff
                + _c * (_C[_qp] - _C_m) / C_diff + _d;

  return _G[_qp] * (std::exp(-20.0 * (_C[_qp] + 0.2)) + std::exp(20.0 * (_C[_qp] - 0.7)) + _a * ((_C[_qp] - _C_m) / C_diff) * ((_C[_qp] - _C_m) / C_diff) + Part_1 * Part_2);
}
