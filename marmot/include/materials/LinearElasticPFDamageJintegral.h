#ifndef LINEARELASTICPFDAMAGEJINTEGRAL_H
#define LINEARELASTICPFDAMAGEJINTEGRAL_H

#include "LinearElasticPFDamage.h"

class LinearElasticPFDamageJintegral : public LinearElasticPFDamage
{

 public:
  LinearElasticPFDamageJintegral(const InputParameters & parameters);

 protected:
  virtual void computeEshelbytensor();
  virtual void computeQpStress();

  MaterialProperty<ColumnMajorMatrix> & _Eshelby_tensor;
  MaterialProperty<RankTwoTensor> & _Eshelby_tensor_out;
};

#endif //LINEARELASTICPFDAMAGEJINTEGRAL_H
