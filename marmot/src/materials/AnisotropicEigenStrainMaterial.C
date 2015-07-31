#include "AnisotropicEigenStrainMaterial.h"
#include "libmesh/vector_value.h"

template<>
InputParameters validParams<AnisotropicEigenStrainMaterial>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  params.addRangeCheckedParam<std::vector<Real> >("eigenstrain_coef", "eigenstrain_coef_size = 3", "Vector of anisotropic eigenstrain values in all 3 directions");
  params.addRequiredParam<Real>("factor", "coefficient of elastisity tensor dependency on concentration");
  params.addParam<Real>("length_scale", 1.0e-9, "Dimension of length in meters, default nm");
  params.addParam<Real>("pressure_scale", 1.0e6, "Pressure scale in Pa, default Mpa");
  params.addParam<bool>("inhomogeneity", false, "whether inhomogeneity presents");
  return params;
}

AnisotropicEigenStrainMaterial::AnisotropicEigenStrainMaterial(const InputParameters & parameters) :
    EigenStrainBaseMaterial(parameters),
    _length_scale(getParam<Real>("length_scale")),
    _pressure_scale(getParam<Real>("pressure_scale")),
    _eigenstrain_coef(getParam<std::vector<Real> >("eigenstrain_coef")),
    _factor(getParam<Real>("factor")),
    _inhomogeneity(getParam<bool>("inhomogeneity"))
{
}

void
AnisotropicEigenStrainMaterial::computeEigenStrain()
{
  RankTwoTensor init_eigenstrain, init_deigenstrain_dc;

  Real c = _c[_qp];
  if (c > 1.0) c = 1.0;
  if (c < 0.0) c = 0.0;

  for (unsigned int i = 0; i < 3; ++i)
  {
    for (unsigned int j = 0; j < 3; ++j)
    {
      if (i == j)
      {
        init_eigenstrain(i,i) = _eigenstrain_coef[i] * c * (1.0 - c);
        init_deigenstrain_dc(i,i) = _eigenstrain_coef[i] * (1.0 - 2.0*c);
      }
    }
  }

  _eigenstrain[_qp] = init_eigenstrain;

  _delastic_strain_dc[_qp] = -init_deigenstrain_dc;
  _d2elastic_strain_dc2[_qp].zero();
}

void AnisotropicEigenStrainMaterial::computeQpElasticityTensor()
{
  ElasticityTensorR4 Cijkl_0(_Cijkl), Cijkl_1(_Cijkl);
  ElasticityTensorR4 init_delasticity_tensor_dc;

  const Real JtoeV = 6.24150974e18; // joule to eV conversion

  // Convert from XPa to eV/(xm)^3, where X is pressure scale and x is length scale;
  Cijkl_0 *= JtoeV * (_length_scale*_length_scale*_length_scale) * _pressure_scale;
  Cijkl_1 *= JtoeV * (_length_scale*_length_scale*_length_scale) * _pressure_scale;

  if (!_inhomogeneity)  // / Concentration-independent/homogeneous linear elasiticity tensor
  {
    _elasticity_tensor[_qp] = Cijkl_0;
    _Jacobian_mult[_qp] = Cijkl_0;

    _delasticity_tensor_dc[_qp].zero();
    _d2elasticity_tensor_dc2[_qp].zero();
  }
  else  // / Concentration-dependent linear elasiticity tensor - Cijkl_1 = _Cijkl*(1.0-_factor*_c[_qp])
  {
    Real delta_c;
    Real c = _c[_qp];
    if (c < 0.0)
      c = 0.0;

    delta_c = 1.0 - _factor*c;
    if (delta_c < 1e-5)
      delta_c = 1e-5;

    Cijkl_1 *= delta_c;

    _elasticity_tensor[_qp] = Cijkl_1;
    _Jacobian_mult[_qp] = Cijkl_1;

    init_delasticity_tensor_dc += -Cijkl_0*_factor;
    _delasticity_tensor_dc[_qp] = init_delasticity_tensor_dc;
    _d2elasticity_tensor_dc2[_qp].zero();
  }
  //  _console << "_delasticity_tensor_dc[_qp](1,1,1,1) = " << _delasticity_tensor_dc[_qp](1,1,1,1) << std::endl;
}
