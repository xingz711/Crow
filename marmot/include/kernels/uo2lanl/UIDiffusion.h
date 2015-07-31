#ifndef UIDIFFUSION_H
#define UIDIFFUSION_H

#include "CHBulk.h"
#include "Material.h"

//Forward Declarations
class UIDiffusion;

template<>
InputParameters validParams<UIDiffusion>();

class UIDiffusion : public CHBulk
{
public:
  UIDiffusion(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _kT;
};

#endif //UIDIFFUSION_H
