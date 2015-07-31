
#ifndef POLYCRYSTALBUBBLEKERNELACTION_H
#define POLYCRYSTALBUBBLEKERNELACTION_H

#include "Action.h"

//Forward Declarations
class PolycrystalBubbleKernelAction;

template<>
InputParameters validParams<PolycrystalBubbleKernelAction>();

class PolycrystalBubbleKernelAction: public Action
{
public:
  PolycrystalBubbleKernelAction(const InputParameters & params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  std::string _cg;

};

#endif //POLYCRYSTALBUBBLEKERNELBLOCK_H
