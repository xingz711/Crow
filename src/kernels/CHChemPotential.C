#include "CHChemPotential.h"

template<>
InputParameters validParams<CHChemPotential>()
{
  InputParameters params = validParams<CHBulk>();

  return params;
}

CHChemPotential::CHChemPotential(const std::string & name, InputParameters parameters) :
    CHBulk(name, parameters)
{
}

RealGradient  //Use This an example of the the function should look like
CHChemPotential::computeGradDFDCons(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return 34*_grad_u[_qp] - 12*16*_u[_qp]*_u[_qp]*_grad_u[_qp] ; // return Residual value

    case Jacobian:
      return 34*_grad_phi[_j][_qp] - 24*16*_u[_qp]*_grad_u[_qp]*_phi[_j][_qp] - 12*16*_u[_qp]*_u[_qp]*_grad_phi[_j][_qp] - _grad_phi[_j][_qp]; //return Jacobian value
  }

  mooseError("Invalid type passed in");
}
