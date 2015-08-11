/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SinteringFreeEnergy.h"

template<>
InputParameters validParams<SinteringFreeEnergy>()
{
  InputParameters params = validParams<DerivativeFunctionMaterialBase>();
  params.addClassDescription("Material that implements the math free energy and its derivatives: \nF = 1/4(1 + c)^2*(1 - c)^2");
  params.addRequiredCoupledVar("c","Concentration variable");
  params.addCoupledVar("v","order paarameters");
  //params.addParam<Real>("kbT",0.0, "Effect of temperature");
  //params.addParam<Real>("tol",1e-6 "Effect of temperature");
  return params;
}

SinteringFreeEnergy::SinteringFreeEnergy(const InputParameters & parameters) :
    DerivativeFunctionMaterialBase(parameters),
    _c(coupledValue("c")),
    _c_var(coupled("c"))
   // _kbT(getParam<Real>("kbT")),
    //_tol(getParam<Real>("tol")
{
  // Array of coupled variables is created in the constructor
  _ncrys = coupledComponents("v"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
  _vals.resize(_ncrys); //Size variable arrays
  _vals_var.resize(_ncrys);
  
  //Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

Real
SinteringFreeEnergy::computeF()
{
 // if (_tol[i] > 0.0)
   //  {
     //   a = _c[_qp];
      //  _func_params[i] = a < _tol[i] ? _tol[i] : (a > 1.0 - _tol[i] ? 1.0 - _tol[i] : a);
     // }
  
  Real SumEtaj = 0.0;
  Real SumEtaj3 = 0.0;
    for (unsigned int i = 0; i < _ncrys; ++i)
    {
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters
      SumEtaj3 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[i])[_qp];
    }
  return 16.0*_c[_qp]*_c[_qp]*(1.0 - _c[_qp])*(1.0 - _c[_qp]) + _c[_qp]*_c[_qp] + 6.0*(1.0 - _c[_qp])*SumEtaj - 4.0*(2.0 - _c[_qp])*SumEtaj3 + 3*SumEtaj*SumEtaj;// + _kbT*(_c[_qp]*log(_c[_qp])+ (1.0 - _c[_qp])*log(1.0 - _c[_qp]));
}

Real
SinteringFreeEnergy::computeDF(unsigned int j_var)
{
    Real SumEtaj = 0.0;
    Real SumEtaj3 = 0.0;
  if (j_var == _c_var)//Note that these checks are only really necessary when the material has more than one coupled variable
  {
    for (unsigned int i = 0; i < _ncrys; ++i)
    {
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters
      SumEtaj3 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[i])[_qp];
    }
    return 64.0*_c[_qp]*_c[_qp]* _c[_qp]- 96.0*_c[_qp]*_c[_qp] + 34.0*_c[_qp] - 6.0*SumEtaj + 4.0*SumEtaj3; // + _kbT*(log(_c[_qp]) - log(1.0 - _c[_qp]));
  }
  else
    return 0.0;
  
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if (j_var == _vals_var[i])
    {
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters
      //sSumEtaj3 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[i])[_qp];
      return 6.0*(1.0 - _c[_qp])*(*_vals[i])[_qp] - 8.0*(2.0 - _c[_qp])*SumEtaj + 12.0*SumEtaj*(*_vals[i])[_qp]; 
    }
    else
      return 0.0; 
  }
}

Real
SinteringFreeEnergy::computeD2F(unsigned int j_var, unsigned int k_var)
{
  if ( (j_var == _c_var) && (k_var == _c_var) )
    return 192.0*_c[_qp]*_c[_qp] - 192.0*_c[_qp] + 34.0; // + _kbT*(1/_c[_qp] + 1.0/ (1.0 - _c[_qp])); 
  else
    return 0.0;
  
  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if ( (j_var == _c_var) && (k_var == _vals_var[i]))
    {
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters
      //SumEtaj3 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[i])[_qp];
      return - 6.0*(*_vals[i])[_qp] + 12.0*SumEtaj; 
    }
    else
      return 0.0;  

  //for (unsigned int i = 0; i < _ncrys; ++i)
    if ((j_var == _vals_var[i]) && (k_var == _vals_var[i]))
    {
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters
      //SumEtaj3 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[i])[_qp];
      return 6.0*(1.0 - _c[_qp]) - 16.0*(2.0 - _c[_qp])*(*_vals[i])[_qp] + 12.0*SumEtaj + 24.0*(*_vals[i])[_qp]*(*_vals[i])[_qp]; 
    }
    else
      return 0.0;  
  }
}

Real
SinteringFreeEnergy::computeD3F(unsigned int j_var, unsigned int k_var, unsigned int l_var)
{
  if ((j_var == _c_var) && (k_var == _c_var) && (l_var == _c_var))
    return - 192.0 + 384.0* _c[_qp];// - _kbT*(1/(_c[_qp]*_c[_qp]) - 1.0/ ((1.0 - _c[_qp])*(1.0 - _c[_qp])));
  else
    return 0.0;
}
