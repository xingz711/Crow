#ifndef GBPOSITION_H
#define GBPOSITION_H

#include "NodalProxyMaxValue.h"

//Forward Declarations
class GBPosition;
class MooseMesh;

template<>
InputParameters validParams<GBPosition>();

class GBPosition : public NodalProxyMaxValue
{
public:
  GBPosition(const InputParameters & parameters);

  virtual Real getValue();

protected:
  virtual Real computeValue();

  unsigned int _direction_index;
  MooseMesh & _mesh;
};

#endif //GBPOSITION_H
