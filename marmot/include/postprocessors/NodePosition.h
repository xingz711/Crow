#ifndef NODEPOSITION_H
#define NODEPOSITION_H

#include "GeneralPostprocessor.h"

//Forward Declarations
class NodePosition;

template<>
InputParameters validParams<NodePosition>();

class NodePosition : public GeneralPostprocessor
{
public:
  NodePosition(const InputParameters & parameters);

  virtual void initialize() {}
  virtual void execute() {}
  virtual Real getValue();

protected:
  const PostprocessorValue & _node_id;
  unsigned int _direction_index;
};

#endif //NODEPOSITION_H
