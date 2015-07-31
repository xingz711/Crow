#ifndef CHRADPENALTYBC_H
#define CHRADPENALTYBC_H

#include "IntegratedBC.h"

//Forward Declarations
class CHRadPenaltyBC;

template<>
InputParameters validParams<CHRadPenaltyBC>();

class CHRadPenaltyBC : public IntegratedBC
{
public:
  CHRadPenaltyBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real _M;
};

#endif //CHRADPENALTYBC_H
