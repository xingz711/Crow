#include "SplitCHBeta.h"

template<>
InputParameters validParams<SplitCHBeta>()
{
  InputParameters params = validParams<SplitCHBase>();
  params.addRequiredParam<Real>("Beta", "Parameter to adjust this new function");
  return params;
}

SplitCHBeta::SplitCHBeta(const InputParameters & parameters) :
    SplitCHBase(parameters),
    _Beta(getParam<Real>("Beta"))
{
}

Real // Echiample of what the virtual function should look like
SplitCHBeta::computeDFDC(PFFunctionType type)
{
  Real chi = _u[_qp]; // this is the concentration variable: [Cr] (Wt. percentage) in alloy
  switch (type)
  {
    case Residual:
    {
      Real dF_dchi = _Beta * 4 * std::pow((-1+chi), 4) * std::pow(chi, 3) + 4 * std::pow((-1+chi), 3) * std::pow(chi, 4);
      return dF_dchi;
    }
    case Jacobian:
    {
      Real d2F_dchi2 = _Beta * 12 * std::pow((-1+chi),4) * std::pow(chi, 2) + 32 * std::pow((-1+chi), 3) * std::pow(chi, 3) + 12 * std::pow((-1+chi), 2) * std::pow(chi, 4);
      Real Jacobian = d2F_dchi2 * _phi[_j][_qp];
      return Jacobian;
    }
  }

  mooseError("Invalid type passed in");
}
