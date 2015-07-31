#ifndef LINEARELASTICPFDAMAGEPOROSITY_H
#define LINEARELASTICPFDAMAGEPOROSITY_H

#include "LinearElasticPFDamageSolIC.h"

class LinearElasticPFDamagePorosity : public LinearElasticPFDamageSolIC
{
 public:
  LinearElasticPFDamagePorosity(const InputParameters & parameters);

 protected:
  virtual void initProperty();

  bool _has_porosity;
  VariableValue & _porosity;

 private:
};

#endif //LINEARELASTICPFDAMAGEPOROSITY_H
