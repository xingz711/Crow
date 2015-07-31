#include "CHVacGB.h"

template<>
InputParameters validParams<CHVacGB>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<MaterialPropertyName>("Ao_name", "Ao", "Coefficient that scales the energy density");
  params.addParam<MaterialPropertyName>("BetaS_name", "BetaS", "Free energy parameter that scales the free energy");
  return params;
}

CHVacGB::CHVacGB(const InputParameters & parameters) :
    CHBulk(parameters),
    _Ao(getMaterialProperty<Real>("Ao_name")),
    _BetaS(getMaterialProperty<Real>("BetaS_name"))
{
  unsigned int n = coupledComponents("v");

  _vals.resize(n);
  _grad_vals.resize(n);

  for (unsigned int i = 0; i < n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

RealGradient
CHVacGB::computeGradDFDCons(PFFunctionType type)
{
  // Calculate sums of grain order parameters and their gradients
  Real grnsum = 0.0;
  RealGradient grad_grnsum = 0.0;

  for (unsigned int i = 0; i < _vals.size(); ++i)
  {
    grnsum += (*_vals[i])[_qp] * (*_vals[i])[_qp];
    grad_grnsum += 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp];
  }

  // Calculate the required values for the residual and jacobian
  switch (type)
  {
    case Residual:
      return _Ao[_qp]*(_grad_u[_qp]*(3.0*_u[_qp]*_u[_qp] - 1.0) + 2*_BetaS[_qp]*(_grad_u[_qp] * grnsum + _u[_qp]*grad_grnsum));

    case Jacobian:
    {
      RealGradient grad_f_prime = _grad_phi[_j][_qp]*(3.0*_u[_qp]*_u[_qp] - 1.0) + _grad_u[_qp]*6.0*_u[_qp]*_phi[_j][_qp];
      grad_f_prime+=2*_BetaS[_qp]*(_grad_phi[_j][_qp]*grnsum + _phi[_j][_qp]*grad_grnsum);

      return _Ao[_qp]*grad_f_prime;
    }
  }

  mooseError("Internal error");
}
