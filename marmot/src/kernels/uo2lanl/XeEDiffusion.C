#include "XeEDiffusion.h"
#include "MathUtils.h"

template<>
InputParameters validParams<XeEDiffusion>()
{
  InputParameters params = validParams<CHBulk>();
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");
  return params;
}

XeEDiffusion::XeEDiffusion(const InputParameters & parameters) :
    CHBulk(parameters),
    _kT(getMaterialProperty<Real>("kT"))
{
}

RealGradient
XeEDiffusion::computeGradDFDCons(PFFunctionType type)
{
  //Xe diffusion (binary, no interaction parameters)
  switch (type)
  {
    case Residual:
      return _kT[_qp] * _grad_u[_qp];

    case Jacobian:
      return _kT[_qp] * _grad_phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}
