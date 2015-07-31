#ifndef CHRADSINK_H
#define CHRADSINK_H

#include "CHBulk.h"
#include "Material.h"

//Forward Declarations
class CHRadSink;

template<>
InputParameters validParams<CHRadSink>();

class CHRadSink : public CHBulk
{
public:
  CHRadSink(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

  // VariableValue & _Phi_i;
  // VariableGradient & _grad_Phi_i;
  VariableValue & _u_old;

  const MaterialProperty<Real> & _C_i;
  const MaterialProperty<Real> & _m_i;
  const MaterialProperty<Real> & _b_i;
  // MaterialProperty<Real> & _M;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  unsigned int _n;
};

#endif //CHRADSINK_H
