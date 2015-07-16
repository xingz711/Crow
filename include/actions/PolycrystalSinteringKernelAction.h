#ifndef POLYCRYSTALSINTERINGKERNELACTION_H
#define POLYCRYSTALSINTERINGKERNELACTION_H

#include "Action.h"

class PolycrystalSinteringKernelAction: public Action
{
public:
  PolycrystalSinteringKernelAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  VariableName _c;

  MaterialPropertyName _mob_name;
  MaterialPropertyName _kappa_name;

};

template<>
InputParameters validParams<PolycrystalSinteringKernelAction>();

#endif //POLYCRYSTALKERNELACTION_H
