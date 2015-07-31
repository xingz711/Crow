#include "MidPlaneBubbleCoverage.h"

template<>
InputParameters validParams<MidPlaneBubbleCoverage>()
{
  InputParameters params = validParams<ElementAverageValue>();
  params.addRequiredParam<Real>("GB_lengthy", "The total length of the GB, including bubbles");
  params.addParam<Real>("GB_lengthz", 0.0, "The total length of the GB, including bubbles");
  params.addRequiredParam<Real>("Lx", "The length of the simulation cell in the x-direction");
  params.addParam<Real>("tol", 0.001, "Toolerance length for determining if a point is on the midplane of teh simulation cell.");
  params.addParam<unsigned int>("dim", 2, "dimensionality of the simulation");
  params.addParam<Real>("bub_threshold", 0.9, "TThe concentration value used to differentiate bubbles");

  return params;
}

MidPlaneBubbleCoverage::MidPlaneBubbleCoverage(const InputParameters & parameters) :
    ElementAverageValue(parameters),
    _GB_lengthy(getParam<Real>("GB_lengthy")),
    _GB_lengthz(getParam<Real>("GB_lengthz")),
    _Lx(getParam<Real>("Lx")),
    _tol(getParam<Real>("tol")),
    _dim(getParam<unsigned int>("dim")),
    _bub_threshold(getParam<Real>("bub_threshold"))
{
}

// / \todo Does this even work? Is x ever exactly == _Lx/2 at any qp?
Real MidPlaneBubbleCoverage::computeQpIntegral()
{
  Real x = _q_point[_qp](0);

  if (_u[_qp] > _bub_threshold && x - _tol <= _Lx/2.0 && x + _tol >= _Lx/2.0)
    switch (_dim)
    {
      case 2:
        return 1.0/_GB_lengthy;
      case 3:
        return 1.0/_GB_lengthy/_GB_lengthz;
    }

  return 0.0;
}
