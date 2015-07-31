#include "GammaZrHMaterial.h"

template<>
InputParameters validParams<GammaZrHMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addParam<Real>("length_scale", 1.0, "defines the base length scale of the problem in m");
  params.addParam<Real>("time_scale", 1.0, "defines the base time scale of the problem");
  return params;
}

GammaZrHMaterial::GammaZrHMaterial(const InputParameters & parameters) :
    Material(parameters),
    _T(coupledValue("T")),
    _length_scale(getParam<Real>("length_scale")),
    _time_scale(getParam<Real>("time_scale")),
    _L(declareProperty<Real>("L")),
    _M(declareProperty<Real>("M")),
    _grad_M(declareProperty<RealGradient>("grad_M")),
    _kappa(declareProperty<Real>("kappa_op")),
    _beta(declareProperty<Real>("beta")), // concentration gradient energy coeff
    _C_m(0.05),
    _C_gamma(0.5),
    _G(declareProperty<Real>("G"))
{
}

void
GammaZrHMaterial::computeQpProperties()
{
  // Real T = _T[_qp] - 273.15;

  _kappa[_qp] = 1.0;
  _beta[_qp] = _kappa[_qp] / (_C_gamma - _C_m);

  _M[_qp] = 1.0;
  _grad_M[_qp] = 0.0;

  _L[_qp] = 1.0;
  _G[_qp] = 1.0;

  /* C1111 = 155.4e9;
  C1122 = 68.03e9;
  C1133 = 64.6e9;
  C2222 = C1111;
  C2233 = C1133;
  C2323 = 36.31e9;
  C3333 = 172.51e9;
  C1313 = C2323;
  C1212 = 44.09e9;*/
}
