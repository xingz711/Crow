#include "CuGrGr.h"

template<>
InputParameters validParams<CuGrGr>()
{
  InputParameters params = validParams<GBEvolution>();

  params.set<Real>("GBmob0") = 2.5e-6; // Grain boundary mobility prefactor in m^4/(J*s), defaults to the value for copper from Schoenfelder1997
  params.set<Real>("Q") = 0.23; // Grain boundary migration activation energy in eV, defaults to the value for copper from Schoenfelder1997
  params.set<Real>("GBenergy") = 0.708; // Grain boundary energy in J/m^2, defaults to the value for copper from Schoenfelder1997
  params.set<Real>("molar_volume") = 7.11e-6; // Molar volume in m^3/mol, needed for temperature gradient driving force

  return params;
}

CuGrGr::CuGrGr(const InputParameters & parameters) :
    GBEvolution(parameters)
{
}
