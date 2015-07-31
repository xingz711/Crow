#ifndef SPLITCHPOREGB_H
#define SPLITCHPOREGB_H

#include "SplitCHCRes.h"

//Forward Declarations
class SplitCHPoreGB;

template<>
InputParameters validParams<SplitCHPoreGB>();

/**
 * CHPoreGB computes the dF/dc term for the Pore/GB interaction model
 */
class SplitCHPoreGB : public SplitCHCRes
{
public:
  SplitCHPoreGB(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

private:
  std::vector<VariableValue *> _vals;
  Real _en_ratio;

  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _gamma;

  unsigned int _ncrys;
};

#endif //SPLITCHPOREGB_H
