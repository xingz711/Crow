#include "CgTOTAux.h"

template<>
InputParameters validParams<CgTOTAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1","Coupled variable, Xe concentration");
  params.addRequiredCoupledVar("coupled2","Coupled variable, XeVa concentration");

  return params;
}

CgTOTAux::CgTOTAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2"))

{
}

Real
CgTOTAux::computeValue()
{
  //Calculates the total Xe concentration for the Xe, Va, XeVa, VaVa, UI system

  return _coupled_val1[_qp] + _coupled_val2[_qp];
}
