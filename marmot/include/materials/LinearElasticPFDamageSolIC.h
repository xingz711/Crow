#ifndef LINEARELASTICPFDAMAGESOLIC_H
#define LINEARELASTICPFDAMAGESOLIC_H

#include "LinearElasticPFDamage.h"

class LinearElasticPFDamageSolIC : public LinearElasticPFDamage
{
public:
  LinearElasticPFDamageSolIC(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() {};
  virtual void computeQpStress();
  virtual void initProperty();

  bool _has_conc;
  VariableValue & _conc;
  Real _c_void_tol;
  Real _kvoid;

private:
};


#endif //LINEARELASTICPFDAMAGESOLIC_H
