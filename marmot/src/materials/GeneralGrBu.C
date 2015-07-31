#include "GeneralGrBu.h"

template<>
InputParameters validParams<GeneralGrBu>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("Dg","Gas diffusivity");
  params.addRequiredParam<Real>("L","Order parameter mobility");
  params.addRequiredParam<Real>("Ao","Coefficient that scales the energy density");
  params.addRequiredParam<Real>("BetaS","Free energy parameter that scales the free surface energy");
  params.addRequiredParam<Real>("BetaGB","Free energy parameter that scales the grain boundary energy");
  params.addRequiredParam<Real>("kappSS","Kappa for free surface energy");
  params.addRequiredParam<Real>("kappGB","Kappa for grain boundary energy");
  params.addRequiredCoupledVar("cg", "gas concentration");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");

  return params;
}

GeneralGrBu::GeneralGrBu(const InputParameters & parameters) :
    Material(parameters),
    _cg(coupledValueOld("cg")),
    _grad_cg(coupledGradientOld("cg")),
    _kappSS_input(getParam<Real>("kappSS")),
    _kappGB_input(getParam<Real>("kappGB")),
    _Dg_input(getParam<Real>("Dg")),
    _L_input(getParam<Real>("L")),
    _Ao_input(getParam<Real>("Ao")),
    _BetaS_input(getParam<Real>("BetaS")),
    _BetaGB_input(getParam<Real>("BetaGB")),
    _kappa(declareProperty<Real>("kappa")),
    _kappaSS(declareProperty<Real>("kappaSS")),
    _Dg(declareProperty<Real>("Dg")),
    _Dg2(declareProperty<Real>("Dg2")),
    _grad_Dg(declareProperty<RealGradient>("grad_Dg")),
    _L(declareProperty<Real>("L")),
    _Ao(declareProperty<Real>("Ao")),
    _BetaS(declareProperty<Real>("BetaS")),
    _BetaGB(declareProperty<Real>("BetaGB")),
    _mu(declareProperty<Real>("mu"))
{
  unsigned int n = coupledComponents("v");
  _vals.resize(n);
  _grad_vals.resize(n);

  for (unsigned int i = 0; i < n; ++i)
  {
    _vals[i] = &coupledValueOld("v", i);
    _grad_vals[i] = &coupledGradientOld("v", i);
  }
}

void
GeneralGrBu::computeProperties()
{
  for (unsigned int qp = 0; qp < _qrule->n_points(); ++qp)
  {
    Real P1 = (*_vals[0])[qp];
    Real P2 = (*_vals[1])[qp];
    Real cgi = _cg[qp];

    RealGradient grad_P1 = (*_grad_vals[0])[qp];
    RealGradient grad_P2 = (*_grad_vals[1])[qp];
    RealGradient grad_cgi = _grad_cg[qp];

    Real P1P2 = P1*P1*P2*P2;
    Real P1cg = P1*P1*cgi*cgi;
    Real P2cg = P2*P2*cgi*cgi;

    if ((P1P2+P1cg+P2cg) > 0.0)
      _kappa[qp] = (_kappGB_input*P1P2 + _kappSS_input*(P1cg + P2cg)) / (P1P2 + P1cg + P2cg);
    else
      _kappa[qp] = _kappGB_input;

    _Dg[qp] = _Dg_input;

    _grad_Dg[qp] = 0.0;
    // _grad_Dg[qp] = 2.0*(P1*grad_P1 + P2*grad_P2 + cgi*grad_cgi)*_Dg_input/((P1*P1 + P2*P2 + cgi*cgi)*(P1*P1 + P2*P2 + cgi*cgi));
    _L[qp] = _L_input;
    _Ao[qp] = _Ao_input;
    _BetaS[qp] = _BetaS_input;
    _BetaGB[qp] = _BetaGB_input;

    _mu[qp] = 0.3;

    _Dg2[qp] = _Dg[qp]*(1+3*cgi*cgi);
  }
}
