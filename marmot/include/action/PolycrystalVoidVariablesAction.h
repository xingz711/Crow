#ifndef POLYCRYSTALVOIDVARIABLESACTION_H
#define POLYCRYSTALVOIDVARIABLESACTION_H

#include "InputParameters.h"
#include "Action.h"

//Forward Declarations
class PolycrystalVoidVariablesAction;

template<>
InputParameters validParams<PolycrystalVoidVariablesAction>();

class PolycrystalVoidVariablesAction: public Action
{
public:
  PolycrystalVoidVariablesAction(const InputParameters & params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  unsigned int _op_num;
  // unsigned int _void_num;
  std::string _var_name_base;

};

#endif //POLYCRYSTALVOIDVARIABLESACTION_H
