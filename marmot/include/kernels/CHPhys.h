#ifndef CHPHYS_H
#define CHPHYS_H

#include "CHBulk.h"

//Forward Declarations
class CHPhys;

template<>
InputParameters validParams<CHPhys>();

class CHPhys : public CHBulk
{
public:
  CHPhys(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  const MaterialProperty<Real> & _kT;

  /// Formtion energy material property
  const MaterialProperty<Real> & _Efv;

  /// True id the double well term is used
  bool _dwterm;
};

#endif //CHPHYS_H
