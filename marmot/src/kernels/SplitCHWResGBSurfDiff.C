#include "SplitCHWResGBSurfDiff.h"

#include "Material.h"

template<>
InputParameters validParams<SplitCHWResGBSurfDiff>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<Real>("diffindex", 0.0, "Index for surface diffusion");
  params.addParam<Real>("gbindex", 0.0, "Index for GB diffusion");
  params.addParam<Real>("bulkindex", 1.0, "Index for bulk diffusion");
  params.addParam<MaterialPropertyName>("mob_name", "mobtemp","The mobility used with the kernel");
  params.addRequiredCoupledVar("c","intermediate parameter--concentration");
  return params;
}

SplitCHWResGBSurfDiff::SplitCHWResGBSurfDiff(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _diffindex(getParam<Real>("diffindex")),
    _gbindex(getParam<Real>("gbindex")),
    _bulkindex(getParam<Real>("bulkindex")),
    _c_var(coupled("c")),
    _c(coupledValue("c")),
    _mobtemp(getMaterialProperty<Real>(_mob_name)),
    _ncrys(coupledComponents("v"))
{
  _vals.resize(_ncrys);

  for (unsigned int i=0; i<_ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
}

// / take into account GB diffusivity: (Acta mata 54,2006, 953-961)
Real
SplitCHWResGBSurfDiff::computeQpResidual()
{
  Real grnprod = 0.0;
  for (unsigned int i=0; i<_ncrys; ++i)
    for (unsigned int j=i+1; j<_ncrys; ++j)
      grnprod += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

  Real c = _c[_qp];
  if (c > 1.0)
    c = 1.0;
  else if (c < 0.0)
    c = 0.0;

  Real mob_fctr = _diffindex*(std::exp(4.0*c*(1.0 - c)) - 1.0) + 16.0*_gbindex*grnprod + _bulkindex;
  return mob_fctr*_mobtemp[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
}

// / take into account GB diffusivity: (Acta mata 54,2006, 953-961)
Real
SplitCHWResGBSurfDiff::computeQpJacobian()
{
  Real grnprod = 0.0;
  for (unsigned int i=0; i<_ncrys; ++i)
    for (unsigned int j=i+1; j<_ncrys; ++j)
      grnprod += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

  Real mob_fctr = _diffindex*(std::exp(4.0*_c[_qp]*(1.0-_c[_qp])) - 1.0) + 16.0*_gbindex*grnprod + _bulkindex;
  return mob_fctr*_mobtemp[_qp]*_grad_phi[_j][_qp]*_grad_test[_i][_qp];;
}

Real
SplitCHWResGBSurfDiff::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
  {
    Real dmob_fctr = 4.0*(1.0 - 2.0*_c[_qp])*_diffindex * std::exp(4.0*_c[_qp]*(1.0-_c[_qp])) *_phi[_j][_qp];
    return dmob_fctr*_mobtemp[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];
  }

  return 0.0;
}
