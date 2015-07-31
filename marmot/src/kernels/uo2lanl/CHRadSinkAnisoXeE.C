#include "CHRadSinkAnisoXeE.h"
#include "MathUtils.h"

template<>
InputParameters validParams<CHRadSinkAnisoXeE>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name","M","The mobility used with the kernel");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
 return params;
}

CHRadSinkAnisoXeE::CHRadSinkAnisoXeE(const InputParameters & parameters) :
    Kernel(parameters),
    _vC_i(getMaterialProperty<std::vector<Real> >("vC_i")),
    _vm_i(getMaterialProperty<std::vector<Real> >("vm_i")),
    _vb_i(getMaterialProperty<std::vector<Real> >("vb_i")),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _M(getMaterialProperty<Real>(_mob_name)),
    _LogC(getMaterialProperty<Real>("LogC")),
    _LogTol(getMaterialProperty<Real>("LogTol")),
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
CHRadSinkAnisoXeE::computeQpResidual()
{

  //Segregation of Xe to grain boundaries. attempt to rewrite how grain boundaries interact, no longer in use

  const Real tol = _LogTol[_qp];

  //Terms used by the Residual and Jacobian calculation
  RealGradient outval1 = 0.0;

  unsigned int ind = 0;
  unsigned int indi = 0;
  unsigned int indj = 0;

  Real ytot = _u[_qp];
  RealGradient grad_ytot = _grad_u[_qp];

  for (unsigned int i = 0; i < _n; ++i) {
    for (unsigned int j = i + 1; j < _n; ++j) {
      ind=i * 4 + (j - 1);

      Real Scale_ij=(*_vals[i])[_qp] + (*_vals[j])[_qp];

      RealGradient grad_Scale_ij=(*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp];

      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                 / (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

      RealGradient grad_f_i = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp]
                               * (*_vals[j])[_qp] * (*_vals[j])[_qp] - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp]
                               * (*_grad_vals[j])[_qp]) * (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

      grad_f_i -= 4 * grad_Scale_ij * Scale_ij * Scale_ij * Scale_ij
                  * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]);

      grad_f_i = grad_f_i / (Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij);

      f_i = f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (1);

      grad_f_i = grad_f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (1)
                 + f_i * ((*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp]) / (1);

      Real inside_g_i = 100.0 * ytot + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

      Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

      Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

      RealGradient grad_g_i = grad_ytot * dgidc;

      outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

      for (int k=j+1; k<_n; ++k)
      {
        indi=i * 4 + (k - 1);
        Real Scale_ik = (*_vals[i])[_qp] + (*_vals[k])[_qp];

        RealGradient grad_Scale_ik = (*_grad_vals[i])[_qp] + (*_grad_vals[k])[_qp];

        Real f_k = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                   / (Scale_ik * Scale_ik * Scale_ik * Scale_ik);

        RealGradient grad_f_k = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                                 - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[k])[_qp]
                                 * (*_grad_vals[k])[_qp]) * (Scale_ik * Scale_ik * Scale_ik * Scale_ik);

        grad_f_k -= 4 * grad_Scale_ik * Scale_ik * Scale_ik * Scale_ik
                    * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]);

        grad_f_k = grad_f_k / (Scale_ik * Scale_ik * Scale_ik * Scale_ik * Scale_ik * Scale_ik * Scale_ik * Scale_ik);

        f_k = f_k * ((*_vals[i])[_qp] + (*_vals[k])[_qp]) / (1);

        grad_f_k = grad_f_k * ((*_vals[i])[_qp] + (*_vals[k])[_qp]) / (1)
                   + f_k * ((*_grad_vals[i])[_qp] + (*_grad_vals[k])[_qp]) / (1);

        Real inside_g_k = 100.0 * ytot + exp(-_vb_i[_qp][indi] / _vm_i[_qp][indi]);

        Real dgkdc = 100.0 * _vm_i[_qp][indi] * MathUtils::poly4Log(inside_g_k,tol,1);

        Real g_k = _vm_i[_qp][indi] * MathUtils::poly4Log(inside_g_k,tol,0) + _vb_i[_qp][indi];

        RealGradient grad_g_k = grad_ytot*dgkdc;

        indj=j * 4 + (k - 1);

        Real Scale_jk=(*_vals[j])[_qp] + (*_vals[k])[_qp];

        RealGradient grad_Scale_jk=(*_grad_vals[j])[_qp] + (*_grad_vals[k])[_qp];

        Real f_j = -8.0 * (*_vals[j])[_qp] * (*_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                   / (Scale_jk * Scale_jk * Scale_jk * Scale_jk);

        RealGradient grad_f_j = (-8.0 * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                                 - 8.0 * (*_vals[j])[_qp] * (*_vals[j])[_qp] * 2.0 * (*_vals[k])[_qp] * (*_grad_vals[k])[_qp])
                                * (Scale_jk * Scale_jk * Scale_jk * Scale_jk);

        grad_f_j -= 4 * grad_Scale_jk * Scale_jk * Scale_jk * Scale_jk
                    * (-8.0 * (*_vals[j])[_qp] * (*_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]);

        grad_f_j = grad_f_j / (Scale_jk * Scale_jk * Scale_jk * Scale_jk * Scale_jk * Scale_jk * Scale_jk * Scale_jk);

        f_j = f_j * ((*_vals[j])[_qp] + (*_vals[k])[_qp]) / (1);

        grad_f_j = grad_f_j * ((*_vals[j])[_qp] + (*_vals[k])[_qp]) / (1)
                   + f_j * ((*_grad_vals[j])[_qp] + (*_grad_vals[k])[_qp]) / (1);

        Real inside_g_j = 100.0 * ytot + exp(-_vb_i[_qp][indj] / _vm_i[_qp][indj]);

        Real dgjdc = 100.0 * _vm_i[_qp][indj] * MathUtils::poly4Log(inside_g_j,tol,1);

        Real g_j = _vm_i[_qp][indj] * MathUtils::poly4Log(inside_g_j,tol,0) + _vb_i[_qp][indj];

        RealGradient grad_g_j = grad_ytot * dgjdc;

        outval1 += -grad_g_k * f_k + (_vC_i[_qp][indi] - g_k) * grad_f_k;

        outval1 += -grad_g_j * f_j + (_vC_i[_qp][indj] - g_j) * grad_f_j;

      }

    }
  }

  return  _grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2 * outval1;
}

Real
CHRadSinkAnisoXeE::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  //Terms used by the Residual and Jacobian calculation
  RealGradient outval1 = 0.0;
  RealGradient doutval1 = 0.0;

  unsigned int ind = 0;
  unsigned int indi = 0;
  unsigned int indj = 0;

  Real ytot = _u[_qp];
  RealGradient grad_ytot = _grad_u[_qp];

  for (unsigned int i = 0; i < _n; ++i) {
    for (unsigned int j = i + 1; j < _n; ++j) {
      for (int k=j+1; k<_n; ++k)
      {
        ind = i + j;
        indi = i + k;
        indj = j + k;

        Real Scale_ij = (*_vals[i])[_qp] + (*_vals[j])[_qp];

        RealGradient grad_Scale_ij = (*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp];

        Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                   / (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        RealGradient grad_f_i = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]
                                 - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp])
                                * (Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        grad_f_i -= 4 * grad_Scale_ij * Scale_ij * Scale_ij * Scale_ij
                    * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp]);

        grad_f_i = grad_f_i / (Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij);

        f_i = f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (1);

        grad_f_i = grad_f_i * ((*_vals[i])[_qp] + (*_vals[j])[_qp]) / (1)
                   + f_i * ((*_grad_vals[i])[_qp] + (*_grad_vals[j])[_qp]) / (1);

        Real inside_g_i = 100.0 * ytot + exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]);

        Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,1);

        Real g_i = _vm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

        RealGradient grad_g_i = grad_ytot * dgidc;

        Real Scale_ik = (*_vals[i])[_qp] + (*_vals[k])[_qp];

        RealGradient grad_Scale_ik = (*_grad_vals[i])[_qp] + (*_grad_vals[k])[_qp];

        Real f_k = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                   / (Scale_ik * Scale_ik * Scale_ik * Scale_ik);

        RealGradient grad_f_k = (-8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                                 - 8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[k])[_qp] * (*_grad_vals[k])[_qp])
                                * (Scale_ik * Scale_ik * Scale_ik * Scale_ik);

        grad_f_k -= 4 * grad_Scale_ik * Scale_ik * Scale_ik * Scale_ik
                    * (-8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]);

        grad_f_k = grad_f_k / (Scale_ik*Scale_ik*Scale_ik*Scale_ik*Scale_ik*Scale_ik*Scale_ik*Scale_ik);

        f_k = f_k * ((*_vals[i])[_qp] + (*_vals[k])[_qp]) / (1);

        grad_f_k = grad_f_k * ((*_vals[i])[_qp] + (*_vals[k])[_qp]) / (1)
                   + f_k * ((*_grad_vals[i])[_qp] + (*_grad_vals[k])[_qp]) / (1);

        Real inside_g_k = 100.0 * ytot + exp(-_vb_i[_qp][indi] / _vm_i[_qp][indi]);

        Real dgkdc = 100.0 * _vm_i[_qp][indi] * MathUtils::poly4Log(inside_g_k,tol,1);

        Real g_k = _vm_i[_qp][indi] * MathUtils::poly4Log(inside_g_k,tol,0) + _vb_i[_qp][indi];

        RealGradient grad_g_k = grad_ytot * dgkdc;

        Real Scale_jk = (*_vals[j])[_qp] + (*_vals[k])[_qp];

        RealGradient grad_Scale_jk = (*_grad_vals[j])[_qp] + (*_grad_vals[k])[_qp];

        Real f_j = -8.0 * (*_vals[j])[_qp] * (*_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                   / (Scale_jk * Scale_jk * Scale_jk * Scale_jk);

        RealGradient grad_f_j = (-8.0 * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]
                                 - 8.0 * (*_vals[j])[_qp] * (*_vals[j])[_qp] * 2.0 * (*_vals[k])[_qp] * (*_grad_vals[k])[_qp])
                                * (Scale_jk * Scale_jk * Scale_jk * Scale_jk);

        grad_f_j -= 4 * grad_Scale_jk * Scale_jk * Scale_jk * Scale_jk
                    * (-8.0*(*_vals[j])[_qp] * (*_vals[j])[_qp] * (*_vals[k])[_qp] * (*_vals[k])[_qp]);

        grad_f_j = grad_f_j / (Scale_jk*Scale_jk*Scale_jk*Scale_jk*Scale_jk*Scale_jk*Scale_jk*Scale_jk);

        f_j = f_j * ((*_vals[j])[_qp]+(*_vals[k])[_qp]) / (1);

        grad_f_j = grad_f_j * ((*_vals[j])[_qp] + (*_vals[k])[_qp]) / (1)
                   + f_j * ((*_grad_vals[j])[_qp] + (*_grad_vals[k])[_qp]) / (1);

        Real inside_g_j = 100.0 * ytot + exp(-_vb_i[_qp][indj] / _vm_i[_qp][indj]);

        Real dgjdc = 100.0 * _vm_i[_qp][indj] * MathUtils::poly4Log(inside_g_j,tol,1);

        Real g_j = _vm_i[_qp][indj] * MathUtils::poly4Log(inside_g_j,tol,0) + _vb_i[_qp][indj];

        RealGradient grad_g_j = grad_ytot * dgjdc;

        outval1 += -grad_g_i * f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;

        outval1 += -grad_g_k * f_k + (_vC_i[_qp][indi] - g_k) * grad_f_k;

        outval1 += -grad_g_j * f_j + (_vC_i[_qp][indj] - g_j) * grad_f_j;

        doutval1 += (-_vm_i[_qp][ind] * 100 * _phi[_j][_qp] * MathUtils::poly4Log((100 * ytot + std::exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind])),tol,1)*grad_f_i
                     - ((_vm_i[_qp][ind] * 100 * _grad_phi[_j][_qp] * (100 * ytot + std::exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]))
                         - 100 * _phi[_j][_qp] * _vm_i[_qp][ind] * 100 * grad_ytot) * MathUtils::poly4Log(((100 * ytot + std::exp(-_vb_i[_qp][ind] / _vm_i[_qp][ind]))),tol,2) * (-1)) * f_i);

        doutval1 += (-_vm_i[_qp][indi] * 100 * _phi[_j][_qp] * MathUtils::poly4Log((100 * ytot + std::exp(-_vb_i[_qp][indi] / _vm_i[_qp][indi])),tol,1) * grad_f_k
                     - ((_vm_i[_qp][indi] * 100 * _grad_phi[_j][_qp] * (100 * ytot + std::exp(-_vb_i[_qp][indi] / _vm_i[_qp][indi]))
                         - 100 * _phi[_j][_qp] * _vm_i[_qp][indi] * 100 * grad_ytot) * MathUtils::poly4Log(((100 * ytot + std::exp(-_vb_i[_qp][indi] / _vm_i[_qp][indi]))),tol,2) * (-1)) * f_k);

        doutval1 += (-_vm_i[_qp][indj] * 100 * _phi[_j][_qp] * MathUtils::poly4Log((100 * ytot + std::exp(-_vb_i[_qp][indj] / _vm_i[_qp][indj])),tol,1) * grad_f_j
                     - ((_vm_i[_qp][indj] * 100 * _grad_phi[_j][_qp] * (100 * ytot + std::exp(-_vb_i[_qp][indj] / _vm_i[_qp][indj]))
                         - 100 * _phi[_j][_qp] * _vm_i[_qp][indj] * 100 * grad_ytot) * MathUtils::poly4Log(((100 * ytot + std::exp(-_vb_i[_qp][indj] / _vm_i[_qp][indj]))),tol,2)*(-1)) * f_j);


      }
    }
  }

  return 1.0 * (   _grad_test[_i][_qp] * _M[_qp] * _u[_qp] * 2.0 * doutval1
                   + _grad_test[_i][_qp] * _M[_qp] * _phi[_j][_qp] * 2.0 * outval1
               );
}
