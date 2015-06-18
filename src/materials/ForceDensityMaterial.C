#include "ForceDensityMaterial.h"

//#include "Material.h"

template<>
InputParameters validParams<ForceDensityMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("etas", "Array of other coupled order parameters");
  params.addCoupledVar("c", "Concentration field");
  params.addParam<Real>("ceq",0.9816, "Equilibrium density");
  params.addParam<Real>("cgb",0.25, "Thresold Concentration for GB");
  params.addParam<Real>("k", 100.0, "stiffness constant");
  return params;
}

ForceDensityMaterial::ForceDensityMaterial(const std::string & name, InputParameters parameters) :
   Material(name,parameters),
   _c(coupledValue("c")),
   _ceq(getParam<Real>("ceq")),
   _cgb(getParam<Real>("cgb")),
   _k(getParam<Real>("k"))
    //_dF(declareProperty<Real>("dF"))

{
  // Array of coupled variables is created in the constructor
  _ncrys = coupledComponents("etas"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
  _vals.resize(_ncrys); //Size variable arrays
  _grad_vals.resize(_ncrys);
  _vals_var.resize(_ncrys);
  _product_etas.resize(_ncrys);
  _diff_grad_etas.resize(_ncrys);
  _dF.resize(_ncrys);
  //_vadv.resize(_ncrys);
  // _gamma = 1.5;

  //Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("etas", i);
    _grad_vals[i] = &coupledGradient("etas", i);
    _vals_var[i] = coupled("etas", i);
    _dF[i]  = &declareProperty<RealGradient>("force_density");
  }
}

void
ForceDensityMaterial::computeQpProperties()
{
  Real product_eta_value = 0.0;

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _product_etas[i] = 0.0;
    _diff_grad_etas[i] = 0.0;
    for (unsigned int j = 0; j < _ncrys; ++j)
    {
      if(i!=j)
      {
        _product_etas[i] += (*_vals[i])[_qp]*(*_vals[j])[_qp]; //Sum all other order parameters
        _diff_grad_etas[i] += ((*_grad_vals[i])[_qp]-(*_grad_vals[j])[_qp]);
      }
   }
    if(_product_etas[i] >= _cgb)
    {
      product_eta_value = 1.0;
    }
    //_diff_grad_etas[i] += _diff_grad_etas[j];
    (*_dF[i])[_qp] = _k*(_c[_qp]-_ceq)*product_eta_value*_diff_grad_etas[i];
  }
}
