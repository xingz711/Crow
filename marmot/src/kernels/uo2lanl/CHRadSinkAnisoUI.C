#include "CHRadSinkAnisoUI.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHRadSinkAnisoUI>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "M", "The mobility used with the kernel");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<MaterialPropertyName>("LogC_name", "LogC", "For PolyLog");
  params.addParam<MaterialPropertyName>("LogTol_name", "LogTol", "For PolyLog");
  return params;
}

CHRadSinkAnisoUI::CHRadSinkAnisoUI(const InputParameters & parameters) :
    Kernel(parameters),
    _vUIC_i(getMaterialPropertyByName<std::vector<Real> >("vUIC_i")),
    _vUIm_i(getMaterialPropertyByName<std::vector<Real> >("vUIm_i")),
    _vUIb_i(getMaterialPropertyByName<std::vector<Real> >("vUIb_i")),
    _M(getMaterialProperty<Real>("mob_name")),
    _kb(getMaterialPropertyByName<Real>("kb")),
    _kT(getMaterialPropertyByName<Real>("kT")),
    _LogC(getMaterialProperty<Real>("LogC_name")),
    _LogTol(getMaterialProperty<Real>("LogTol_name")),
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (unsigned int i = 0; i < _n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

Real
CHRadSinkAnisoUI::computeQpResidual()
{

  //Segregation of UI to grain boundaries, presently not used and not up to date

  const Real tol = _LogTol[_qp];

  RealGradient outval1 = 0.0;

  Real ytot = _u[_qp];
  RealGradient grad_ytot = _grad_u[_qp];

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i) {
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp]*(*_vals[j])[_qp];

      RealGradient grad_f_i = -8.0*2.0*(*_vals[i])[_qp]*(*_grad_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

      grad_f_i -= 8.0*(*_vals[i])[_qp]*(*_vals[i])[_qp]*2.0*(*_vals[j])[_qp]*(*_grad_vals[j])[_qp];

      Real inside_g_i = 100.0*ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]);

      Real dgidc = 100.0*_vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

      Real g_i = _vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vUIb_i[_qp][ind];

      RealGradient grad_g_i = grad_ytot*dgidc;

      outval1 += -grad_g_i*f_i + (_vUIC_i[_qp][ind] - g_i)*grad_f_i;

      ind++;

    }
  }

  return  _grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2 * outval1;
}


Real
CHRadSinkAnisoUI::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  // Terms used by the Residual and Jacobian calculation

  Real ytot = _u[_qp];
  RealGradient grad_ytot= _grad_u[_qp];

  RealGradient outval1 = 0.0;
  RealGradient doutval1 = 0.0;

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i) {
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0*(*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

      RealGradient grad_f_i = -8.0*2.0*(*_vals[i])[_qp]*(*_grad_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

      grad_f_i -= 8.0*(*_vals[i])[_qp]*(*_vals[i])[_qp]*2.0*(*_vals[j])[_qp]*(*_grad_vals[j])[_qp];

      Real inside_g_i = 100.0*ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]);

      Real dgidc = 100.0*_vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

      Real g_i = _vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vUIb_i[_qp][ind];

      RealGradient grad_g_i = grad_ytot*dgidc;

      outval1 += -grad_g_i*f_i + (_vUIC_i[_qp][ind] - g_i)*grad_f_i;

      doutval1 +=   -_vUIm_i[_qp][ind]*100*_phi[_j][_qp]
                    * MathUtils::poly4Log((100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind])), tol, 1)
                    * grad_f_i -
                    (
                      ( _vUIm_i[_qp][ind] * 100.0 * _grad_phi[_j][_qp]
                        * (100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]))
                        - 100.0 * _phi[_j][_qp] * _vUIm_i[_qp][ind] * 100.0 * grad_ytot
                      )
                      * MathUtils::poly4Log(((100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]))),tol,2)*(-1.0)
                    )
                    * f_i;

      ind++;
    }
  }

  return  (_grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2.0 * doutval1
           + _grad_test[_i][_qp] * _M[_qp] * _phi[_j][_qp] * 2.0 * outval1);
}
