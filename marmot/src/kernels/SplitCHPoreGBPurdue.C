#include "SplitCHPoreGBPurdue.h"

template<>
InputParameters validParams<SplitCHPoreGBPurdue>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addClassDescription("Computes the dFdc term for the Pore/GB interaction model based on Ahmed2013");
  return params;
}

SplitCHPoreGBPurdue::SplitCHPoreGBPurdue(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _B(getMaterialProperty<Real>("B")),
    _C(getMaterialProperty<Real>("C")),
    _ncrys(coupledComponents("v"))
{
  _vals.resize(_ncrys); // Size variable arrays
  _vals_var.resize(_ncrys);

  // Array of coupled variables is created in the constructor
  for (unsigned int i = 0; i < _ncrys; ++i)
    {
      _vals[i] = &coupledValue("v", i);
      _vals_var[i] = coupled("v",i);
    }
}

Real
SplitCHPoreGBPurdue::computeDFDC(PFFunctionType type)
{
  // Calculate sums of grain order parameters
  Real sumop2 = 0.0;
  Real sumop3 = 0.0;

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    sumop2 += (*_vals[i])[_qp] * (*_vals[i])[_qp];
    sumop3 += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp];
  }

  Real c = _u[_qp];

  switch (type)
  {
    case Residual:
      return _B[_qp] * (2.0 * c - 6.0 * c * c + 4.0 * c * c * c) +
             _C[_qp] * (2.0 * (c - 1.0) + 6.0 * sumop2 - 4.0 * sumop3);

    case Jacobian:
      return (_B[_qp] * (2.0 - 12.0 * c + 12.0 * c * c) + _C[_qp] * 2.0) * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}

Real
SplitCHPoreGBPurdue::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Get off-diagonal terms from the parent class
  Real value = SplitCHCRes::computeQpOffDiagJacobian(jvar);

  // Get off-diagonal part from this class
  for (unsigned int i=0; i<_ncrys; ++i)
    if (jvar == _vals_var[i])
    {
      Real c = _u[_qp];

      // Derivative of sumop2
      Real dsumop2 = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp];

      // Derivative of sumop3
      Real dsumop3 = 3.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * _phi[_j][_qp];

      // Derivative of DFDOP
      Real dDFDC = _C[_qp] * (6.0 * dsumop2 - 4.0 * dsumop3);;

      // Full Jacobian
      value += _test[_i][_qp] * dDFDC;
    }

  return value;
}
