#include "CHRadSinkAnisoXe.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHRadSinkAnisoXe>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name","M","The mobility used with the kernel");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredCoupledVar("c1","Xe Concentration");
  params.addParam<std::string>("LogC_name","LogC","For PolyLog");
  params.addParam<std::string>("LogTol_name","LogTol","For PolyLog");

  return params;
}

CHRadSinkAnisoXe::CHRadSinkAnisoXe(const InputParameters & parameters) :
    Kernel(parameters),
    _vC_i(getMaterialProperty<std::vector<Real> >("vC_i")),
    _vm_i(getMaterialProperty<std::vector<Real> >("vm_i")),
    _vb_i(getMaterialProperty<std::vector<Real> >("vb_i")),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _M(getMaterialProperty<Real>(_mob_name)),
    _LogC(getMaterialProperty<Real>("LogC")),
    _LogTol(getMaterialProperty<Real>("LogTol"))

{
  _n = coupledComponents("v");
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (int i=0; i<_n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }

}

Real
CHRadSinkAnisoXe::computeQpResidual()
{

  //Xe segregation to grain boundaries

  Real tol = _LogTol[_qp];

  //Terms used by the Residual and Jacobian calculation

  //RealGradient outval;

  RealGradient outval1 = 0.0;

  unsigned int ind = 0;

  for (int i=0; i<_n; ++i)
    for (int j=i+1; j<_n; ++j)
    {
      if (((*_vals[i])[_qp] + (*_vals[j])[_qp])>0.9 || ((*_vals[i])[_qp] + (*_vals[j])[_qp]) < 0.1) {

        Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i -= 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

        Real inside_g_i = 100.0 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

        Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

        Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

        RealGradient grad_g_i = _grad_u[_qp] * dgidc;

        outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

      }

      else {
        Real Scale_ij=(*_vals[i])[_qp] + (*_vals[j])[_qp];

        RealGradient grad_Scale_ij=(*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp];

        Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                   / (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        RealGradient grad_f_i = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                                 - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp])
                                * (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        grad_f_i -= 4 * grad_Scale_ij * Scale_ij * Scale_ij * Scale_ij
                    * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]);

        grad_f_i = grad_f_i / (Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        f_i = f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (2);

        grad_f_i = grad_f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp])/(2)
                   + f_i * ((*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp]) / (2);

        Real inside_g_i = 100.0 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

        Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

        Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

        RealGradient grad_g_i = _grad_u[_qp] * dgidc;

        outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

      }
      ind++;

    }

  return  _grad_test[_i][_qp] * 2 * outval1 * _M[_qp] * _u[_qp];
  mooseError("Unsupported type passed in.");
}

Real
CHRadSinkAnisoXe::computeQpJacobian()
{

  Real tol = _LogTol[_qp];

  //Terms used by the Residual and Jacobian calculation

  //RealGradient outval;

  RealGradient outval1 = 0.0;
  RealGradient doutval1 = 0.0;

  unsigned int ind = 0;

  for (int i=0; i<_n; ++i)
    for (int j=i+1; j<_n; ++j)
    {

      if (((*_vals[i])[_qp] + (*_vals[j])[_qp]) > 0.9 || ((*_vals[i])[_qp] + (*_vals[j])[_qp]) < 0.1) {

        Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i -= 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

        Real inside_g_i = 100.0 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

        Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

        Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

        RealGradient grad_g_i = _grad_u[_qp] * dgidc;

        outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

        doutval1 += (-_vm_i[_qp][ind] * 100 * _phi[_j][_qp] * MathUtils::poly4Log(inside_g_i,tol,1) * grad_f_i
                     -((_vm_i[_qp][ind] * 100 * _grad_phi[_j][_qp] * (100 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]))
                        - 100 * _phi[_j][_qp] * _vm_i[_qp][ind] * 100 * _grad_u[_qp]) * MathUtils::poly4Log(inside_g_i,tol,2) * (-1))
                     * f_i);

      }

      else {
        Real Scale_ij=(*_vals[i])[_qp] + (*_vals[j])[_qp];

        RealGradient grad_Scale_ij=(*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp];

        Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                   / (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        RealGradient grad_f_i = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                                 - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp])
                                * (Scale_ij*Scale_ij * Scale_ij * Scale_ij);

        grad_f_i -= 4 * grad_Scale_ij * Scale_ij * Scale_ij * Scale_ij
                    * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]);

        grad_f_i = grad_f_i / (Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij);
        f_i = f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (2);

        grad_f_i = grad_f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (2)
                   + f_i * ((*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp]) / (2);

        Real inside_g_i = 100.0 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

        Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

        Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

        RealGradient grad_g_i = _grad_u[_qp] * dgidc;

        outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

        doutval1 += (-_vm_i[_qp][ind] * 100 * _phi[_j][_qp] * MathUtils::poly4Log(inside_g_i,tol,1) * grad_f_i
                     -((_vm_i[_qp][ind] * 100 * _grad_phi[_j][_qp] * (100 * _u[_qp] + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]))
                        -100 * _phi[_j][_qp] * _vm_i[_qp][ind] * 100 * _grad_u[_qp]) * MathUtils::poly4Log(inside_g_i,tol,2) * (-1))
                     * f_i);

      }
      ind++;

    }

  return _grad_test[_i][_qp] * 2 * doutval1 * _M[_qp] * _u[_qp]
         + _grad_test[_i][_qp] * 2 * outval1 * _M[_qp] * _phi[_j][_qp];

  mooseError("Unsupported type passed in.");

}
