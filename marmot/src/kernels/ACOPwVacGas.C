#include "ACOPwVacGas.h"

template<>
InputParameters validParams<ACOPwVacGas>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("cv", "Other species concentration");
  params.addRequiredCoupledVar("cg", "Other species concentration");
  params.addRequiredParam<MaterialPropertyName>("W_name", "The name of the W property");
  return params;
}

ACOPwVacGas::ACOPwVacGas(const InputParameters & parameters) :
    ACBulk(parameters),
    _cv(coupledValue("cv")),
    _cg(coupledValue("cg")),
    _Efv(getMaterialPropertyByName<Real>("Efv")),
    _Efg(getMaterialPropertyByName<Real>("Efg")),
    _RT(getMaterialPropertyByName<Real>("RT")),
    _W(getMaterialProperty<Real>("W_name"))
{
}

Real
ACOPwVacGas::computeDFDOP(PFFunctionType type)
{
  Real cv = _cv[_qp];
  Real cg = _cg[_qp];
  Real op = _u[_qp];
  Real mcv = 1.0 - cv - cg;

  // Compute log products
  Real cvlogcv = 0.0;
  if (cv > 0.0)
    cvlogcv = cv * std::log(cv);

  Real cglogcg = 0.0;
  if (cg > 0.0)
    cglogcg = cg * std::log(cg);

  Real mcvlogmcv = 0.0;
  if (mcv > 0.0)
    mcvlogmcv = mcv * std::log(mcv);

  // Compute free energies
  Real fm = _RT[_qp]*(cvlogcv + cglogcg + mcvlogmcv) + _Efv[_qp]*cv + _Efg[_qp]*cg;
  Real fb = (1.0 - cv)*(1.0 - cv);

  switch (type)
  {
    case Residual:
    {
      // Compute order parameter function derivatives
      Real dhdop = 30.0*(op*op*op*op - 2.0*op*op*op + op*op);
      Real dgdop = 2.0*op - 6.0*op*op + 4.0*op*op*op;

      return -dhdop*fm + dhdop*fb + _W[_qp]*dgdop;
    }

    case Jacobian:
    {
      // Compute order parameter function derivatives
      Real Ddhdop = _phi[_j][_qp]*30.0*(4.0*op*op*op - 6.0*op*op + 2.0*op);
      Real Ddgdop = 2.0*_phi[_j][_qp]*(1.0 - 6.0*op + 6.0*op*op);

      return -Ddhdop*fm + Ddhdop*fb + _W[_qp]*Ddgdop;
    }
  }

  mooseError("Invalid type passed in");
}
