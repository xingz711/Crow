#ifndef COUPLEDPFCRYSPLASTMATERIAL_H
#define COUPLEDPFCRYSPLASTMATERIAL_H

#include "FiniteStrainCrystalPlasticity.h"

class CoupledPFCrysPlastMaterial;
class GrainTracker;

template<>
InputParameters validParams<CoupledPFCrysPlastMaterial>();

class CoupledPFCrysPlastMaterial : public FiniteStrainCrystalPlasticity
{
public:
  CoupledPFCrysPlastMaterial(const InputParameters & parameters);

protected:

  /// These are new functions
  virtual void getRotatedElasticityTensor(); // Read in Euler Angles and rotate Elasticity Tensor for each grain
  virtual void getRotatedSchmidTensor(); // Read in Slip Systems and rotate Schmid Tensor for each grain
  virtual void getLatentHardeningProps(); // Calc the Latent Hardening Matrix used in plastic potential expression
  virtual void initQpVariables(); // Initialize additive variable in stress interpolation
  virtual void preFSCPrun(unsigned int&); // Assign rotated properties for each individual phase
  virtual void postFSCPrun(Real&); // Weight-average properties among different phases
  virtual void initSubPot(unsigned int&); // Initialize Sub elastic/plastic potential for each phase
  virtual void calcSubPot(unsigned int&); // Calculate Sub elastic/plastic potential for each phase
  virtual void calcDrivingForce(Real&); // Calculate elastic/plastic driving force

  /// Override these functions
  virtual void computeProperties(); // Override to preform stress interpolation scheme
  virtual void preSolveQp();
  virtual void postSolveQp(); // Get Sub properties from each phase
  virtual void preSolveStatevar(); // Modify initial guess value of _gss_tmp
  virtual void solveStatevar();
  virtual void postSolveStatevar();
  virtual void preSolveStress(); // Modify intial guess value of _pk2_tmp
  virtual void solveStress();
  virtual void initAdditionalProps();
  // virtual void calcResidual( RankTwoTensor & );

  /// Override but EMPTY these functions
  virtual void getEulerAngles();
  virtual void getEulerRotations();
  virtual void getSlipSystems();
  virtual void postSolveStress();

  /// Euler Angle file
  std::string _Euler_angles_file_name;

  /// Sub and Total variables in stress inerpolation scheme
  std::vector<RankTwoTensor> _rotensor;
  RankTwoTensor _sigma, _subsig;
  RankTwoTensor _PK2, _subPK2;
  RankTwoTensor _matrot, _CrysRot;
  RankTwoTensor _pdfgrd, _subpdfgrd;
  RankTwoTensor _ee, _subee;
  RankTwoTensor _eqv_slip_incr, _sub_eqv_slip_incr;
  std::vector<Real> _slprsis, _subslprsis;
  ElasticityTensorR4 _Jacobian, _subJacobian;
  std::vector<Real> _sub_ework; // sub elastic work
  std::vector<Real> _sub_phi; // sub plastic potential
  Real _eWork; // total elastic work
  Real _Phi; // total plastic potential

  /// Slip system and elasticity variables
  std::vector<Real> _sd;
  std::vector<Real> _sn;
  std::vector<std::vector<Real> > _mo_rotated;
  std::vector<std::vector<Real> > _no_rotated;
  std::vector<std::vector<RankTwoTensor> > _s0_rotated;
  std::vector<ElasticityTensorR4> _C_rotated;

  /// Plastic potential parameters
  std::vector<std::vector<std::vector<Real> > > _alpha_ab; // latent hardening matrix
  Real _tao0; /// reference shear stress
  Real _gamma0; /// reference slip

  /// Unit conversion parameters
  Real _time_scale;
  Real _length_scale;
  Real _pressure_scale;
  const Real _JtoeV;

  /// Identity tensor
  RankTwoTensor _iden;

  /// Phase field related variables
  std::vector<VariableValue *> _vals;
  const GrainTracker & _grain_tracker;
  unsigned int _grain_num;
  unsigned int _nop;

  /// Material Properties
  MaterialProperty<std::vector<Real> > & _dgss;
  MaterialProperty<std::vector<Real> > & _dgss_old;
  MaterialProperty<std::vector<std::vector<Real> > >  & _gamma;
  MaterialProperty<std::vector<std::vector<Real> > >  & _gamma_old;
  MaterialProperty<std::vector<unsigned int> > & _grain_id_container;
  MaterialProperty<std::vector<unsigned int> > & _grain_id_container_old;
  std::vector<MaterialProperty<Real> *> _edf; // elastic driving force
  std::vector<MaterialProperty<Real> *> _pdf; // plastic driving force
  MaterialProperty<Real> & _elastic_potential;
  MaterialProperty<Real> & _plastic_potential;
  std::vector< MaterialProperty<Real> *> _sub_ppot;
};

#endif //COUPLEDPFCRYSPLASTMATERIAL_H
