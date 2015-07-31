#include "ACGrGrGenericDF.h"

#include "Material.h"

template<>
InputParameters validParams<ACGrGrGenericDF>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredRangeCheckedParam<std::vector<Real> >("DF", "DF_size <= 3", "Driving force vector in J/m^3, no more than 3 entries");
  params.addParam<Real>("length_scale", 1.0e-9, "Length scale of the problem in meters");
  params.addCoupledVar("c", 0.0, "concentration representing a void, bubble or precipitate");

  return params;
}

ACGrGrGenericDF::ACGrGrGenericDF(const InputParameters & parameters) :
    ACBulk(parameters),
    _length_scale(getParam<Real>("length_scale")),
    _c(coupledValue("c")),
    _l_GB(getMaterialProperty<Real>("l_GB")),
    _JtoeV(6.24150974e18) // joule to eV conversion
{
  std::vector<Real> DF = getParam<std::vector<Real> >("DF");
  while(DF.size() < 3)
    DF.push_back(0.0);

  _DF = RealGradient(DF[0], DF[1], DF[2]);
  _DF = _DF * _JtoeV*(_length_scale*_length_scale*_length_scale); // Convert to eV/_length_scale^3
}

Real
ACGrGrGenericDF::computeDFDOP(PFFunctionType type)
{
  Real A = -0.75*_l_GB[_qp];
  // A = 1.0;

  Real mult = (1.0 - _c[_qp]);
  if (mult < 0.0)
    mult = 0.0;
  if (mult > 1.0)
    mult = 1.0;

  switch (type)
  {
    case Residual:
      return mult * A * _grad_u[_qp] * _DF;

    case Jacobian:
      return mult * A * _grad_phi[_j][_qp] * _DF;
  }

  mooseError("Invalid type passed in");
}
