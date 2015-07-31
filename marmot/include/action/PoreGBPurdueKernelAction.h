#ifndef POREGBPURDUEKERNELACTION_H
#define POREGBPURDUEKERNELACTION_H

#include "Action.h"

//Forward Declarations
class PoreGBPurdueKernelAction;

template<>
InputParameters validParams<PoreGBPurdueKernelAction>();

class PoreGBPurdueKernelAction: public Action
{
public:
  PoreGBPurdueKernelAction(const InputParameters & params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  VariableName _c_name;

  MaterialPropertyName _mob_name;
  MaterialPropertyName _kappa_name;
};

#endif //POREGBPURDUEKERNELACTION_H
