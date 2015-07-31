#include "NodePosition.h"
#include "MooseMesh.h"
#include "SubProblem.h"

template<>
InputParameters validParams<NodePosition>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<PostprocessorName>("node_id", "The node_id from which to retrieve our coordinate direction");
  params.addRequiredParam<unsigned int>("direction_index", "The index of the direction the position is measured in");
  return params;
}

NodePosition::NodePosition(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _node_id(getPostprocessorValue("node_id")),
    _direction_index(parameters.get<unsigned int>("direction_index"))
{
}

Real
NodePosition::getValue()
{
  return _subproblem.mesh().node(_node_id)(_direction_index);
}
