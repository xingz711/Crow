#ifndef CHGRBU_H
#define CHGRBU_H

#include "SplitCHCRes.h"

//Forward Declarations
class CHGrBu;

template<>
InputParameters validParams<CHGrBu>();

class CHGrBu : public SplitCHCRes
{
public:
  CHGrBu(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

  const MaterialProperty<Real> & _Ao;
  const MaterialProperty<Real> & _BetaS;

  unsigned int _nvals;
  std::vector<VariableValue *> _vals;
};

#endif //CHGRBU_H
