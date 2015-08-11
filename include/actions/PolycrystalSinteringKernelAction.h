#ifndef POLYCRYSTALSINTERINGKERNELACTION_H
#define POLYCRYSTALSINTERINGKERNELACTION_H

#include "Action.h"

class PolycrystalSinteringKernelAction: public Action
{
public:
  PolycrystalSinteringKernelAction(InputParameters params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  VariableName _c;
  bool _implicit;
  VariableName _T;
  
  std::vector<VariableName > _vals;
  //std::vector<unsigned int> _vals_var;

  unsigned int _ncrys;
};

template<>
InputParameters validParams<PolycrystalSinteringKernelAction>();

#endif //POLYCRYSTALKERNELACTION_H
