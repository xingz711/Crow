#include "VoidTempGrad.h"

template<>
InputParameters validParams<VoidTempGrad>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVar("temp", "Temperature");
  params.addRequiredParam<Real>("Dvan_zero", "diffusivity coefficient, D_0");
  params.addRequiredParam<Real>("Act_energy", "diffusion activation energy");
  params.addRequiredParam<Real>("Heat_trans", "heat of transport");
  return params;
}

VoidTempGrad::VoidTempGrad(const InputParameters & parameters) :
    Material(parameters),
    _temp(coupledValue("temp")),
    _grad_temp(coupledGradient("temp")),
    _Dvan(declareProperty<Real>("Dvan")),
    // _grad_Dvan(declareProperty<RealGradient>("grad_Dvan")),
    _kT(declareProperty<Real>("kT")),
    _Qheat(declareProperty<Real>("Qheat")),
    _mobtemp(declareProperty<Real>("mobtemp")),
    _Dvan_zero(getParam<Real>("Dvan_zero")),
    _Act_energy(getParam<Real>("Act_energy")),
    _Heat_trans(getParam<Real>("Heat_trans"))
{
}

void
VoidTempGrad::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kT[_qp] = (_temp[_qp]/298.)*0.0256; // diffusivity in um^2/s
    // _kT[_qp] = (1230./298.)*0.0256;

    // (be consistent with length scale)
    _Dvan[_qp] = _Dvan_zero * std::exp(-_Act_energy/_kT[_qp]);

    _Qheat[_qp] = -_Heat_trans; // heat of transport in eV
    _mobtemp[_qp] = _Dvan[_qp] / _kT[_qp];
  }
}
