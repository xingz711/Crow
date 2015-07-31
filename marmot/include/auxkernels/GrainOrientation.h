#ifndef GRAINORIENTATION_H
#define GRAINORIENTATION_H

#include "AuxKernel.h"

//Forward Declarations
class GrainOrientation;

template<>
InputParameters validParams<GrainOrientation>();

class GrainOrientation : public AuxKernel
{
public:
  GrainOrientation(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:
  bool _output_angles;
  std::string _angle_file_name;
  std::vector<VariableValue *> _vals;
  std::vector<Point> _Euler_Angles;
};

#endif //GRAINORIENTATION_H
