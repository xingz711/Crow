#include "GBCalcAux.h"

template<>
InputParameters validParams<GBCalcAux>()
{
  InputParameters params = validParams<BndsCalcAux>();
  params.addParam<Real>("GBthreshold", 0.9, "Threshold of bnds term to distinguish GBs");
  return params;
}

GBCalcAux::GBCalcAux(const InputParameters & parameters) :
    BndsCalcAux(parameters),
    _GBthreshold(getParam<Real>("GBthreshold"))
{
}

Real
GBCalcAux::computeValue()
{
  Real p12 = 0;

  for (unsigned int i = 0; i < _ncrys; ++i)
    for (unsigned int j = i + 1; j < _ncrys; ++j)
      p12 += (*_vals[i])[_qp] * (*_vals[j])[_qp];

  Real value = 0;
  Real bnds = BndsCalcAux::computeValue();

  if (bnds <= _GBthreshold && p12 > 0.01)
    value = 1.0;

  return value;
}
