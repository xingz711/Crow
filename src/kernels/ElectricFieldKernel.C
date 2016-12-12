#include "ElectricFieldKernel.h"

template<>
InputParameters validParams<ElectricFieldKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("elec", "Electric potential for joule heating.");
  params.addParam<MaterialPropertyName>("electic_conductivity", "electic_conductivity", "material property providing electrical resistivity of the material.");
  return params;
}

ElectricFieldKernel::ElectricFieldKernel(const InputParameters & parameters) :
    DerivativeMaterialInterface<JvarMapKernelInterface<Kernel> >(parameters),
    _elec(coupledValue("elec")),
    _grad_elec(coupledGradient("elec")),
    _elec_var(coupled("elec")),
    _electic_conductivity(getMaterialProperty<Real>("electic_conductivity")),
    _dRdT(getMaterialPropertyDerivative<Real>("electic_conductivity", _var.name()))
{
  // Get number of coupled variables
  unsigned int nvar = _coupled_moose_vars.size();

  // reserve space for derivatives
  _dRdarg.resize(nvar);

  // Iterate over all coupled variables
  for (unsigned int i = 0; i < nvar; ++i)
    _dRdarg[i] = &getMaterialPropertyDerivative<Real>("electic_conductivity", _coupled_moose_vars[i]->name());
}

Real
ElectricFieldKernel::computeQpResidual()
{
  return -_electic_conductivity[_qp] * _grad_elec[_qp] * _grad_elec[_qp] * _test[_i][_qp];
}

Real
ElectricFieldKernel::computeQpJacobian()
{
  return -_dRdT[_qp] * _grad_elec[_qp] * _grad_elec[_qp] * _test[_i][_qp] * _phi[_j][_qp];
}

Real
ElectricFieldKernel::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _elec_var)
    return -2 * _electic_conductivity[_qp] * _grad_elec[_qp] * _test[_i][_qp] * _grad_phi[_j][_qp];
  else
  {  // Get the coupled variable jvar is referring to
    const unsigned int cvar = mapJvarToCvar(jvar);

    return -(*_dRdarg[cvar])[_qp] * _grad_elec[_qp] * _grad_elec[_qp] * _test[_i][_qp] * _phi[_j][_qp];
  }
}
