#ifndef XEDIFFUSION_H
#define XEDIFFUSION_H

#include "Kernel.h"

//Forward Declarations
class XeDiffusion;

template<>
InputParameters validParams<XeDiffusion>();

class XeDiffusion : public Kernel
{
public:

  XeDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  std::string _D_name;
  const MaterialProperty<Real> & _D;

  std::string _LXe_name;
  const MaterialProperty<Real> & _LXe;

  std::string _MXe_name;
  const MaterialProperty<Real> & _MXe;

  VariableValue & _c1Old;

  // std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  // std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;
};

#endif //XEDIFFUSION_H
