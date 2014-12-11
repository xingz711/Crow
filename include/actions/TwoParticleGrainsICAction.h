#ifndef TWOPARTICLEGRAINSICACTION_H
#define TWOPARTICLEGRAINSICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class TwoParticleGrainsICAction: public Action
{
public:
  TwoParticleGrainsICAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _op_num;

};

template<>
InputParameters validParams<TwoParticleGrainsICAction>();

#endif //TWOPARTICLEGRAINSICACTION_H
