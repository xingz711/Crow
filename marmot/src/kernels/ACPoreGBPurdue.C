#include "ACPoreGBPurdue.h"

template<>
InputParameters validParams<ACPoreGBPurdue>()
{
  InputParameters params = validParams<ACBulk>();
  params.addClassDescription("Computes the evolution for the Pore/GB interaction model based on Ahmed2013");
  params.addRequiredCoupledVar("v", "Array of order parameter names except the current one");
  params.addRequiredCoupledVar("c", "Concentration representing pores");
  return params;
}

ACPoreGBPurdue::ACPoreGBPurdue(const InputParameters & parameters) :
    ACBulk(parameters),
    _C(getMaterialProperty<Real>("C")),
    _c(coupledValue("c")),
    _c_var(coupled("c")),
    _ncrys(coupledComponents("v")) // determine number of grains minus 1 from the number of names passed in.
{
  // Array of coupled variables is created in the constructor
  _vals.resize(_ncrys); // Size variable arrays
  _vals_var.resize(_ncrys);
  for (unsigned int i=0; i<_ncrys; ++i)
  {
    // Loop through grains and load coupled variables into the arrays
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v",i);
  }
}

Real
ACPoreGBPurdue::computeDFDOP(PFFunctionType type)
{
  Real c = _c[_qp];
  Real etai = _u[_qp];

  Real sumop2 = 0.0;
  for (unsigned int i=0; i<_ncrys; ++i)
    sumop2 += (*_vals[i])[_qp]*(*_vals[i])[_qp]; // Sum over all order parameters except etai

  // Calculate either the residual or jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:
      return _C[_qp] * (12.0 * c*etai - 12.0 * (c + 1.0) * etai * etai +
                        12 * etai * (sumop2 + etai * etai));

    case Jacobian:
      return _C[_qp] * (12.0 * c - 24.0 * (c + 1.0) * etai +
                        12.0 * sumop2 + 36.0 * etai * etai) * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}

Real
ACPoreGBPurdue::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Get off-diagonal part from parent class
  Real value = ACBulk::computeQpOffDiagJacobian(jvar);

  //Get off-digaonal part from this class
  Real etai = _u[_qp];

  for (unsigned int i=0; i<_ncrys; ++i)
    if (jvar == _vals_var[i])
    {
      // Derivative of dsumop2
      Real dsumop2 = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp];

      // Derivative of DFDOP
      Real dDFDOP = _C[_qp] * 12.0 * etai * dsumop2;

      value += _L[_qp] * _test[_i][_qp] * dDFDOP;
    }

  if (jvar == _c_var)
    value += _L[_qp] * _C[_qp] * (12.0 * etai - 12.0 * etai * etai) * _phi[_j][_qp] * _test[_i][_qp];

  return value;
}
