#include "CHVacwGas.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHVacwGas>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVar("cg", "Gas Concentration");
  return params;
}

CHVacwGas::CHVacwGas(const InputParameters & parameters) :
    CHBulk(parameters),
    _cg(coupledValue("cg")),
    _grad_cg(coupledGradient("cg")),
    _kT(getMaterialPropertyByName<Real>("kT")),
    _Efv(getMaterialPropertyByName<Real>("Efv")),
    _Efg(getMaterialPropertyByName<Real>("Efg")),
    _mug0(getMaterialPropertyByName<Real>("mug0"))
{
}

RealGradient
CHVacwGas::computeGradDFDCons(PFFunctionType type)
{
  const Real tol = 1.0e-10;

  Real w = _Efv[_qp];

  Real cv = _u[_qp];
  RealGradient grad_cv = _grad_u[_qp];

  Real cg = _cg[_qp];
  RealGradient grad_cg = _grad_cg[_qp];

  Real mcvcg = 1.0 - cv -cg;

  // Values to ensure no numerical issues
  Real cvlogcv = 0.0;
  if (cv > 0.0)
    cvlogcv = cv*std::log(cv);

  Real cglogcg = 0.0;
  if (cg > 0.0)
    cglogcg = cg*std::log(cg);

  Real mcvcglogmcvcg = 0.0;
  if (mcvcg > 0.0)
    mcvcglogmcvcg = mcvcg * std::log(mcvcg);

  Real logcg = std::log(tol);
  if (cg > tol)
    logcg = std::log(cg);

  if (_kT[_qp] == 0.0)
    mooseError("problem in CHVacwGas");

  const Real logkT = std::log(_kT[_qp]);

  // Free energy values

  Real h = (1.0 - cv)*(1.0 - cv);
  Real dh = -2.0 + 2.0*cv;
  Real d2h = 2.0;
  Real dj = 2.0*cv;
  Real d2j = 2.0;

  Real fm0 = _Efg[_qp]*cg + _kT[_qp]*(cvlogcv + cglogcg + mcvcglogmcvcg);
  Real fm = cv*mcvcg*fm0;
  Real dfmdcv = _kT[_qp]*(mcvcg*cvlogcv - cv*mcvcglogmcvcg);
  Real dfmdcg = _Efg[_qp]*cv*mcvcg + _kT[_qp]*(mcvcg*cv*logcg - cv*mcvcglogmcvcg);
  Real d2fmdcv2 = _kT[_qp]*(1.0 - cg);
  Real d2fmdcvdcg = _kT[_qp]*cv;

  Real d2fdwdcv2 = -2.0*w*cv*mcvcg;

  Real fb = cv*mcvcg*(_mug0[_qp]*cg + _kT[_qp]*cglogcg + cg*_kT[_qp]*logkT);
  Real dfb = cv*mcvcg*(_mug0[_qp] + _kT[_qp]*(1.0 + logcg) + _kT[_qp]*logkT);
  // fb = 0.0;
  // dfb = 0.0;

  Real d2f0dcv2 = d2h*fm + 2.0*dh*dfmdcv + h*d2fmdcv2 + d2j*fb + d2fdwdcv2;
  Real d2f0dcvdcg = dh*dfmdcg + h*d2fmdcvdcg + dj*dfb;

  switch (type)
  {
    case Residual:
    {
      RealGradient residual = d2f0dcv2*grad_cv + d2f0dcvdcg*grad_cg;

      if (libmesh_isnan(residual(0)))
      {
        Moose::err << "QP = " << _qp << std::endl;
        mooseError("is nan in CHVacwGas");
      }

      return residual;
    }

    case Jacobian:
    {
      Real Dfm = mcvcg*fm0 - cv*fm0 + dfmdcv;
      Real Ddfmdcv = _kT[_qp]*(-cvlogcv + mcvcg*(1.0 /*+ logcv*/) - mcvcglogmcvcg + cv*(1.0 /*+ logmcvcg*/));
      Real Ddfmdcg = _Efg[_qp]*(-cv + mcvcg) + _kT[_qp]*(-cv*logcg + mcvcg*logcg - mcvcglogmcvcg + cv*(1.0 /*+ logmcvcg*/));
      Real Dd2fmdcv2 = 0.0;
      Real Dd2fmdcvdcg = _kT[_qp];

      Real Dd2fdwdcv2 = -2.0*w*(-cv + mcvcg);

      Real Dd2f0dcv2 = _phi[_j][_qp]*(d2h*Dfm + 2.0*(dh*Ddfmdcv + d2h*dfmdcv) + (dh*d2fmdcv2 + h*Dd2fmdcv2) + Dd2fdwdcv2);
      Real Dd2f0dcvdcg = _phi[_j][_qp]*((d2h*dfmdcg + dh*Ddfmdcg) + (dh*d2fmdcvdcg + h*Dd2fmdcvdcg) + d2j*dfb);

      RealGradient jacobian = d2f0dcv2*_grad_phi[_j][_qp] + Dd2f0dcv2*grad_cv + d2f0dcvdcg*_grad_phi[_j][_qp] + Dd2f0dcvdcg*grad_cg;

      return jacobian;
    }
  }

  mooseError("Internal error");
}
