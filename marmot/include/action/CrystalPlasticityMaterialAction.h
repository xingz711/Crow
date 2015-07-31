#ifndef CRYSTALPLASTICITYMATERIALACTION_H
#define CRYSTALPLASTICITYMATERIALACTION_H

#include "Action.h"

//Forward Declarations
class CrystalPlasticityMaterialAction;

template<>
InputParameters validParams<CrystalPlasticityMaterialAction>();

class CrystalPlasticityMaterialAction: public Action
{
public:
  CrystalPlasticityMaterialAction(const InputParameters & params);

  virtual void act();

private:
  Real _c11;
  Real _c12;
  Real _c44;

  Real _youngs_modulus;
  Real _poissons_ratio;

  // Real _mu;
  Real _g0;
  Real _h0;
  Real _q0;
  Real _a0;
  Real _tau0;

  FileName _euler_angle_file_name;
  FileName _slip_plane_file_name;

  unsigned int _nss;
  unsigned int _op_num;
  std::string _var_name_base;

  std::vector<Real> _angle;
};

#endif //CRYSTALPLASTICITYMATERIALACTION_H
