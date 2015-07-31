#include "FlowAux.h"

template<>
InputParameters validParams<FlowAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<unsigned int>("index", "index of the flow stress");
  return params;
}

FlowAux::FlowAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _flow_vector(getMaterialProperty<std::vector<Real> >("flow")),
    _index(getParam<unsigned int>("index") )
{
}

Real
FlowAux::computeValue()
{
  return _flow_vector[_index][_qp];
}
