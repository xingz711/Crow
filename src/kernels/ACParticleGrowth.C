#include "ACParticleGrowth.h"

#include "Material.h"

template<>
InputParameters validParams<ACParticleGrowth>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("v", "Array of coupled variable names");
  params.addRequiredCoupledVar("c", "phase field variable");
  return params;
}

ACParticleGrowth::ACParticleGrowth(const std::string & name, InputParameters parameters) :
    ACBulk(name,parameters),
    _c(coupledValue("c")),
    _L(getMaterialProperty<Real>("L"))
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
  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
    SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters

 
  //Calcualte either the residual or jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:     
      return 12.0* ((1-_c[_qp]) - (2 - _c[_qp])*_u[_qp]*_u[_qp] + _u[_qp]*SumEtaj) ;

    case Jacobian:
      return 12.0*(_phi[_j][_qp]*( - 2*(2 - _c[_qp])*_u[_qp] + SumEtaj)) ;
  }

  mooseError("Invalid type passed in");
}

Real
ACParticleGrowth::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if (jvar == _vals_var[i])
    {
      Real dSumEtaj = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp]; //Derivative of SumEtaj
      Real dDFDOP =  12.0 * _u[_qp] * dSumEtaj;

      return _L[_qp] * _test[_i][_qp] * dDFDOP;
    }
  }

  return 0.0;
}
