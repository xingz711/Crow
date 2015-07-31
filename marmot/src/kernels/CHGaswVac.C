#include "CHGaswVac.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHGaswVac>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVar("cv", "Vacancy Concentration");
  return params;
}

CHGaswVac::CHGaswVac(const InputParameters & parameters) :
    CHBulk(parameters),
    _cv(coupledValue("cv")),
    _grad_cv(coupledGradient("cv")),
    _kT(getMaterialPropertyByName<Real>("kT")),
    _Efg(getMaterialPropertyByName<Real>("Efg")),
    _mug0(getMaterialPropertyByName<Real>("mug0"))
{
}

RealGradient
CHGaswVac::computeGradDFDCons(PFFunctionType type)
{
  Real cg = _u[_qp];
  RealGradient grad_cg = _grad_u[_qp];

  Real cv = _cv[_qp];
  RealGradient grad_cv = _grad_cv[_qp];

  Real mcvcg = 1.0 - cv - cg;

  Real cglogcg = 0.0;
  if (cg > 0.0) // Exact representation
    cglogcg = cg * std::log(cg);

  Real mcvcglogmcvcg = 0.0;
  if (mcvcg > 0.0) // Exact representation
    mcvcglogmcvcg = mcvcg*std::log(mcvcg);

  if (_kT[_qp] == 0.0)
    mooseError("problem in CHGaswVac");

  Real logkT = std::log(_kT[_qp]);

  Real dfmdcg = _Efg[_qp] * cg * mcvcg + _kT[_qp] * (mcvcg * cglogcg - cg * mcvcglogmcvcg);
  Real d2fmdcg2 = _kT[_qp] * (1.0 - cv);
  Real d2fmdcvdcg = _kT[_qp] * cg;

  Real j = cv*cv;
  Real dj = 2.0 * cv;
  Real h = (1.0 - cv) * (1.0 - cv);
  Real dh = -2.0 * (1.0 - cv);

  Real d2fbdcg2 = mcvcg * _kT[_qp];
  Real dfbdcg = mcvcg * (cg * _mug0[_qp] + _kT[_qp] * (cg + cglogcg + cg * logkT));
  // d2fbdcg2 = 0.0;
  // dfbdcg = 0.0;

  Real d2f0dcg2 = h * d2fmdcg2 + j * d2fbdcg2;
  Real d2f0dcvdcg = dh * dfmdcg + h * d2fmdcvdcg + dj * dfbdcg;

  switch (type)
  {
    case Residual:
      return d2f0dcg2*grad_cg + d2f0dcvdcg*grad_cv;

    case Jacobian:
    {
      Real Ddfmdcg =   _Efg[_qp] * (-cg + mcvcg)
                     + _kT[_qp] * (-cglogcg + mcvcg*(1.0 /*+ logcg*/) - mcvcglogmcvcg + cg*(1.0 /*+ logmcvcg*/));
      Real Dd2fmdcg2 =  0.0;
      Real Dd2fmdcvdcg = _kT[_qp];
      Real Dd2fbdcg2 = -_kT[_qp];
      Real Ddfbdcg =   -(cg * _mug0[_qp] + _kT[_qp] * (cg + cglogcg + cg * logkT))
                     + mcvcg * (_mug0[_qp] + _kT[_qp] * (1.0 /*+ logcg*/ + logkT));
      // Dd2fbdcg2=0.0;
      // Ddfbdcg = 0.0;
      Real Dd2f0dcg2 = (h*Dd2fmdcg2 + j*Dd2fbdcg2)*_phi[_j][_qp];
      Real Dd2f0dcvdcg = (dh*Ddfmdcg + h*Dd2fmdcvdcg + dj*Ddfbdcg)*_phi[_j][_qp];
      RealGradient jacobian = d2f0dcg2*_grad_phi[_j][_qp] + Dd2f0dcg2*grad_cg + d2f0dcvdcg*_grad_phi[_j][_qp] + Dd2f0dcvdcg*grad_cv;
      return jacobian;
    }
  }

  mooseError("Internal error");
}
