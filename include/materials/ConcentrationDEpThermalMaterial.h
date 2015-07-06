#ifndef MICROTHERMALUO2XET_H
#define MICROTHERMALUO2XET_H

#include "MicroThermal.h"

//Forward Declarations
class MicroThermalUO2XeT;

template<>
InputParameters validParams<MicroThermalUO2XeT>();

/**
 * This kernel computes the thermophysical material properties of UO2 at different T and Xe concentration.
 * The temperature-dependence of Xe-free UO2 can be found at:
 * J.K. Fink, Thermophysical Properties of Uranium Dioxide, Journal of Nuclear
 * Materials, Vol. 279 p. 1-18, 2000.
 * The Xe-dependece is from LANL's MD calculation
 */
class MicroThermalUO2XeT : public MicroThermal
{
public:
  MicroThermalUO2XeT(const std::string & name,
          InputParameters parameters);

protected:
  virtual Real computeBulkThermalConductivity();
  VariableValue &  _T;
  VariableValue &  _conc_Xe;
  MooseEnum  _model_type;
};

#endif //MICROTHERMALUO2XET_H
