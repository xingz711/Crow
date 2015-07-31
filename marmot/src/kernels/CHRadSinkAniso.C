#include "CHRadSinkAniso.h"
#include "MathUtils.h"

template<>
InputParameters validParams<CHRadSinkAniso>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

CHRadSinkAniso::CHRadSinkAniso(const InputParameters & parameters) :
    CHBulk(parameters),
    _vC_i(getMaterialProperty<std::vector<Real> >("vC_i")),
    _vm_i(getMaterialProperty<std::vector<Real> >("vm_i")),
    _vb_i(getMaterialProperty<std::vector<Real> >("vb_i")),
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

RealGradient
CHRadSinkAniso::computeGradDFDCons(PFFunctionType type)
{
  const Real tol = 1.0e-5;

  // Terms used by the Residual and Jacobian calculation
  RealGradient outval1 = 0.0;
  Real outval2 = 0.0;
  RealGradient outval3 = 0.0;

  RealGradient doutval1 = 0.0;
  Real doutval2 = 0.0;
  RealGradient doutval3 = 0.0;

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                      * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp]
                                         * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      grad_f_i -=   8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                  * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

      Real inside_g_i = 100.0 * _u[_qp] + std::exp(-_vb_i[_qp][ind]/_vm_i[_qp][ind]);

      Real g_i = _vm_i[_qp][ind] * MathUtils::poly2Log(inside_g_i,tol,0) + _vb_i[_qp][ind];

      Real dgidc = 100.0 * _vm_i[_qp][ind] * MathUtils::poly2Log(inside_g_i, tol, 1);
      Real d2gidc2 = 100.0*100.0 * _vm_i[_qp][ind] * MathUtils::poly2Log(inside_g_i, tol, 2);
      Real d3gidc3 = 100.0*100.0*100.0 * _vm_i[_qp][ind] * MathUtils::poly2Log(inside_g_i, tol, 3);

      RealGradient grad_g_i = _grad_u[_qp] * dgidc;
      RealGradient grad_dgidc = _grad_u[_qp] * d2gidc2;

      outval1 += -grad_g_i*f_i + (_vC_i[_qp][ind] - g_i) * grad_f_i;
      outval2 += -dgidc * f_i;
      outval3 += grad_dgidc*f_i + dgidc * grad_f_i;

      if (type == Jacobian)
      {
        Real dg_i = _phi[_j][_qp] * dgidc;
        RealGradient dgrad_g_i = (_grad_phi[_j][_qp] * dgidc + _phi[_j][_qp] * grad_dgidc);

        Real ddgidc = _phi[_j][_qp] * d2gidc2;
        RealGradient dgrad_dgidc = (_grad_phi[_j][_qp] * d2gidc2 + _phi[_j][_qp] * _grad_u[_qp] * d3gidc3);

        doutval1 += -dgrad_g_i * f_i + (_vC_i[_qp][ind] - dg_i)*grad_f_i;
        doutval2 += -ddgidc * f_i;
        doutval3 += dgrad_dgidc * f_i + ddgidc * grad_f_i;
      }

      ind++;
    }

  switch (type)
  {
    case Residual: // Determine grad dFdc
      return 2*outval1 + 2*_grad_u[_qp]*outval2 + 2*_u[_qp]*outval3;

    case Jacobian:
      return 2*doutval1 + 2*(_grad_phi[_j][_qp]*outval2 + _grad_u[_qp]*doutval2) + 2*(_phi[_j][_qp]*outval3 + _u[_qp]*doutval3);
  }

  mooseError("Unsupported type passed in.");
}
