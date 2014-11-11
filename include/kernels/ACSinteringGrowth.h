#include "ACBulk.h"

#ifndef ACSinteringGrowth_H
#define ACSinteringGrowth_H

//Forward Declarations
class ACSinteringGrowth;

template<>
InputParameters validParams<ACSinteringGrowth>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class ACSinteringGrowth : public ACBulk
{
public:
  ACSinteringGrowth(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

 // MaterialProperty<Real> & _mu;
//  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _tgrad_corr_mult;
  
 
  bool _has_T;
  VariableGradient * _grad_T;

  unsigned int _ncrys;
  // Real _gamma;

  VariableValue & _rho;
};

#endif //ACSinteringGrowth_H
