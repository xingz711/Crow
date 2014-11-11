#ifndef CHTEMP_H
#define CHTEMP_H

#include "CHBulk.h"

//Forward Declarations
class CHTemp;

template<>
InputParameters validParams<CHTemp>();

/**Cahn-Hilliard Kernel implementing the free energy for sintering
 **/
class CHTemp : public CHBulk
{
public:
  CHTemp(const std::string & name, InputParameters parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  Real _temp;
  Real _r;
};

#endif //CHMATH_H
