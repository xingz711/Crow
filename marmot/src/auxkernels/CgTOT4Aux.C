#include "CgTOT4Aux.h"

template<>
InputParameters validParams<CgTOT4Aux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1","Coupled variable, Xe concentration");
  params.addRequiredCoupledVar("coupled2","Coupled variable, XeVa concentration");
  params.addRequiredCoupledVar("coupled3","Coupled variable, XeVaVa concentration");
  params.addRequiredCoupledVar("coupled4","Coupled variable, XeXeVaVa concentration");

  return params;
}

CgTOT4Aux::CgTOT4Aux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2")),
    _coupled_val3(coupledValue("coupled3")),
    _coupled_val4(coupledValue("coupled4"))

{
}

Real
CgTOT4Aux::computeValue()
{
  //Calculates the total Xe concentration for the Xe, Va, XeVa, VaVa, UI, XeVaVa, XeXeVaVa system

  return _coupled_val1[_qp] + _coupled_val2[_qp] + _coupled_val3[_qp]+ 2*_coupled_val4[_qp];
}
