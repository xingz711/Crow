#include "UIDiffusion.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UIDiffusion>()
{
  InputParameters params = validParams<CHBulk>();
  params.addParam<MaterialPropertyName>("mob_name", "MUI", "Mobility for interstitial diffusion");
  return params;
}

UIDiffusion::UIDiffusion(const InputParameters & parameters) :
    CHBulk(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MUI(getMaterialProperty<Real>(_mob_name)),
    _kT(getMaterialProperty<Real>("kT"))
{
}

RealGradient  // Use This an example of the the function should look like
UIDiffusion::computeGradDFDCons(PFFunctionType type)
{

  //Diffusion of interstitials, specifically for UI in the Xe, Va, XeV, VaVa, UI and Va_I system

  switch (type)
  {
    case Residual:
      return _kT[_qp] * (_grad_u[_qp] + _u[_qp] / (1.0-_u[_qp]) * _grad_u[_qp]);

    case Jacobian:
      return _kT[_qp] * (
               _grad_phi[_j][_qp]
               + _phi[_j][_qp] / (1.0-_u[_qp]) * _grad_u[_qp]
               + _u[_qp] / (1.0-_u[_qp]) * _grad_phi[_j][_qp]
               - _u[_qp] / (1.0 -_u[_qp]) / (1.0-_u[_qp]) * _phi[_j][_qp] * _grad_u[_qp]
             );
  }

  mooseError("Invalid type passed in");
}
