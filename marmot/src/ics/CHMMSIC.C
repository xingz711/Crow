#include "CHMMSIC.h"

template<>
InputParameters validParams<CHMMSIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<Real>("alpha", "The value of the alpha constant");
  return params;
}

CHMMSIC::CHMMSIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _alpha(getParam<Real>("alpha"))
{
}

Real
CHMMSIC::value(const Point & p)
{
  return std::sin(_alpha * libMesh::pi * p(0));
}

RealGradient
CHMMSIC::gradient(const Point & p)
{
  Real val = _alpha * libMesh::pi * std::cos(_alpha * libMesh::pi * p(0));
  return Gradient(val,0.0, 0.0);
}
