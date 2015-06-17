#include "ACParticleGrowth.h"

#include "Material.h"

template<>
InputParameters validParams<ACParticleGrowth>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("v", "Array of coupled variable names");
  params.addRequiredCoupledVar("c", "phase field variable");
  params.addParam<Real>("B", 1.0, "Constant value for B");
  return params;
}

ACParticleGrowth::ACParticleGrowth(const std::string & name, InputParameters parameters) :
    ACBulk(name,parameters),
    _c(coupledValue("c")),
    _c_var(coupled("c")),
    _L(getMaterialProperty<Real>("L")),
    _B(getParam<Real>("B"))
{
  // Array of coupled variables is created in the constructor
  _ncrys = coupledComponents("v"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
  _vals.resize(_ncrys); //Size variable arrays
  _vals_var.resize(_ncrys);

  // _gamma = 1.5;

  //Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

Real
ACParticleGrowth::computeDFDOP(PFFunctionType type)
{
  Real c = _c[_qp];
  if (c < 1.0e-8)
    c = 1e-6;
  if (c > 1.0)
    c = 0.999;

  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
    SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters


  //Calcualte either the residual or jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:
      return 12.0*_B* ((1-c)*_u[_qp] - (2 - c)*_u[_qp]*_u[_qp] + _u[_qp]*SumEtaj);

    case Jacobian:
      return 12.0*_B*(_phi[_j][_qp]*((1 - c)- 2*(2 - c)*_u[_qp] + SumEtaj + 2 * _u[_qp] * _u[_qp]));
  }

  mooseError("Invalid type passed in");
}

Real
ACParticleGrowth::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
    {
     Real dDFDOP = 12.0*_B*(_u[_qp] *_u[_qp] - _u[_qp])  * _phi[_j][_qp];
     return _L[_qp]*_test[_i][_qp]*dDFDOP;
    }
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if (jvar == _vals_var[i])
    {
      Real dSumEtaj = 2.0*_B*(*_vals[i])[_qp] * _phi[_j][_qp]; //Derivative of SumEtaj
      Real dDFDOP =  12.0*_B*_u[_qp] * dSumEtaj;
      return _L[_qp]*_test[_i][_qp]*dDFDOP;
    }
  }

  return 0.0;
}
