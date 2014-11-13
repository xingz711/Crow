#include "ACBulk.h"

#ifndef ACParticleGrowth_H
#define ACParticleGrowth_H

//Forward Declarations
class ACParticleGrowth;

template<>
InputParameters validParams<ACParticleGrowth>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class ACParticleGrowth : public ACBulk
{
public:
  ACParticleGrowth(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  unsigned int _ncrys;
  
  VariableValue & _c;
  MaterialProperty<Real> & _L;
  
};

#endif //ACParticleGrowth_H
