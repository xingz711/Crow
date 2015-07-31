#include "SplitCHWResTGrad.h"

#include "Material.h"

template<>
InputParameters validParams<SplitCHWResTGrad>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<Real>("diffindex", 0.0, "Index for surface diffusion");
  params.addParam<Real>("gbindex", 0.0, "Index for GB diffusion");
  params.addParam<Real>("bulkindex", 1.0, "Index for bulk diffusion");
  params.addRequiredParam<MaterialPropertyName>("diff_name", "The diffusivity used with the kernel");
  params.addParam<MaterialPropertyName>("Q_name", "Qheat", "The material name for the heat of transport");
  params.addRequiredCoupledVar("c", "intermediate parameter--concentration");
  params.addRequiredCoupledVar("T", "intermediate parameter--temperature");
  return params;
}

SplitCHWResTGrad::SplitCHWResTGrad(const InputParameters & parameters) :
    Kernel(parameters),
    _diffindex(getParam<Real>("diffindex")),
    _gbindex(getParam<Real>("gbindex")),
    _bulkindex(getParam<Real>("bulkindex")),
    _tmpt_var(coupled("T")),
    _tmpt(coupledValue("T")),
    _grad_tmpt(coupledGradient("T")),
    _c_var(coupled("c")),
    _c(coupledValue("c")),
    _D_v(getMaterialProperty<Real>("diff_name")),
    _Qheat(getMaterialProperty<Real>("Q_name")),
    _ncrys(coupledComponents("v")),
    _kb(8.617343e-5) // Boltzmann constant in eV/K
{
  _vals.resize(_ncrys);
  for (unsigned int i=0; i<_ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
}

Real
SplitCHWResTGrad::computeQpResidual()
{
  // ******************take into account GB diffusivity: (Acta mata 54,2006, 953-961)*************
  Real grnprod = 0.0;
  for (unsigned int i=0; i<_ncrys; ++i)
    for (unsigned int j=i+1; j<_ncrys; ++j)
      grnprod += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

  Real mob_fctr = _diffindex*(std::exp(4.0*_c[_qp]*(1.0-_c[_qp])) - 1.0) + 16.0*_gbindex*grnprod + _bulkindex;

  Real T_term = mob_fctr*_D_v[_qp]*_Qheat[_qp]*_c[_qp]/(_kb*_tmpt[_qp]*_tmpt[_qp]);
  return T_term*_grad_tmpt[_qp]*_grad_test[_i][_qp];
}

Real
SplitCHWResTGrad::computeQpJacobian()
{
  return 0.0;
}

Real
SplitCHWResTGrad::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
  {
    // ******************take into account GB diffusivity: (Acta mata 54,2006, 953-961)*************
    Real grnprod = 0.0;
    for (unsigned int i=0; i<_ncrys; ++i)
      for (unsigned int j=i+1; j<_ncrys; ++j)
        grnprod += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

    Real mob_fctr = _diffindex*(std::exp(4.0*_c[_qp]*(1.0-_c[_qp])) - 1.0) + 16.0*_gbindex*grnprod + _bulkindex;
    Real dmob_fctr = 4.0*(1.0 - 2.0*_c[_qp])*_diffindex * std::exp(4.0*_c[_qp]*(1.0-_c[_qp])) *_phi[_j][_qp];

    Real dT_term = dmob_fctr*_D_v[_qp]*_Qheat[_qp]*_c[_qp]/(_kb*_tmpt[_qp]*_tmpt[_qp]) + mob_fctr*_D_v[_qp]*_Qheat[_qp]*_phi[_j][_qp]/(_kb*_tmpt[_qp]*_tmpt[_qp]);
    return dT_term*_grad_tmpt[_qp]*_grad_test[_i][_qp];
  }

  return 0.0;
}
