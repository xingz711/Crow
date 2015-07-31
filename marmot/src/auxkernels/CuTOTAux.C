#include "CuTOTAux.h"

template<>
InputParameters validParams<CuTOTAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1","Coupled variable, Xe concentration");
  params.addRequiredCoupledVar("coupled2","Coupled variable, Va concentration");
  params.addRequiredCoupledVar("coupled3","Coupled variable, XeVa concentration");
  params.addRequiredCoupledVar("coupled4","Coupled variable, VaVa concentration");

  return params;
}

CuTOTAux::CuTOTAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2")),
    _coupled_val3(coupledValue("coupled3")),
    _coupled_val4(coupledValue("coupled4"))
{
}

Real
CuTOTAux::computeValue()
{
  //Calculates the U concentration for the Xe, Va, XeVa, VaVa system

  return   1.0
           - _coupled_val1[_qp] - _coupled_val2[_qp]
           - 2.0 * _coupled_val3[_qp] - 2.0 * _coupled_val4[_qp];
}
