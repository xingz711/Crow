#include "ACBulk.h"

#ifndef ACGRBUPOLY_H
#define ACGRBUPOLY_H

//Forward Declarations
class ACGrBuPoly;

template<>
InputParameters validParams<ACGrBuPoly>();

class ACGrBuPoly : public ACBulk
{
public:
  ACGrBuPoly(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  VariableValue & _cg;
  unsigned int _cg_var;

  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  const MaterialProperty<Real> & _Ao;
  const MaterialProperty<Real> & _BetaS;
  const MaterialProperty<Real> & _BetaGB;

  unsigned int _ncrys;
};

#endif //ACGRBUPOLY_H
