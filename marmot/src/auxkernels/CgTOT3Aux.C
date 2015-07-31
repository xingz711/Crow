#include "CgTOT3Aux.h"

template<>
InputParameters validParams<CgTOT3Aux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1","Coupled variable, Xe concentration");
  params.addRequiredCoupledVar("coupled2","Coupled variable, XeVa concentration");
  params.addRequiredCoupledVar("coupled3","Coupled variable, XeVaVa concentration");

  return params;
}

CgTOT3Aux::CgTOT3Aux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2")),
    _coupled_val3(coupledValue("coupled3"))

{
}

Real
CgTOT3Aux::computeValue()
{
  //Calculates the total Xe concentration for the Xe, Va, XeVa, VaVa, UI, XeVaVa system

  return _coupled_val1[_qp] + _coupled_val2[_qp] + _coupled_val3[_qp];
}
