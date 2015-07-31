#include "CHRadSinkAnisoVaVa.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHRadSinkAnisoVaVa>()
{
  InputParameters params = validParams<Kernel>();

  params.addParam<MaterialPropertyName>("mob_name","M","The mobility used with the kernel");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredCoupledVar("c1","Xe Concentration");
  params.addRequiredCoupledVar("c2","Va Concentration");
  params.addRequiredCoupledVar("c3","XeVa Concentration");
  params.addParam<std::string>("LogC_name","LogC","For PolyLog");
  params.addParam<std::string>("LogTol_name","LogTol","For PolyLog");

  return params;
}

CHRadSinkAnisoVaVa::CHRadSinkAnisoVaVa(const InputParameters & parameters) :
    Kernel(parameters),
    _vVaC_i(getMaterialProperty<std::vector<Real> >("vVaC_i")),
    _vVam_i(getMaterialProperty<std::vector<Real> >("vVam_i")),
    _vVab_i(getMaterialProperty<std::vector<Real> >("vVab_i")),
    _M(getMaterialProperty<Real>("mob_name")),
    _kb(getMaterialPropertyByName<Real>("kb")),
    _kT(getMaterialPropertyByName<Real>("kT")),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
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
CHRadSinkAnisoVaVa::computeQpResidual()
{

  //Segregation of VaVa to grain boundaries, no longer in use or up to date

  const Real tol = _LogTol[_qp];

  RealGradient outval1 = 0.0;

  Real ytot = 2 * _u[_qp] + _c1[_qp] + _c2[_qp] + 2 * _c3[_qp];

  RealGradient grad_ytot = 2 * _grad_u[_qp] + _grad_c1[_qp] + _grad_c2[_qp] + 2 * _grad_c3[_qp];

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      grad_f_i -= 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vVab_i[_qp][ind]/_vVam_i[_qp][ind]);

      Real dgidc = 100.0 * _vVam_i[_qp][ind] * poly4Log(inside_g_i,tol,1);

      Real g_i = _vVam_i[_qp][ind] * poly4Log(inside_g_i,tol,0) + _vVab_i[_qp][ind];

      RealGradient grad_g_i = grad_ytot * dgidc;

      outval1 += -grad_g_i * f_i + (_vVaC_i[_qp][ind] - g_i)*grad_f_i;

      ind++;

    }

  return 2.0 * _grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2.0 * outval1;
}

Real
CHRadSinkAnisoVaVa::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real ytot = 2.0 * _u[_qp] + _c1[_qp] + _c2[_qp] + 2.0 * _c3[_qp];

  RealGradient grad_ytot= 2.0 * _grad_u[_qp] + _grad_c1[_qp] + _grad_c2[_qp] + 2.0 * _grad_c3[_qp];

  RealGradient outval1 = 0.0;

  RealGradient doutval1 = 0.0;

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      grad_f_i -= 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0*(*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vVab_i[_qp][ind]/_vVam_i[_qp][ind]);

      Real dgidc = 100.0 * _vVam_i[_qp][ind] * poly4Log(inside_g_i,tol,1);

      Real g_i = _vVam_i[_qp][ind] * poly4Log(inside_g_i,tol,0) + _vVab_i[_qp][ind];

      RealGradient grad_g_i = grad_ytot * dgidc;

      outval1 += -grad_g_i * f_i + (_vVaC_i[_qp][ind] - g_i) * grad_f_i;

      doutval1+=(-_vVam_i[_qp][ind] * 100 * 2 * _phi[_j][_qp] *
                 poly4Log((100 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind])),tol,1) * grad_f_i -
                 ((_vVam_i[_qp][ind] * 100 * 2 * _grad_phi[_j][_qp] * (100 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind]))
                   - 100 * 2 * _phi[_j][_qp] * _vVam_i[_qp][ind] * 100 * grad_ytot) * poly4Log(((100 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind]))),tol,2) * (-1))
                 * f_i);

      ind++;
    }

  return 2.0 * (   _grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2.0 * doutval1
                   + _grad_test[_i][_qp] * _M[_qp] * _phi[_j][_qp] * 2.0 * outval1
               );
}
