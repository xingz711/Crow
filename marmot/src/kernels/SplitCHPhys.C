#include "SplitCHPhys.h"
#include "MathUtils.h"

template<>
InputParameters validParams<SplitCHPhys>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addParam<std::string>("Ef_name", "Efv", "Name used in the materials for the formation energy");
  return params;
}

SplitCHPhys::SplitCHPhys(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _kT(getMaterialPropertyByName<Real>("kT")),
    _Ef_name(getParam<std::string>("Ef_name")),
    _Efv(getMaterialProperty<Real>(_Ef_name))
{
}

Real
SplitCHPhys::computeDFDC(PFFunctionType type)
{
  const Real tol = 1.0e-6;
  Real c = _u[_qp];

  // check against tolerance and correct c values accordingly
  c = c < tol ? tol : (c > 1.0-tol ? 1.0-tol : c);

  Real wparam = _Efv[_qp];

  switch (type)
  {
    case Residual:
      // return _kT[_qp] * (poly2Log(c, tol, 0) - poly2Log(1.0 - c, tol, 0)) + wparam*(1.0 - 2.0 * c);
      return _kT[_qp] * (std::log(c) - std::log(1.0 - c)) + wparam * (1.0 - 2.0 * c);;

    case Jacobian:
      // (_kT[_qp]*(poly2Log(c,tol,1) + poly2Log(1.0-c,tol, 1)) - 2.0*wparam);
      return _phi[_j][_qp] * (_kT[_qp] * (1.0/c + 1.0 / (1.0 - c)) - wparam * 2.0);
  }

  mooseError("Invalid type passed in");
}
