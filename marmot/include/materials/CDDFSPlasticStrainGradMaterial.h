#ifndef CDDFSPLASTICSTRAINGRADMATERIAL_H
#define CDDFSPLASTICSTRAINGRADMATERIAL_H

#include "FiniteStrainMaterial.h"

class CDDFSPlasticStrainGradMaterial;

template<>
InputParameters validParams<CDDFSPlasticStrainGradMaterial>();

/**
 * \brief CDD Finite Strain Crystal Plasticity Material
 * This model is a dislocation density informed crystal plasticity model
 * developed with Hussein Zbib from WSU.
 * This model adds in gradient plasticity terms to the CDD crystal plasticity
 * model.  Developed with Hussein Zbib and myself at WSU.
 * CDD Finite Strain Crystal Plasticity Material with Strain Gradient Terms
 *
 * \author Erin Iesulauro Barker (erin.barker@pnnl.gov)
 * \author Stephanie Pitts (sapitts@wsu.edu)
 * \author A.M. Jokisaari
 * \author O. Heinonen
 *
 * \remark This is a first cut of this implementation, updates and bug fixes to come.
 */
class CDDFSPlasticStrainGradMaterial : public FiniteStrainMaterial
{
public:
  CDDFSPlasticStrainGradMaterial(const InputParameters & parameters);

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

  virtual void update_gss_tau(std::vector<Real> & slip_incr,
                              std::vector<Real> & tau);

  virtual void get_slip_sys();
  virtual void get_euler_ang();

  void get_euler_rot();
  RankTwoTensor getmatrot(RankTwoTensor &a);

  void calc_l_inverse();

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

  MaterialProperty<Real> & _l_inv;
  MaterialProperty<Real> & _rhoSG;

  // Additional Variables for the Strain Gradient Implementation:
  MaterialProperty<RankTwoTensor> & _couple_stress_x;
  MaterialProperty<RankTwoTensor> & _couple_stress_x_old;
  MaterialProperty<RankTwoTensor> & _couple_stress_y;
  MaterialProperty<RankTwoTensor> & _couple_stress_y_old;
  MaterialProperty<RankTwoTensor> & _couple_stress_z;
  MaterialProperty<RankTwoTensor> & _couple_stress_z_old;
  Real _A_ijklmn;  // Will become the Rank-6 Couple Stress Elasticity Tensor
  Real _alpha_sg;

  VariableSecond & _second_disp_x;
  VariableSecond & _second_disp_y;
  VariableSecond & _second_disp_z;

  VariableSecond & _second_disp_x_old;
  VariableSecond & _second_disp_y_old;
  VariableSecond & _second_disp_z_old;

  // Additional Variables to print out the second derivative of the displacement
  MaterialProperty<RankTwoTensor> & _SX;
  MaterialProperty<RankTwoTensor> & _SY;
  MaterialProperty<RankTwoTensor> & _SZ;
};

#endif //CDDFSPLASTICSTRAINGRADMATERIAL_H
