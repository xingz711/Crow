#include "CgBTOTAux.h"

template<>
InputParameters validParams<CgBTOTAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1", "Coupled variable, Xe conncentration");
  params.addRequiredCoupledVar("coupled2", "Coupled variable, XeVa concentration");
  params.addRequiredCoupledVar("coupled3", "Coupled variable, XeVaVa concentration");

  return params;
}

CgBTOTAux::CgBTOTAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2")),
    _coupled_val3(coupledValue("coupled3"))
{
}

Real
CgBTOTAux::computeValue()
{
  //Calculates the total Xe concentration for the Xe, Va, XeVa, VaVa, UI, XeVaVa system, but XeVaVa is not treated as mobile (incorrect and no longer used)

  return _coupled_val1[_qp] + _coupled_val2[_qp] + _coupled_val3[_qp];
}
