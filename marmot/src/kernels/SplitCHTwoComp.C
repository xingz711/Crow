#include "SplitCHTwoComp.h"

template<>
InputParameters validParams<SplitCHTwoComp>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addRequiredParam<Real>("a", "The a parameter");
  params.addRequiredParam<Real>("lambda", "The lambda parameter");
  params.addRequiredParam<Real>("c1", "The c1 parameter");
  params.addRequiredParam<Real>("c2", "The c2 parameter");
  params.addRequiredParam<Real>("c3", "The c3 parameter");
  params.addRequiredParam<Real>("c4", "The c4 parameter");
  params.addRequiredCoupledVar("q_alpha", "alpha component");
  return params;
}

SplitCHTwoComp::SplitCHTwoComp(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _a(getParam<Real>("a")),
    _lambda(getParam<Real>("lambda")),
    _c1(getParam<Real>("c1")),
    _c2(getParam<Real>("c2")),
    _c3(getParam<Real>("c3")),
    _c4(getParam<Real>("c4")),
    _q_alpha(coupledValue("q_alpha"))
{
}

Real
SplitCHTwoComp::computeDFDC(PFFunctionType type)
{
  const Real c = _u[_qp];
  const Real q_a = 1-_q_alpha[_qp];
  const Real q_b = 1-q_a;

  switch (type)
  {
    case Residual:
      return 2*(_lambda*(    (c-_c1)-    (_c2-c)) +
                     _a*(q_a*(c-_c3)-q_b*(_c4-c))); // return Residual value

    case Jacobian:
      return 2*(_lambda*(    (1)-    (-1)) +
                     _a*(q_a*(1)-q_b*(-1))) * _phi[_j][_qp]; // return Jacobian value
  }

  mooseError("Invalid type passed in");
}
