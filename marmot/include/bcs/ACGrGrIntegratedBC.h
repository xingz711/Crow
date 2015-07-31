#ifndef ACGRGRINTEGRATEDBC_H
#define ACGRGRINTEGRATEDBC_H

#include "IntegratedBC.h"

//Forward Declarations
class ACGrGrIntegratedBC;

template<>
InputParameters validParams<ACGrGrIntegratedBC>();

class ACGrGrIntegratedBC : public IntegratedBC
{
public:
  ACGrGrIntegratedBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Real> & _L;
  const MaterialProperty<Real> & _kappa;
};

#endif //ACGRGRINTEGRATEDBC_H
