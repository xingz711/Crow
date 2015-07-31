// Original class author: A.M. Jokisaari,  O. Heinonen

// CDD Finite Strain Crystal Plasticity Material
// author: Erin Iesulauro Barker (erin.barker@pnnl.gov)
// This model is a dislocation density informed crystal plasticity model
// developed with Hussein Zbib from WSU

#ifndef CDDFSPLASTICMATERIAL_H
#define CDDFSPLASTICMATERIAL_H

#include "FiniteStrainMaterial.h"

class CDDFSPlasticMaterial;

template<>
InputParameters validParams<CDDFSPlasticMaterial>();

class CDDFSPlasticMaterial : public FiniteStrainMaterial
{
public:
  CDDFSPlasticMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpStress();
  virtual void computeQpElasticityTensor();
  virtual void initQpStatefulProperties();
  virtual void calc_resid_jacob(RankTwoTensor & pk2,
                                RankTwoTensor & sig,
                                RankTwoTensor & fp_old_inv,
                                RankTwoTensor & fp_inv,
                                std::vector<Real> & slip_incr,
                                std::vector<Real> & tau,
                                RankTwoTensor & resid,
                                RankFourTensor & jac);

  virtual void get_slip_incr(std::vector<Real> & tau,
                             std::vector<Real> & slip_incr,
                             std::vector<Real> & dslipdtau);

  virtual void update_gss_tau(std::vector<Real> & /*slip_incr*/,
                              std::vector<Real> & tau);

  virtual void get_slip_sys();
  virtual void get_euler_ang();

  void get_euler_rot();
  RankTwoTensor getmatrot(RankTwoTensor &a);

  const unsigned int _nss;

  std::vector<Real> _flowprops;

  std::string _slip_sys_file_name;
  std::string _euler_angle_file_name;

  std::vector<Real> _mo;
  std::vector<Real> _no;

  std::vector<Real> _a0;
  std::vector<Real> _xm;

  Real _rtol;
  Real _gtol;
  Real _slip_incr_tol;

  MaterialProperty<RankTwoTensor> & _fp;
  MaterialProperty<RankTwoTensor> & _fp_old;
  MaterialProperty<RankTwoTensor> & _pk2;
  MaterialProperty<RankTwoTensor> & _pk2_old;
  MaterialProperty<RankTwoTensor> & _lag_e;
  MaterialProperty<std::vector<Real> >  & _gss;
  MaterialProperty<std::vector<Real> >  & _gss_old;
  MaterialProperty<Real>  & _acc_slip;
  MaterialProperty<Real>  & _acc_slip_old;
  MaterialProperty<RankTwoTensor> & _update_rot;
  MaterialProperty<RankTwoTensor> & _crysrot;
  MaterialProperty<RankTwoTensor> & _crysrot_old;

  Real _b;
  Real _mu;
  Real _nu;
  Real _a;
  Real _beta;
  Real _Rc;
  Real _rhoDL_0;
  Real _dl_0;
  Real _tau0;
  Real _rhoM_0;
  Real _rhoI_0;
  Real _ani;
  std::vector<Real> _alphas;

  Real _alpha_1;
  Real _alpha_2;
  Real _alpha_3;
  Real _alpha_4;
  Real _alpha_5;
  Real _alpha_6;
  Real _v0;

  MaterialProperty<std::vector<Real> > & _rhoM;
  MaterialProperty<std::vector<Real> > & _rhoM_old;
  MaterialProperty<std::vector<Real> > & _rhoI;
  MaterialProperty<std::vector<Real> > & _rhoI_old;
  MaterialProperty<Real> & _total_density;
  MaterialProperty<std::vector<Real> > & _tau_save;
  MaterialProperty<std::vector<Real> > & _slip_incr_save;
};

#endif //CDDFSPLASTICMATERIAL_H
