#include "GBPosition.h"
#include "MooseMesh.h"
#include "SubProblem.h"

template<>
InputParameters validParams<GBPosition>()
{
  InputParameters params = validParams<NodalProxyMaxValue>();
  params.addRequiredParam<unsigned int>("direction_index", "The index of the direction the position is measured in");
  return params;
}

GBPosition::GBPosition(const InputParameters & parameters) :
    NodalProxyMaxValue(parameters),
    _direction_index(parameters.get<unsigned int>("direction_index")),
    _mesh(_subproblem.mesh())
{
}

Real
GBPosition::computeValue()
{
  Real val = 1.0 - _u[_qp];
  return val;
}

Real
GBPosition::getValue()
{
  unsigned int node_id = NodalProxyMaxValue::getValue();
  return _mesh.node(node_id)(_direction_index);
}
