#include "MicroThermalUO2XeT.h"

template<>
InputParameters validParams<MicroThermalUO2XeT>()
{
  InputParameters params = validParams<MicroThermal>();
  params.addCoupledVar("T", 300.0, "Temperature in Kelvin");
  params.addCoupledVar("conc_Xe", 0.0, "Xe concentration");
  MooseEnum model_type_options("MD analytical");
  params.addRequiredParam<MooseEnum>("model_type", model_type_options, "MD: MD+Fink; analytical: Analytical model by Ronchi.");
  return params;
}

MicroThermalUO2XeT::MicroThermalUO2XeT(const InputParameters & parameters) :
    MicroThermal(parameters),
    _T(coupledValue("T")),
    _conc_Xe(coupledValue("conc_Xe")),
    _model_type(getParam<MooseEnum>("model_type"))
{
}

Real
MicroThermalUO2XeT::computeBulkThermalConductivity()
{
  Real UO2_cond;
  Real Coeff_Xe;
  Real D_Xe;
  Real t;

  switch (_model_type)
  {
  case 0: // MD
    Coeff_Xe = 1.5;
    D_Xe = 0.45;
    break;

  case 1: // analytical
    Coeff_Xe = 20.3;
    D_Xe = 1.0;
    break;
  }

  t = _T[_qp] / 1000.0;
  UO2_cond = 100.0/(7.5408 + 17.692*t + 3.6142*t*t + 100.0*Coeff_Xe*std::pow(_conc_Xe[_qp], D_Xe)) + 6400.0/std::pow(t, 2.5)*std::exp(-16.35/t); //for 95% density
  UO2_cond = UO2_cond/(1-(2.6-0.5*t)*0.05); //Convert thermal conductivity from 95% to 100% density. Eq. (18) in Fink, JNM 279, 1, 2000.

  if (UO2_cond < 0.0)
    mooseError("MicroThermalUO2XeT UO2_cond cannot be less than 0!");

  return UO2_cond;
}

