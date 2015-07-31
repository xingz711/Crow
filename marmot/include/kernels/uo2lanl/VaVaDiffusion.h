#ifndef VAVADIFFUSION_H
#define VAVADIFFUSION_H

#include "CHBulk.h"
#include "Material.h"

//Forward Declarations
class VaVaDiffusion;

template<>
InputParameters validParams<VaVaDiffusion>();

class VaVaDiffusion : public CHBulk
{
public:

  VaVaDiffusion(const InputParameters & parameters);

protected:

  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:

  std::string _LXeVa_name;
  const MaterialProperty<Real> & _LXeVa;

  std::string _LUVa_name;
  const MaterialProperty<Real> & _LUVa;

  std::string _LXeU_name;
  const MaterialProperty<Real> & _LXeU;

  std::string _LXeVaU_name;
  const MaterialProperty<Real> & _LXeVaU;

  std::string _LXeVaVa_name;
  const MaterialProperty<Real> & _LXeVaVa;

  std::string _LXeVaXe_name;
  const MaterialProperty<Real> & _LXeVaXe;

  std::string _LXeVa2_name;
  const MaterialProperty<Real> & _LXeVa2;

  std::string _LXeVaVa2_name;
  const MaterialProperty<Real> & _LXeVaVa2;

  std::string _LUVa2_name;
  const MaterialProperty<Real> & _LUVa2;

  std::string _LVaVa2_name;
  const MaterialProperty<Real> & _LVaVa2;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  const MaterialProperty<Real> & _kT;

  const MaterialProperty<Real> & _cu;
  const MaterialProperty<RealGradient> & _grad_cu;
};

#endif //VAVADIFFUSION_H
