#ifndef XEVA2DIFFUSION_H
#define XEVA2DIFFUSION_H

#include "CHBulk.h"

//Forward Declarations
class XeVa2Diffusion;

template<>
InputParameters validParams<XeVa2Diffusion>();

class XeVa2Diffusion : public CHBulk
{
public:
  XeVa2Diffusion(const InputParameters & parameters);

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

  //  VariableValue & _c1;
  //  VariableGradient & _grad_c1;

  //  VariableValue & _c2;
  //  VariableGradient & _grad_c2;

  //  VariableValue & _c4;
  //  VariableGradient & _grad_c4;

  const MaterialProperty<Real> & _kT;

  const MaterialProperty<Real> & _cu;
  const MaterialProperty<RealGradient> & _grad_cu;
};

#endif //XEVA2DIFFUSION_H
