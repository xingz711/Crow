#ifndef FECRINT_ENERGY_H
#define FECRINT_ENERGY_H

#include "GeneralPostprocessor.h"

//Forward Declarations
class FeCrInt_energy;

template<>
InputParameters validParams<FeCrInt_energy>();

/**
 * This postprocessor computes the interfacial zone between the alpha and alpha'
 * phases within an Fe-Cr binary system.
 * Note that specializations of this integral are possible by deriving from this
 * class and overriding computeQpIntegral().
 */
class FeCrInt_energy : public GeneralPostprocessor
{
public:
  FeCrInt_energy(const InputParameters & parameters);
  virtual void initialize();
  virtual void execute();
  virtual PostprocessorValue getValue();

protected:
   const PostprocessorValue & _alphaConc;
   const PostprocessorValue & _alphaPrimeConc;
   const PostprocessorValue & _FreeAlpha_density;
   const PostprocessorValue & _FreeAlphaPrime_density;
   const PostprocessorValue & _Tot_F;
   Real _T;
   // VariableValue & _T;
};

#endif //FECRINT_ENERGY_H
