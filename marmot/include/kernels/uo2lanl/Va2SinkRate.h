#ifndef Va2SinkRate_H
#define Va2SinkRate_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class Va2SinkRate;

template<>
InputParameters validParams<Va2SinkRate>();

class Va2SinkRate : public Kernel
{
public:

  Va2SinkRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:

  std::string _MVa2_name;

  const MaterialProperty<Real> & _MVa2;
  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;
  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Zg;
  const MaterialProperty<Real> & _dgHVa2;
  const MaterialProperty<Real> & _dgSVa2;
  const MaterialProperty<Real> & _dgHVa;
  const MaterialProperty<Real> & _dgSVa;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cvv;
  const MaterialProperty<Real> & _kappa_cui;
  const MaterialProperty<Real> & _kappa_cu;
  const MaterialProperty<Real> & _kV2;
};

#endif //COEFDIFFUSION_H
