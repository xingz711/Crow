#include "XeDiffusion.h"
#include "MathUtils.h"

template<>
InputParameters validParams<XeDiffusion>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("D_name", "D", "The name of the diffusivity");
  params.addParam<std::string>("LXe_name", "LXe", "The name of the regular solution parameter");
  params.addParam<std::string>("MXe_name", "MXe", "The name of the Xe mobility parameter");
  params.addParam<std::string>("LogC_name", "LogC",  "*undocumented*");
  params.addParam<std::string>("LogTol_name", "LogTol",  "*undocumented*");
  params.addRequiredCoupledVar("c1", "Xe Concentration");
  return params;
}

XeDiffusion::XeDiffusion(const InputParameters & parameters) :
    Kernel(parameters),
    _D_name(getParam<std::string>("D_name")),
    _D(getMaterialProperty<Real>(_D_name)),
    _LXe_name(getParam<std::string>("LXe_name")),
    _LXe(getMaterialProperty<Real>(_LXe_name)),
    _MXe_name(getParam<std::string>("MXe_name")),
    _MXe(getMaterialProperty<Real>(_MXe_name)),
    _c1Old(coupledValueOld("c1")),
    _LogC(getMaterialProperty<Real>("LogC")),
    _LogTol(getMaterialProperty<Real>("LogTol"))
{
}

Real
XeDiffusion::computeQpResidual()
{

  //Xe Diffusion (binary)

  const Real tol=_LogTol[_qp];

  return   _D[_qp] * _grad_test[_i][_qp] * _grad_u[_qp]
           - 2.0 * _MXe[_qp] * _LXe[_qp] * _grad_test[_i][_qp] * _grad_u[_qp]
           + _D[_qp] * _grad_test[_i][_qp] * _grad_u[_qp] * _u[_qp] * MathUtils::poly4Log(1-_u[_qp], tol, 1);
}

Real
XeDiffusion::computeQpJacobian()
{
  const Real tol=_LogTol[_qp];

  return   _D[_qp] * _grad_test[_i][_qp] * _grad_phi[_j][_qp]
           - 2.0 * _MXe[_qp] * _LXe[_qp] * _grad_test[_i][_qp] * _grad_phi[_j][_qp]
           - 2.0 * _MXe[_qp]/_u[_qp] * _phi[_j][_qp] * _LXe[_qp] * _grad_test[_i][_qp] * _grad_u[_qp]
           + _D[_qp] * _grad_test[_i][_qp] * _grad_u[_qp] * (_phi[_j][_qp]
               * ((1.0 - _u[_qp]) - _u[_qp] * (-_phi[_j][_qp])) * MathUtils::poly4Log((1.0 - _u[_qp]), tol, 2)) * -1.0
           + _D[_qp] * _u[_qp] * MathUtils::poly4Log(1.0 - _u[_qp], tol, 1) * _grad_test[_i][_qp] * _grad_phi[_j][_qp];
}
