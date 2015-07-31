#ifndef BICRYSTALCIRCLEGRAINVOIDICACTION_H
#define BICRYSTALCIRCLEGRAINVOIDICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class BicrystalCircleGrainVoidICAction: public Action
{
public:
  BicrystalCircleGrainVoidICAction(const InputParameters & params);

  virtual void act();

private:

  std::string _var_name_base;
  unsigned int _op_num;
};

template<>
InputParameters validParams<BicrystalCircleGrainVoidICAction>();

#endif //BICRYSTALCIRCLEGRAINVOIDICACTION_H
