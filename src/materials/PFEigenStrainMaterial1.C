#include "PFEigenStrainMaterial1.h"
//#include "AddV.h"
template<>
InputParameters validParams<PFEigenStrainMaterial1>()
{
  InputParameters params = validParams<SimpleEigenStrainMaterial>();
  params.addCoupledVar("v", "order parameters");
  params.addParam<Real>("e_v", "lattice mismatch co-eff");

  return params;
}

PFEigenStrainMaterial1::PFEigenStrainMaterial1(const std::string & name,
                                                 InputParameters parameters) :
    SimpleEigenStrainMaterial(name, parameters),
    _e_v(getParam<Real>("e_v"))
    
{
  _ncrys = coupledComponents("v");
  _vals.resize(_ncrys);
  _v_name.resize(_ncrys);
  _delastic_strain_dv.resize(_ncrys);
  _d2elastic_strain_dv2.resize(_ncrys);
  _delasticity_tensor_dv.resize(_ncrys);
  _d2elasticity_tensor_dv2.resize(_ncrys);
 
  for (unsigned int i=0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _v_name[i] = getVar("v", i)->name();
        
    // the derivatives of elastic strain w.r.t v are provided here
    _delastic_strain_dv[i] = &declarePropertyDerivative<RankTwoTensor>(_base_name + "elastic_strain", _v_name[i]);
    _delasticity_tensor_dv[i] = &declarePropertyDerivative<ElasticityTensorR4>(_elasticity_tensor_name, _v_name[i]);
           
    _d2elastic_strain_dv2[i].resize(_ncrys);
    _d2elasticity_tensor_dv2[i].resize(_ncrys);
    
    for (unsigned int j=0; j < _ncrys; ++j)      
    {
      _d2elastic_strain_dv2[i][j] = _d2elastic_strain_dv2[j][i] = &declarePropertyDerivative<RankTwoTensor>(_base_name + "elastic_strain", _v_name[i], _v_name[j]);
      _d2elasticity_tensor_dv2[i][j] = &declarePropertyDerivative<ElasticityTensorR4>(_elasticity_tensor_name, _v_name[i], _v_name[j]);
    }
  }
}

void PFEigenStrainMaterial1::computeEigenStrain()
{
  Real sumeta = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
    sumeta += (*_vals[i])[_qp]*(*_vals[i])[_qp]; 
  
  _eigenstrain[_qp].addIa(_e_v*sumeta);

  // first derivative w.r.t. v
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
   (*_delastic_strain_dv[i])[_qp].zero();
   (*_delastic_strain_dv[i])[_qp].addIa(- 2.0*_e_v*(*_vals[i])[_qp]);

  // second derivative w.r.t. v
   for (unsigned int j=0; j < _ncrys; ++j)
   {
    (*_d2elastic_strain_dv2[i][j])[_qp].zero();
    (*_d2elastic_strain_dv2[i][j])[_qp].addIa(- 2.0*_e_v);
   }
  }
}

void PFEigenStrainMaterial1::computeQpElasticityTensor()
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
}