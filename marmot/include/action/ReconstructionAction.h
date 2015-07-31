#ifndef RECONSTRUCTIONACTION_H
#define RECONSTRUCTIONACTION_H

#include "InputParameters.h"
#include "Action.h"

//Forward Declarations
class ReconstructionAction;

template<>
InputParameters validParams<ReconstructionAction>();

class ReconstructionAction: public Action
{
public:
  ReconstructionAction(const InputParameters & params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  unsigned int _op_num;
  Real _scaling;
  std::string _var_name_base, _eta, _grn, _phase, _rgb;
  std::string _EBSD_file_name;
  std::string _variable_to_read;
  unsigned int _sd;

  Real _x1, _y1, _z1;
  Real _x2, _y2, _z2;
};

#endif //RECONSTRUCTIONACTION_H
