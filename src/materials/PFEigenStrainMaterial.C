#include "PFEigenStrainMaterial.h"
//#include "AddV.h"
template<>
InputParameters validParams<PFEigenStrainMaterial>()
{
  InputParameters params = validParams<PFEigenStrainBaseMaterial>();
  params.addRequiredParam<Real>("epsilon0", "Initial eigen strain value");
  params.addParam<Real>("c0", 0.0, "Initial concentration value");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addCoupledVar("v", "order parameters");
  params.addParam<std::vector<Real> >("e_v", "lattice mismatch co-eff");

  return params;
}

PFEigenStrainMaterial::PFEigenStrainMaterial(const InputParameters & parameters) :
     PFEigenStrainBaseMaterial(parameters),
    _epsilon0(getParam<Real>("epsilon0")),
    _c0(getParam<Real>("c0")),
    _e_v(getParam<std::vector<Real> >("e_v"))
    
{
}

void PFEigenStrainMaterial::computeEigenStrain()
{
  Real sumeta = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
      sumeta += _e_v[i]*(*_vals[i])[_qp]*(*_vals[i])[_qp]; 
  
    _eigenstrain[_qp].addIa(_epsilon0 * (_c[_qp] - _c0) + sumeta);

  // first derivative w.r.t. v.  true dat!!!!
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
   (*_delastic_strain_dv[i])[_qp].zero();
   (*_delastic_strain_dv[i])[_qp].addIa(- 2.0*_e_v[i]*(*_vals[i])[_qp]);

  // second derivative w.r.t. v
   for (unsigned int j=0; j < _ncrys; ++j)
   {
    (*_d2elastic_strain_dv2[i][j])[_qp].zero();
    (*_d2elastic_strain_dv2[i][j])[_qp].addIa(- 2.0*_e_v[j]);
   }
  }
  // first derivative w.r.t. c
  _delastic_strain_dc[_qp].zero();
  _delastic_strain_dc[_qp].addIa(-_epsilon0); // delastic_strain/dc = -deigenstrain/dc

  // second derivative w.r.t. c (vanishes)
  _d2elastic_strain_dc2[_qp].zero();
}

void PFEigenStrainMaterial::computeQpElasticityTensor()
{
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp] = _Cijkl;

  // the elasticity tensor is independent of v
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
   (*_delasticity_tensor_dv[i])[_qp].zero();
   //(*_d2elasticity_tensor_dv2[i])[_qp].zero();
   
    for (unsigned int j=0; j < _ncrys; ++j)
     (*_d2elasticity_tensor_dv2[i][j])[_qp].zero();
   }
   // the elasticity tensor is independent of c
  _delasticity_tensor_dc[_qp].zero();
  _d2elasticity_tensor_dc2[_qp].zero();
}
