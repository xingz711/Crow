#include "EigenStrainFracMaterial.h"

template<>
InputParameters validParams<EigenStrainFracMaterial>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  return params;
}

EigenStrainFracMaterial::EigenStrainFracMaterial(const InputParameters & parameters) :
    EigenStrainBaseMaterial(parameters),
    _elasticity_tensor0(declareProperty<RankFourTensor>("elasticity_tensor0")),
    _G(declareProperty<Real>("G")),
    _G0(declareProperty<Real>("G0"))
{
}

void EigenStrainFracMaterial::computeQpElasticityTensor()
{
  ElasticityTensorR4 Cijkl_var(_Cijkl);
  Real c=_c[_qp];
  Real fac1, fac2, fac3;
  // Real xk=1e-8;

  fac1 = 1.0 - 10.0 * std::pow(c, 3.0) + 15.0 * std::pow(c, 4.0) - 6.0 * std::pow(c, 5.0);
  fac2 = -30.0 * std::pow(c, 2.0) + 60.0 * std::pow(c, 3.0) - 30.0 * std::pow(c, 4.0);
  fac3 = -60.0 * std::pow(c, 1.0) + 180.0 * std::pow(c, 2.0) - 120.0 * std::pow(c, 3.0);

  _elasticity_tensor0[_qp] = _Cijkl;

  _elasticity_tensor[_qp] = Cijkl_var;
  _elasticity_tensor[_qp] *= fac1;

  _delasticity_tensor_dc[_qp] = Cijkl_var;
  _delasticity_tensor_dc[_qp] *= fac2;

  _d2elasticity_tensor_dc2[_qp] = Cijkl_var;
  _d2elasticity_tensor_dc2[_qp] *= fac3;

  _Jacobian_mult[_qp] = _elasticity_tensor[_qp];

  _G[_qp] = 0.5 * _elastic_strain[_qp].doubleContraction(_elasticity_tensor[_qp] * _elastic_strain[_qp]);
  _G0[_qp] = 0.5 * _elastic_strain[_qp].doubleContraction(_elasticity_tensor0[_qp] * _elastic_strain[_qp]);
}

void EigenStrainFracMaterial::computeEigenStrain()
{
  _eigenstrain[_qp].zero();

  _delastic_strain_dc[_qp].zero();
  _d2elastic_strain_dc2[_qp].zero();

  _G[_qp] = 0.5 * _elastic_strain[_qp].doubleContraction(_elasticity_tensor[_qp] * _elastic_strain[_qp]);
  _G0[_qp] = 0.5 * _elastic_strain[_qp].doubleContraction(_elasticity_tensor0[_qp] * _elastic_strain[_qp]);
}
