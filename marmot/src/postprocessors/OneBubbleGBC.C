#include "OneBubbleGBC.h"

template<>
InputParameters validParams<OneBubbleGBC>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<Real>("Ly", "Length of y-direction of GB");
  params.addRequiredParam<Real>("Lz", "Length of z-direction of GB");
  params.addRequiredParam<Real>("radius_bub", "Footprint radius of GB bubble");

  return params;
}

OneBubbleGBC::OneBubbleGBC(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _Ly(getParam<Real>("Ly")),
    _Lz(getParam<Real>("Lz")),
    _radius_bub(getParam<Real>("radius_bub"))
{
}

Real OneBubbleGBC::getValue()
{
  Real GB_area = _Ly * _Lz;
  Real Bu_area = libMesh::pi * _radius_bub*_radius_bub;

  return Bu_area / GB_area;
}
