#include "RadSourcePoly.h"

#include "Material.h"

template<>
InputParameters validParams<RadSourcePoly>()
{
  InputParameters params = validParams<Kernel>();
  // TODO: This should be a MooseEnum!
  params.addRequiredParam<bool>("Vac_or_Int", "Is this a vancacy (true) or an interstitial (false) source?");
  return params;
}
RadSourcePoly::RadSourcePoly(const InputParameters & parameters) :
    KernelValue(parameters),
    _Vac_or_Int(getParam<bool>("Vac_or_Int")),
    _vacancy_increase(getMaterialProperty<Real>("vacancy_increase")),
    _interstitial_increase(getMaterialProperty<Real>("interstitial_increase"))
{
}

void
RadSourcePoly::subdomainSetup()
{
}

Real
RadSourcePoly::precomputeQpResidual()
{
  Real Pv = 0.0;

  if (_Vac_or_Int)
    Pv = _vacancy_increase[_qp];
  else
    Pv = _interstitial_increase[_qp];

  return -Pv;
}
