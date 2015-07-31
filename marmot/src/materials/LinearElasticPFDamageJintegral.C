#include "LinearElasticPFDamageJintegral.h"

template<>
InputParameters validParams<LinearElasticPFDamageJintegral>()
{
  InputParameters params = validParams<LinearElasticPFDamage>();
  return params;
}

LinearElasticPFDamageJintegral::LinearElasticPFDamageJintegral(const InputParameters & parameters) :
    LinearElasticPFDamage(parameters),
    _Eshelby_tensor(declareProperty<ColumnMajorMatrix>("Eshelby_tensor")),
    _Eshelby_tensor_out(declareProperty<RankTwoTensor>("Eshelby_tensor_out"))
{
}

void LinearElasticPFDamageJintegral::computeEshelbytensor()
{
  RankTwoTensor F(_grad_disp_x[_qp], _grad_disp_y[_qp], _grad_disp_z[_qp]);
  // RankTwoTensor Grad_u=F;
  F.addIa(1.0);
  // RankTwoTensor Finv=F.inverse();
  // RankTwoTensor grad_u=Grad_u*Finv;

  Real W = _stress[_qp].doubleContraction(_elastic_strain[_qp]) * 0.5;

  RankTwoTensor WI;
  WI.addIa(W);
  // WI *= F.det();

  RankTwoTensor Eshelby_tensor;
  // Eshelby_tensor = _stress[_qp] * grad_u-WI;
  Eshelby_tensor = WI-(F.transpose()) * _stress[_qp] * ((F.inverse()).transpose()) * F.det();

  for (unsigned int i = 0; i < 3; ++i)
    for (unsigned int j = 0; j < 3; ++j)
      _Eshelby_tensor[_qp](i,j)=Eshelby_tensor(i,j);

  _Eshelby_tensor_out[_qp]=Eshelby_tensor;
}

void LinearElasticPFDamageJintegral::computeQpStress()
{
  _stress[_qp].zero();
  update_var(_elastic_strain[_qp]);
  computeEshelbytensor();
}
