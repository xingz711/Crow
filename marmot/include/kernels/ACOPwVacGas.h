#ifndef ACOPWVACGAS_H
#define ACOPWVACGAS_H

#include "ACBulk.h"

//Forward Declarations
class ACOPwVacGas;

template<>
InputParameters validParams<ACOPwVacGas>();

class ACOPwVacGas : public ACBulk
{
public:
  ACOPwVacGas(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

private:
  VariableValue & _cv;
  VariableValue & _cg;

  const MaterialProperty<Real> & _Efv;
  const MaterialProperty<Real> & _Efg;
  const MaterialProperty<Real> & _RT;

  std::string _W_name;
  const MaterialProperty<Real> & _W;
};

#endif //ACOPWVACGAS_H
