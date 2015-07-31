#include "UO2GrGr.h"

template<>
InputParameters validParams<UO2GrGr>()
{
  InputParameters params = validParams<GBEvolution>();

  params.set<Real>("GBmob0") = 9.2124e-09; // Grain boundary mobility prefactor in m^4/(J*s), defaults to the value for UO2 from AINSCOUGH et al., 1973
  params.set<Real>("Q") = 2.77; // Grain boundary migration activation energy in eV, defaults to the value for UO2 from AINSCOUGH et al., 1973
  params.set<Real>("GBenergy") = 1.58; // Grain boundary energy in J/m^2, defaults to the value for UO2 averaged from Nerikar et al., 2011
  params.set<Real>("molar_volume") = 24.62e-6; // Molar volume in m^3/mol, needed for temperature gradient driving force

  return params;
}

UO2GrGr::UO2GrGr(const InputParameters & parameters)
  :GBEvolution(parameters)
{}
