#include "CGp.h"

template<>
InputParameters validParams<CGp>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addRequiredCoupledVar("c1", "Xe Concentration");
  params.addRequiredCoupledVar("c3", "XeVa Concentration");
  params.addRequiredCoupledVar("c4", "VaVa cluster concentration");
  params.addRequiredCoupledVar("c6", "VaVa equilibrium cluster concentration");
  return params;
}

CGp::CGp(const InputParameters & parameters) :
    Kernel(parameters),
    _c1(coupledValue("c1")),
    _c2(coupledValue("c2")),
    _c3(coupledValue("c3")),
    _c4(coupledValue("c4")),
    _c6(coupledValue("c6")),
    _EBXeVa(getMaterialProperty<Real>("EBgXeVa")),
    _SBXeVa(getMaterialProperty<Real>("SBgXeVa")),
    _kT(getMaterialProperty<Real>("kT"))
{
}

Real
CGp::computeQpResidual()
{
  //This kernel is no longer used, can be deleted

  Real Keq = std::exp(-_EBXeVa[_qp]/_kT[_qp]) * std::exp(_SBXeVa[_qp]);
  Real cg = _c1[_qp] + _c3[_qp] - _u[_qp];
  Real cv = _c2[_qp] + _c3[_qp] + 2.0 * _c4[_qp] - 2.0 * _c6[_qp] - _u[_qp];

  return _test[_i][_qp]*(_u[_qp]-cv*cg*Keq);
}

Real
CGp::computeQpJacobian()
{
  Real Keq=std::exp(-_EBXeVa[_qp]/_kT[_qp]) * std::exp(_SBXeVa[_qp]);
  Real cg=_c1[_qp] + _c3[_qp] - _u[_qp];
  Real cv=_c2[_qp] + _c3[_qp] + 2*_c4[_qp] - 2*_c6[_qp] - _u[_qp];

  return _test[_i][_qp] * (_phi[_j][_qp] + cv*_phi[_j][_qp] * Keq + _phi[_j][_qp] * cg * Keq);
}
