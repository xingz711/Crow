#include "CVap.h"

template<>
InputParameters validParams<CVap>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("c2", "Va Concentration");
  params.addRequiredCoupledVar("c1", "Xe Concentration");
  params.addRequiredCoupledVar("c3", "XeVa cluster concentration");
  params.addRequiredCoupledVar("c4", "VaVa cluster concentration");
  params.addRequiredCoupledVar("c7", "XeVa equilibrium cluster concentration");

  return params;
}

CVap::CVap(const InputParameters & parameters) :
    Kernel(parameters),
    _c1(coupledValue("c1")),
    _c2(coupledValue("c2")),
    _c3(coupledValue("c3")),
    _c4(coupledValue("c4")),
    _c7(coupledValue("c7")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _kT(getMaterialProperty<Real>("kT"))
{
}

Real
CVap::computeQpResidual()
{

  //This kernel is no longer used, can be deleted

  Real Keq = std::exp(-_EBVaVa[_qp]/_kT[_qp]) * std::exp(_SBVaVa[_qp]);
  Real cv = _c2[_qp] + _c3[_qp] + 2.0 * _c4[_qp] - 2.0 * _u[_qp] - _c7[_qp];

  return _test[_i][_qp] * (_u[_qp] - cv*cv * Keq);
}


Real
CVap::computeQpJacobian()
{
  Real Keq = std::exp(-_EBVaVa[_qp]/_kT[_qp]) * std::exp(_SBVaVa[_qp]);
  Real cv = _c2[_qp] + _c3[_qp] + 2.0 * _c4[_qp] - 2.0 * _u[_qp] - _c7[_qp];

  return _test[_i][_qp] * (_phi[_j][_qp] + 2.0*2.0 * cv * _phi[_j][_qp] * Keq);
}
