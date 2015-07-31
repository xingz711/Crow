#include "CHPhys.h"

template<>
InputParameters validParams<CHPhys>()
{
  InputParameters params = validParams<CHBulk>();
  params.addParam<bool>("dwterm", true, "Double well term used");
  params.addParam<MaterialPropertyName>("Ef_name", "Efv", "Name used in the materials for the formation energy");

  return params;
}

CHPhys::CHPhys(const InputParameters & parameters) :
    CHBulk(parameters),
    _kT(getMaterialProperty<Real>("kT")),
    _Efv(getMaterialProperty<Real>("Ef_name")),
    _dwterm(getParam<bool>("dwterm"))
{
}

RealGradient
CHPhys::computeGradDFDCons(PFFunctionType type)
{
  Real w = _Efv[_qp];

  Real d2fdc2 = _kT[_qp];
  if (_dwterm)
    d2fdc2 += -w * 2.0 * _u[_qp] * (1.0 - _u[_qp]);

  // _console << "w = " << w << ", d2fdc2 = " << d2fdc2 << ", kT = " << _kT[_qp] << std::endl;

  switch (type)
  {
    case Residual:
      return d2fdc2 * _grad_u[_qp];

    case Jacobian:
      Real d3fdc3 = 0.0;
      if (_dwterm)
        d3fdc3 += - w * 2.0 * (1.0 - 2.0 * _u[_qp]);

      // _console << ", d2fdc2 = " << d2fdc2 << ", d3fdc3 = " << d3fdc3 << std::endl;
      // return Jacobian value
      return d2fdc2 * _grad_phi[_j][_qp] + d3fdc3 * _grad_u[_qp] * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}
