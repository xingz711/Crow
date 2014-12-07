// Original class author: A.M. Jokisaari, O. Heinonen

#include "PFElasticMaterial.h"

/**
 * LinearElasticMaterial handles a fully anisotropic, single-crystal material's elastic
 * constants.  It takes all 21 independent stiffness tensor inputs.  This can be extended or
 * simplified to specify HCP, monoclinic, cubic, etc as needed.
 */

template<>
InputParameters validParams<PFElasticMaterial>()
{
  InputParameters params = validParams<TensorMechanicsMaterial>();
  params.addParam<Real>("thermal_expansion_coeff", 0, "Thermal expansion coefficient in 1/K");
  params.addParam<Real>("T0", 300, "Reference temperature for thermal expansion in K");
  params.addCoupledVar("T", 300, "Temperature in Kelvin");
  params.addCoupledVar("c",0, "concentration field");
  params.addCoupledVar("v", 0, "sum of order parameters");
  params.addParam<std::vector<Real> >("e_c", "lattice expansion co-efficient related to composition");
  params.addParam<std::vector<Real> >("e_v","co-eff for lattice mismatch betn parent & daughter phase");
  params.addParam<std::vector<Real> >("applied_strain_vector", "Applied strain: e11, e22, e33, e23, e13, e12");
  //params.addParam<std::vector<Real> >("applied_stress_vector", "Applied strain: s11, s22, s33, s23, s13, s12");
  return params;
}

PFElasticMaterial::PFElasticMaterial(const std::string & name,
                                             InputParameters parameters) :
    TensorMechanicsMaterial(name, parameters),
    _delasticity_tensor_dc(declareProperty<ElasticityTensorR4>("delasticity_tensor_dc")),
    _d2elasticity_tensor_dc2(declareProperty<ElasticityTensorR4>("d2elasticity_tensor_dc2")),
    _T(coupledValue("T")),
    _thermal_expansion_coeff(getParam<Real>("thermal_expansion_coeff")),
    _T0(getParam<Real>("T0")),
    _applied_strain_vector(getParam<std::vector<Real> >("applied_strain_vector")),
    
    _c(coupledValue("c")),
    _v(coupledValue("v")),
    _e_c(getParam<std::vector<Real> >("e_c")),
    _e_v(getParam<std::vector<Real> >("e_v"))
{
 // RankTwoTensor stress_free_strain;
  //Initialize applied strain tensor from input vector
  if (_applied_strain_vector.size() == 6)
    _applied_strain_tensor.fillFromInputVector(_applied_strain_vector);
  else
    _applied_strain_tensor.zero();
}

RankTwoTensor
PFElasticMaterial::computeStressFreeStrain()
{
  //Apply thermal expansion
  RankTwoTensor stress_free_strain;
  stress_free_strain.addIa(-_thermal_expansion_coeff*(_T[_qp] - _T0));

  //Apply uniform applied strain
  if (_applied_strain_vector.size() == 6)
    stress_free_strain += _applied_strain_tensor;
  
  //considering effect of phase field variables
  stress_free_strain += (_e_c*_c[_qp] + _e_v * _v[_qp]);

  return stress_free_strain;
}

void
PFElasticMaterial::computeQpStrain()
{
  //strain = (grad_disp + grad_disp^T)/2
  RankTwoTensor grad_tensor(_grad_disp_x[_qp],_grad_disp_y[_qp],_grad_disp_z[_qp]);

  if (_t_step > 1000000)
  {
    RankTwoTensor test = grad_tensor;
    test.addIa(1.0);

    RankTwoTensor eye = test*test.inverse();
    eye.print();
  }
  RankTwoTensor stress_free_strain = computeStressFreeStrain();
  
  _total_strain[_qp] = (grad_tensor + grad_tensor.transpose())/2.0;
  _elastic_strain[_qp] = _total_strain[_qp] - stress_free_strain;
}

void
PFElasticMaterial::computeQpStress()
{
  //Calculation and Apply stress free strain
  RankTwoTensor stress_free_strain = computeStressFreeStrain();

  //_elastic_strain[_qp] += stress_free_strain;
  //_total_strain[_qp] = _elastic_strain[_qp];

  // stress = C * e
  _stress[_qp] = _elasticity_tensor[_qp]*_elastic_strain[_qp];
}


