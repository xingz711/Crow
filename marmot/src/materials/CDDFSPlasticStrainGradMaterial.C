#include "CDDFSPlasticStrainGradMaterial.h"
#include "MooseRandom.h"
#include "MathUtils.h"

#include "libmesh/petsc_macro.h"
#include <petscsys.h>
#include <petscblaslapack.h>

#if !defined(LIBMESH_HAVE_PETSC)
  extern "C" void FORTRAN_CALL(dsyev) ( ... );
#endif

template<>
InputParameters validParams<CDDFSPlasticStrainGradMaterial>()
{
  InputParameters params = validParams<FiniteStrainMaterial>();
  params.addRequiredParam<unsigned int>("nss","Properties");
  params.addRequiredParam< std::vector<Real> >("flowprops","Properties");
  params.addRequiredParam<std::string>("slip_sys_file_name", "Name of the file containing the slip system");
  params.addParam<std::string>("euler_angle_file_name","", "Name of the file containing the euler angles");
  params.addParam<Real>("rtol",1e-8,"Constitutive stress residue tolerance");
  params.addParam<Real>("gtol",1e2,"Constitutive gss residue tolerance");
  params.addParam<Real>("slip_incr_tol",2e-2,"Constitutive gss residue tolerance");
  params.addParam<Real>("b", 2.54e-10, "Burger's vector magnitude");
  params.addParam<Real>("mu", 80.0e9, "Shear modulus");
  params.addParam<Real>("nu", 0.3, "Poisson's ratio");
  params.addParam<Real>("a", 0.4, "Baily-Hirsch hardening coefficent");
  params.addParam<Real>("beta", 0.0, "beta- irradiation hardening pre-factor");
  params.addParam<Real>("Rc", 15.0, "R_c Critical radius for annihiliation in unit of b");
  params.addParam<Real>("rhoDL_0", 1.0e24, "Initial disloction-loop density ");
  params.addParam<Real>("dl_0", 5.0e-9, "Initial disloction-loop size");
  params.addParam<Real>("tau0", 0.0, "Initial internal friction");
  params.addParam<Real>("rhoM_0", 0.0, "Initial Total disloction densities: Mobile (RhoM)");
  params.addParam<Real>("rhoI_0", 0.0, "Initial Total disloction densities: Immobile(RhoI)");
  params.addParam<Real>("ani", 1.0, "Anisotropic Factor");
  params.addRequiredParam< std::vector<Real> >("alphas","List of Alpha coefficeints");
  params.addParam<Real>("A_ijklmn", 1.0e6, "Couple-Stress Elasticity Tensor coefficients");
  params.addParam<Real>("alpha_sg", 1.0, "Fitting parameter for the GND dislocation calculation");
  return params;
}

CDDFSPlasticStrainGradMaterial::CDDFSPlasticStrainGradMaterial(const InputParameters & parameters) :
    FiniteStrainMaterial(parameters),
    _nss(getParam<unsigned int>("nss")),
    _flowprops(getParam<std::vector<Real> >("flowprops")),
    _slip_sys_file_name(getParam<std::string>("slip_sys_file_name")),
    _euler_angle_file_name(getParam<FileName>("euler_angle_file_name")),
    _rtol(getParam<Real>("rtol")),
    _gtol(getParam<Real>("gtol")),
    _slip_incr_tol(getParam<Real>("slip_incr_tol")),
    _fp(declareProperty<RankTwoTensor>("fp")),
    _fp_old(declarePropertyOld<RankTwoTensor>("fp")),
    _pk2(declareProperty<RankTwoTensor>("pk2")),
    _pk2_old(declarePropertyOld<RankTwoTensor>("pk2")),
    _lag_e(declareProperty<RankTwoTensor>("lage")),
    _gss(declareProperty<std::vector<Real> >("gss")),
    _gss_old(declarePropertyOld<std::vector<Real> >("gss")),
    _acc_slip(declareProperty<Real>("acc_slip")),
    _acc_slip_old(declarePropertyOld<Real>("acc_slip")),
    _update_rot(declareProperty<RankTwoTensor>("update_rot")),
    _crysrot(declareProperty<RankTwoTensor>("crysrot")),
    _crysrot_old(declarePropertyOld<RankTwoTensor>("crysrot")),
    _b(getParam<Real>("b")),
    _mu(getParam<Real>("mu")),
    _nu(getParam<Real>("nu")),
    _a(getParam<Real>("a")),
    _beta(getParam<Real>("beta")),
    _Rc(getParam<Real>("Rc")),
    _rhoDL_0(getParam<Real>("rhoDL_0")),
    _dl_0(getParam<Real>("dl_0")),
    _tau0(getParam<Real>("tau0")),
    _rhoM_0(getParam<Real>("rhoM_0")),
    _rhoI_0(getParam<Real>("rhoI_0")),
    _ani(getParam<Real>("ani")),
    _alphas(getParam<std::vector<Real> >("alphas")),
    _rhoM(declareProperty<std::vector<Real> >("rhoM")),
    _rhoM_old(declarePropertyOld<std::vector<Real> >("rhoM")),
    _rhoI(declareProperty<std::vector<Real> >("rhoI")),
    _rhoI_old(declarePropertyOld<std::vector<Real> >("rhoI")),
    _total_density(declareProperty<Real>("total_density")),
    _tau_save(declareProperty<std::vector<Real> >("tau_save")),
    _slip_incr_save(declareProperty<std::vector<Real> >("slip_incr_save")),

    _l_inv(declareProperty<Real>("l_inv")),
    _rhoSG(declareProperty<Real>("rhoGND")),

    _couple_stress_x(declareProperty<RankTwoTensor>("couple_stress_x") ),
    _couple_stress_x_old(declarePropertyOld<RankTwoTensor>("couple_stress_x") ),
    _couple_stress_y(declareProperty<RankTwoTensor>("couple_stress_y") ),
    _couple_stress_y_old(declarePropertyOld<RankTwoTensor>("couple_stress_y") ),
    _couple_stress_z(declareProperty<RankTwoTensor>("couple_stress_z") ),
    _couple_stress_z_old(declarePropertyOld<RankTwoTensor>("couple_stress_z") ),
    _A_ijklmn(getParam<Real>("A_ijklmn")),
    _alpha_sg(getParam<Real>("alpha_sg")),

    _second_disp_x(coupledSecond("disp_x")),
    _second_disp_y(coupledSecond("disp_y")),
    _second_disp_z(_mesh.dimension() == 3 ? coupledSecond("disp_z") : _second_zero),
    _second_disp_x_old(_fe_problem.isTransient() ? coupledSecondOld("disp_x") : _second_zero),
    _second_disp_y_old(_fe_problem.isTransient() ? coupledSecondOld("disp_y") : _second_zero),
    _second_disp_z_old(_fe_problem.isTransient() && _mesh.dimension() == 3 ? coupledSecondOld("disp_z") : _second_zero),
    _SX(declareProperty<RankTwoTensor>("SX")),
    _SY(declareProperty<RankTwoTensor>("SY")),
    _SZ(declareProperty<RankTwoTensor>("SZ"))
{
}

void CDDFSPlasticStrainGradMaterial::initQpStatefulProperties()
{
  int ind,flag;
  RealTensorValue rot;

  _stress[_qp].zero();

  _fp[_qp].zero();
  _fp[_qp].addIa(1.0);

  _pk2[_qp].zero();

  // _console << "SAP>> Getting ready to start couple stress work" << '\n';
  _couple_stress_x[_qp].zero();
  _couple_stress_y[_qp].zero();
  _couple_stress_z[_qp].zero();
  _couple_stress_x_old[_qp].zero();
  _couple_stress_y_old[_qp].zero();
  _couple_stress_z_old[_qp].zero();

  _gss[_qp].resize(_nss);
  _gss_old[_qp].resize(_nss);

  _rhoM[_qp].resize(_nss);
  _rhoM_old[_qp].resize(_nss);

  _rhoI[_qp].resize(_nss);
  _rhoI_old[_qp].resize(_nss);

  for (unsigned int i = 0; i < _nss; ++i)
  {
    _gss[_qp][i] = _tau0; // 0.0;
    _gss_old[_qp][i] = _tau0; // 0.0;
    _rhoM[_qp][i] = _rhoM_0/_nss;
    _rhoM_old[_qp][i] = _rhoM_0/_nss;
    _rhoI[_qp][i] = _rhoI_0/_nss;
    _rhoI_old[_qp][i] = _rhoI_0/_nss;
  }

  if (_nss==24 && _ani!=1.0)
  {
    // we have BCC, need to deal with anisotropic factor
    // for (unsigned int i=0; i<12; ++i)
    for (unsigned int i=12; i<24; ++i)
    {
      _gss[_qp][i] *= _ani;
      _gss_old[_qp][i] *= _ani;
    }
  }

  _acc_slip[_qp] = 0.0;

  _a0.resize(_nss);
  _xm.resize(_nss);

  flag = 0;
  ind = -1;
  while (flag == 0)
  {
    ind = ind + 1;
    unsigned int is = static_cast<unsigned int>(_flowprops[ind]);
    ind = ind + 1;
    unsigned int ie = static_cast<unsigned int>(_flowprops[ind]);

    if (ie == _nss)
      flag=1;

    for (unsigned int i=is; i<=ie; i++)
    {
      _a0[i-1] = _flowprops[ind+1];
      _xm[i-1] = _flowprops[ind+2];
    }

    ind = ind + 2;
  }

  // new alpha coefficeints
  _alpha_1 = _alphas[0];
  _alpha_2 = _alphas[1];
  _alpha_3 = _alphas[2];
  _alpha_4 = _alphas[3];
  _alpha_5 = _alphas[4];
  _alpha_6 = _alphas[5];

  // convert a0 to v0
  _v0 = _a0[0];

  get_euler_ang();
  get_euler_rot();

  _mo.resize(_nss*3);
  _no.resize(_nss*3);

  get_slip_sys();

  for (int i=0;i<3;i++)
    for (int j=0;j<3;j++)
      rot(i,j) = _crysrot[_qp](i,j);

  _elasticity_tensor[_qp].rotate(rot);

  _update_rot[_qp] = _crysrot[_qp];

  _total_density[_qp] = 0.0;
  _tau_save[_qp].resize(_nss);
  _slip_incr_save[_qp].resize(_nss);
  for (unsigned int i=0; i<_nss; i++)
  {
    _tau_save[_qp][i] = 0.0;
    _slip_incr_save[_qp][i] = 0.0;
  }

  // SAP>> Initialized the print-out values for the mean free glide path and GNDs
  _l_inv[_qp] = 0.0;
  _rhoSG[_qp] = 0.0;

}

void CDDFSPlasticStrainGradMaterial::computeQpStress()
{

  std::vector<Real> gss_prev(_nss), slip_incr(_nss), tau(_nss);
  RankTwoTensor pk2, fp_old_inv, fp_inv, resid, dpk2, sig, rot;
  Real gmax,gdiff,rnorm;
  RankFourTensor jac;
  Real slip_incr_max,fac;
  int iter,iterg,maxiter,maxiterg;
  Real tau_max, gss_max;

  maxiter = 100;
  maxiterg = 100;

  _crysrot[_qp] = _crysrot_old[_qp];

  fp_inv.zero();
  fp_old_inv = _fp_old[_qp].inverse();

  gmax = 1.1*_gtol;

  for (unsigned int i=0; i<_nss; i++)
  {
    _gss[_qp][i] = _gss_old[_qp][i];
    _rhoM[_qp][i] = _rhoM_old[_qp][i];
    _rhoI[_qp][i] = _rhoI_old[_qp][i];
  }


  iterg = 0;
  while (gmax > _gtol && iterg < maxiterg)
  {
    iter = 0;

    pk2 = _pk2_old[_qp];

    calc_resid_jacob(pk2, sig, fp_old_inv, fp_inv, slip_incr, tau, resid, jac);

    slip_incr_max = 0.0;
    tau_max = 0.0;
    gss_max = 0.0;

    for (unsigned int i=0; i<_nss; i++)
    {
      if (fabs(slip_incr[i])>slip_incr_max)
        slip_incr_max = fabs(slip_incr[i]);

      if (std::abs(tau[i])>tau_max)
        tau_max = std::abs(tau[i]);

      if (std::abs(_gss[_qp][i])>gss_max)
        gss_max = std::abs(_gss[_qp][i]);
    }

    fac = 1.0;
    if (slip_incr_max > _slip_incr_tol)
      mooseError("Slip increment exceeds tolerance, slip_incr=" << _current_elem->id() << " " << _qp << " " << slip_incr_max);

    rnorm = resid.L2norm();

    while (rnorm > _rtol && iter <  maxiter)
    {
      dpk2 = jac.invSymm()*resid;

      pk2 = pk2 - dpk2*fac;

      calc_resid_jacob(pk2, sig, fp_old_inv, fp_inv, slip_incr, tau, resid, jac);

      slip_incr_max = 0.0;

      for (unsigned int i=0; i<_nss; i++)
      {
        if (fabs(slip_incr[i])>slip_incr_max)
          slip_incr_max=fabs(slip_incr[i]);

        if (std::abs(tau[i])>tau_max)
          tau_max=std::abs(tau[i]);

        if (std::abs(_gss[_qp][i])>gss_max)
          gss_max=std::abs(_gss[_qp][i]);
      }

      fac = 1.0;
      if (slip_incr_max > _slip_incr_tol)
        mooseError("Slip increment exceeds tolerance, slip_incr=" << _current_elem->id() << " " << _qp << " " << slip_incr_max);

      rnorm = resid.L2norm();
      iter += 1;
    }

    if (iter==maxiter)
      mooseError("Stress Integration error \n");

    for (unsigned int i=0; i<_nss; i++)
      gss_prev[i]=_gss[_qp][i];

    // update_gss(&slip_incr[0]);
    // update_gss(slip_incr);
    update_gss_tau(slip_incr, tau);

    gmax = 0.0;
    for (unsigned int i=0; i<_nss; i++)
    {
      gdiff=fabs(gss_prev[i]-_gss[_qp][i]);

      if (gdiff > gmax)
        gmax = gdiff;
    }
    iterg += 1;
  }

  if (iterg == maxiterg)
    mooseError("gmax=" << gmax << "\n" << "Hardness Integration error \n");

  _fp[_qp] = fp_inv.inverse();
  _pk2[_qp] = pk2;
  _stress[_qp] = sig;

  rot = getmatrot(_deformation_gradient[_qp]);

  _update_rot[_qp] = rot*_crysrot[_qp];

  _acc_slip[_qp] = _acc_slip_old[_qp];

  for (unsigned int i=0; i<_nss; i++)
  {
    _tau_save[_qp][i] = tau[i];
    _acc_slip[_qp] = _acc_slip[_qp]+(std::abs(_slip_incr_save[_qp][i])+std::abs(slip_incr[i]))/2.0;
    _slip_incr_save[_qp][i] = slip_incr[i];
  }

  // Call to calculate the couple stress as the linear elastic work conjugate to the _second_disp tensors
  // TODO:  Move to a separate method called from within ComputeQpStress
  _couple_stress_x[_qp] = _A_ijklmn * _second_disp_x[_qp];
  _couple_stress_y[_qp] = _A_ijklmn * _second_disp_y[_qp];
  _couple_stress_z[_qp] = _A_ijklmn * _second_disp_z[_qp];
}


void
CDDFSPlasticStrainGradMaterial::get_euler_rot()
{
  Real phi1, phi, phi2;
  Real cp, cp1, cp2, sp, sp1, sp2;
  RankTwoTensor RT;
  Real pi = libMesh::pi;

  phi1 = _Euler_angles(0) * (pi/180.0);
  phi =  _Euler_angles(1) * (pi/180.0);
  phi2 = _Euler_angles(2) * (pi/180.0);

  cp1 = std::cos(phi1);
  cp2 = std::cos(phi2);
  cp = std::cos(phi);

  sp1 = std::sin(phi1);
  sp2 = std::sin(phi2);
  sp = std::sin(phi);

  RT(0,0) = cp1 * cp2 - sp1 * sp2 * cp;
  RT(0,1) = sp1 * cp2 + cp1 * sp2 * cp;
  RT(0,2) = sp2 * sp;
  RT(1,0) = -cp1 * sp2 - sp1 * cp2 * cp;
  RT(1,1) = -sp1 * sp2 + cp1 * cp2 * cp;
  RT(1,2) = cp2 * sp;
  RT(2,0) = sp1 * sp;
  RT(2,1) = -cp1 * sp;
  RT(2,2) = cp;

  _crysrot[_qp] = RT.transpose();
  _crysrot_old[_qp] = _crysrot[_qp];
}

void
CDDFSPlasticStrainGradMaterial::get_slip_sys()
{
  std::vector<Real> sd(3*_nss), sn(3*_nss);
  Real vec[3];
  std::ifstream fileslipsys;

  fileslipsys.open(_slip_sys_file_name.c_str());

  if (!fileslipsys)
    mooseError("Can't open slip system input file ");

  for (unsigned int i=0; i<_nss; i++)
  {
    for (unsigned int j=0; j<3; j++)
      fileslipsys >> vec[j];

    Real mag;
    mag = std::pow(vec[0], 2.0) + std::pow(vec[1], 2.0) + pow(vec[2], 2.0);
    mag = std::pow(mag, 0.5);

    for (unsigned int j=0; j<3; ++j)
      sn[i*3+j] = vec[j] / mag;
  }

  for (unsigned int i=0; i<_nss; i++)
  {
    for (unsigned int j=0; j<3; j++)
      fileslipsys >> vec[j];

    Real mag;
    mag = pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2);
    mag = pow(mag,0.5);

    for (unsigned int j=0; j<3; j++)
      sd[i*3+j] = vec[j]/mag;
  }

  fileslipsys.close();

  for (unsigned int i=0; i<_nss; i++)
  {
    for (unsigned int j=0; j<3; j++)
    {
      _mo[i*3+j] = 0.0;
      for (unsigned int k=0; k<3; k++)
        _mo[i*3+j] = _mo[i*3+j] + _crysrot[_qp](j,k)*sd[i*3+k];
    }

    for (unsigned int j=0; j<3; j++)
    {
      _no[i*3+j] = 0.0;
      for (unsigned int k=0; k<3; k++)
        _no[i*3+j] = _no[i*3+j] + _crysrot[_qp](j,k)*sn[i*3+k];
    }
  }
}

void
CDDFSPlasticStrainGradMaterial::update_gss_tau(std::vector<Real> & /*slip_incr*/,
                                               std::vector<Real> & tau)
{
  DenseMatrix<Real> Omega(_nss, _nss), CRM(_nss, _nss);
  Real rhoM_rate = 0.0;
  Real rhoI_rate = 0.0;
  Real v_g;
  Real term1 = 0.0;
  Real term2 = 0.0;
  Real term3 = 0.0;
  Real term4 = 0.0;
  Real term5 = 0.0;
  Real term6 = 0.0;
  Real tau_sr = 0.0;
  Real tau_ir = 0.0;
  Real rhoM_total = 0.0;

  // For calculating CRM
  int nc, ii, jj, jstart;
  Real carlo;
  std::vector<Real> prob(_nss), cprob(_nss);
  unsigned int num_burgers_vecs, num_burgers_sp;

  // Calculting CRM
  if (_nss == 12) // FCCupdate_gss_tau1
  {
    num_burgers_vecs = 6;  // NBS
    num_burgers_sp = 2;    // NCS
  }
  else if (_nss == 24) // BCC1
  {
    num_burgers_vecs = 4;  // NBS
    num_burgers_sp = 6;    // NCS
  }
  else // BCC2
  {
    num_burgers_vecs = 4;  // NBS
    num_burgers_sp = 12;   // NCS
  }

  // compute probability of cross-slip for every slip system
  Real tauCR = 5.0; // 5e6;
  for (unsigned int i = 0; i < _nss; ++i)
    // prob[i] = std::exp((std::abs(tau[i]) - tauCR)/1e6);
    prob[i] = std::exp((std::abs(tau[i]) - tauCR)/1.0);

  // decide on cross-slip between slip systems sharing the same Burgers vectors
  for (unsigned int m = 0; m < num_burgers_vecs; ++m)
  {
    // compute cummulitive probablitiy distribution function for slip systems
    // having the same Burgers vector
    jstart = m * num_burgers_sp;
    // cprob[1] = 0.0;
    cprob[0] = 0.0;
    Real totalProb = 0.0;

    // for (int j = 0; j < num_burgers_sp + 1; ++j)
    for (unsigned int j=1; j<num_burgers_sp+1; ++j)
    {
      jj = jstart+j-1;
      cprob[j] = cprob[j-1]+prob[jj];
      totalProb += prob[jj];
    }

    // normalize
    for (unsigned int j = 0; j < num_burgers_sp + 1; ++j)
      cprob[j] /= cprob[num_burgers_sp];

    // Now, pick a system i with dislocation type m and decide if this
    // m will cross-slip, and do this for all slip planes of each burgers vector
    for (unsigned int i=0; i<num_burgers_sp; ++i)
    {
      // srand(176154321^m + int(time(NULL))^i);
      carlo = MooseRandom::rand();
      // decide on the cross slip
      // NEWER VERION
      // int set = 0;
      for (unsigned int jj=0; jj<num_burgers_sp; ++jj)
      {
        if (carlo>=cprob[jj] && carlo<=cprob[jj+1])
          {
            if (jj == i)
              { // NO Cross Slip
                ii = jstart+i;
                CRM(ii,ii) = 0.0;
                // set = 1;
                break;
              }
            else
              { // i cross slips to jj
                ii = jstart+i;
                nc = jstart+jj;
                CRM(ii,ii) = -1.0;
                CRM(nc,ii) = -CRM(ii,ii);
                // set = 1;
                break;
              }
          }
      }
    }
  }

  // Make Omega - Model 1
  for (unsigned int i = 0; i < _nss; ++i)
  {
    for (unsigned int j = 0; j < _nss; ++j)
    {
      if (i==j)
        Omega(i,j) = 1.0;
      else
        Omega(i,j) = 0.2;
    }
  }


  // Calculate the mean free glide path:
  _rhoSG[_qp] = 0.0;
  _l_inv[_qp] = 0.0; // SAP>>  Leave the 'initialization' of l_inverse here even when defining as a global variable
  //  _console << "SAP>> Calling the new calculate the inverse mean free glide path length method" << '\n';
  calc_l_inverse();

  for (unsigned int i = 0; i < _nss; ++i)
  {
    // add everything up to get new hardness (_gss=tauSR)
    tau_sr = 0.0;
    for (unsigned int j=0; j<_nss; j++)
      tau_sr = tau_sr + Omega(i,j) * (_rhoM[_qp][j] + _rhoI[_qp][j]);
    tau_sr = _a * _b * _mu * sqrt(std::abs(tau_sr));

    // TODO this will need updating when including irradiation
    tau_ir = _beta * _b * _mu * sqrt(_rhoDL_0*_dl_0);

    Real factor = 1.0;
    // for BCC
    if (_nss == 24)
    {
      // if (i < 12)
      if (i > 11)
        factor = _ani;
    }
    _gss[_qp][i] = _tau0*factor + tau_sr + tau_ir;
  }


  for (unsigned int i=0; i<_nss; ++i)
  {
    // calc v_g
    v_g = 0.0;
    Real signT = 1.0;
    if (tau[i] != 0.0) signT = tau[i]/std::abs(tau[i]);
    // EIB - this is tauT from CDD
    // Real vtol = 1.0e-6;
    Real factor = 1.0;
    if (i>11)
      factor = _ani;
    // if (std::abs(tau[i]/_gss[_qp][i]) >= 1.0 )
    if (std::abs(tau[i]/(_tau0*factor)) >= 1.0)
      v_g = _v0*pow(std::abs(tau[i])/_gss[_qp][i],(1.0/_xm[i])) * signT;

    Real RoV = _rhoM[_qp][i] * v_g;

    // calc each dislocation density term
    // Term 1: alpha1 * _rhoM * |vg|/|l|
    // term1 = _alpha_1 * _rhoM[_qp][i] * std::abs(v_g) * _l_inv[_qp];
    term1 = _alpha_1 * std::abs(RoV) * _l_inv[_qp];

    // Term 2: 2 * alpha2 * Rc * _rhoM^2 * |vg|
    // term2 = 2.0 * _alpha_2 * _Rc * _rhoM[_qp][i] * _rhoM[_qp][i] * std::abs(v_g);
    term2 = 2.0 * _alpha_2 * (_Rc*_b) * _rhoM[_qp][i] * std::abs(RoV);

    // Tern 3: alpha3 * _rhoM * |vg|/|l|
    // term3 = _alpha_3 * _rhoM[_qp][i] * std::abs(v_g) * _l_inv[_qp];
    term3 = _alpha_3 * std::abs(RoV) * _l_inv[_qp];

    // Term 4: alpha4 * (|tau|/tau*)^r * _rhoI * |vg|/|l|
    term4 = 0.0;
    // EIB - current version of CDD that I have didn't implement this, even though in model definition
    // if (_gss[_qp][i] > _tau0*factor)
    //  term4 = _alpha_4 * sqrt(std::abs(tau[i])/(_gss[_qp][i]-_tau0)) * _rhoI[_qp][i] * v_g * _l_inv[_qp];
    term4 = _alpha_4 * sqrt(std::abs(tau[i])/(_gss[_qp][i])) * _rhoI[_qp][i] * std::abs(v_g) * _l_inv[_qp];
    // }

    // TODO the 1.5 is really r - needs to be added later

    // Term 5: alpha5 * Sum_b ProbMtx(ab)*_rhoM(b)*|vg(a)|/|l|
    term5 = 0.0;
    for (unsigned int j=0; j<_nss; j++)
      term5 = term5 + _alpha_5 * CRM(i,j) * _rhoM[_qp][j] * std::abs(v_g) * _l_inv[_qp];

    // Term 6: alpha6 * Rc * _rhoI * _rhoM * |vg|
    // term6 = _alpha_6 * _Rc * _rhoI[_qp][i] * _rhoM[_qp][i] * std::abs(v_g);
    term6 = _alpha_6 * (_Rc*_b) * _rhoI[_qp][i] * std::abs(RoV);

    // add them up to for _rhoM_rate and _rhoI_rate
    rhoM_rate = term1 - term2 - term3 + term4 + term5 - term6;
    rhoI_rate = term3 - term4 - term6;

    // intergrate to get new _rhoM and _rhoI
    _rhoM[_qp][i] = _rhoM[_qp][i] + rhoM_rate*_dt;
    _rhoI[_qp][i] = _rhoI[_qp][i] + rhoI_rate*_dt;

    rhoM_total += _rhoM[_qp][i] + _rhoI[_qp][i];
  }

  _total_density[_qp] = 0.0;
  for (unsigned int i=0; i<_nss; ++i)
  {
    // add everything up to get new hardness (g=tauSR)
    tau_sr = 0.0;
    for (unsigned int j=0; j<_nss; j++)
      tau_sr = tau_sr + Omega(i,j) * (_rhoM[_qp][j]+_rhoI[_qp][j]);

    tau_sr = _a * _b * _mu * sqrt(std::abs(tau_sr));
    // TODO this will need updating when including irradiation
    tau_ir = _beta * _b * _mu * sqrt(_rhoDL_0*_dl_0);

    if ( i > 11 )
      _gss[_qp][i] = (_tau0*_ani + tau_sr + tau_ir);
    else
      _gss[_qp][i] = (_tau0 + tau_sr + tau_ir);

    _total_density[_qp] = _total_density[_qp] + (_rhoM[_qp][i] + _rhoI[_qp][i]);
  }

}


void
CDDFSPlasticStrainGradMaterial::calc_resid_jacob(RankTwoTensor & pk2,
                                                 RankTwoTensor & sig,
                                                 RankTwoTensor & fp_old_inv,
                                                 RankTwoTensor & fp_inv,
                                                 std::vector<Real> & slip_incr,
                                                 std::vector<Real> & tau,
                                                 RankTwoTensor & resid,
                                                 RankFourTensor & jac)
{

  RankTwoTensor fe,ce,ee,iden,ce_pk2;
  RankTwoTensor eqv_slip_incr,pk2_new;
  std::vector<RankTwoTensor> s0(_nss), dtaudpk2(_nss), dfpinvdslip(_nss);
  RankTwoTensor temp2;
  std::vector<Real> dslipdtau(_nss);
  RankFourTensor dfedfpinv,deedfe,dfpinvdpk2;
  RankFourTensor temp4;

  // Calculate Residual
  iden.zero();
  iden.addIa(1.0);

  fe = _deformation_gradient[_qp] * fp_old_inv;

  ce = fe.transpose() * fe;
  ee = ce - iden;
  ee *= 0.5;

  ce_pk2 = ce * pk2;
  ce_pk2 = ce_pk2 / fe.det();
  //  ce_pk2 = pk2; // Approximation

  for (unsigned int i=0; i<_nss; i++)
  {
    for (unsigned int j=0; j<3; j++)
      for (unsigned int k=0; k<3; k++)
        s0[i](j,k) = _mo[i*3+j] * _no[i*3+k];

    tau[i] = ce_pk2.doubleContraction(s0[i]);
  }

  // Calculate dslip, dslipdtau
  get_slip_incr(tau, slip_incr, dslipdtau);

  eqv_slip_incr.zero();
  for (unsigned int i=0; i<_nss; i++)
    eqv_slip_incr += s0[i]*slip_incr[i];

  eqv_slip_incr = iden - eqv_slip_incr;
  fp_inv = fp_old_inv * eqv_slip_incr;
  fe = _deformation_gradient[_qp] * fp_inv;

  ce = fe.transpose() * fe;
  ee = ce - iden;
  ee *= 0.5;

  pk2_new = _elasticity_tensor[_qp] * ee;

  _lag_e[_qp] = _deformation_gradient[_qp].transpose() * _deformation_gradient[_qp] - iden;
  _lag_e[_qp] = _lag_e[_qp] * 0.5;

  resid = pk2 - pk2_new;
  /*End Calculate Residual*/
  /*Calculate Jacobian*/

  for (unsigned int i=0; i<_nss; i++)
  {
    dtaudpk2[i] = s0[i];
    dfpinvdslip[i] = -fp_old_inv * s0[i];
  }

  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      for (int k=0; k<3; k++)
        dfedfpinv(i,j,k,j) = _deformation_gradient[_qp](i,k);

  deedfe.zero();

  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      for (int k=0; k<3; k++)
      {
        deedfe(i,j,k,i) = deedfe(i,j,k,i) + fe(k,j)*0.5;
        deedfe(i,j,k,j) = deedfe(i,j,k,j) + fe(k,i)*0.5;
      }

  dfpinvdpk2.zero();
  for (unsigned int i=0; i<_nss; i++)
  {
    temp2 = dfpinvdslip[i]*dslipdtau[i];
    temp4 = temp2.outerProduct(dtaudpk2[i]);
    dfpinvdpk2 += temp4;
  }

  jac = _elasticity_tensor[_qp]*deedfe*dfedfpinv*dfpinvdpk2;
  jac = RankFourTensor::Identity() - jac;

  /*End Calculate Jacobian*/

  sig = fe * pk2 * fe.transpose();
  sig = sig/fe.det();
}

void
CDDFSPlasticStrainGradMaterial::get_slip_incr(std::vector<Real> & tau,
                                              std::vector<Real> & slip_incr,
                                              std::vector<Real> & dslipdtau)
{
  for (unsigned int i=0; i<_nss; i++)
    slip_incr[i] = _rhoM[_qp][i] * _b * _v0 * std::pow(std::abs(tau[i])/_gss[_qp][i],(1/_xm[0])) * MathUtils::sign(tau[i]) * _dt;

  for (unsigned int i=0; i<_nss; i++)
    dslipdtau[i] = _rhoM[_qp][i] * _b *_v0 / _xm[i] * std::pow(std::abs(tau[i])/_gss[_qp][i],(1.0/_xm[0]-1.0))/_gss[_qp][i]*_dt;
}

RankTwoTensor CDDFSPlasticStrainGradMaterial::getmatrot(RankTwoTensor &a)
{
  RankTwoTensor rot;
  RankTwoTensor c, diag, evec;
  double cmat[3][3], w[3], work[10];
  int info;
  int nd = 3;
  int lwork = 10;

  c = a.transpose()*a;

  for (int i=0;i<3;i++)
    for (int j=0;j<3;j++)
      cmat[i][j] = c(i,j);

#if !defined(LIBMESH_HAVE_PETSC)
  FORTRAN_CALL(dsyev)("V", "U", &nd, &cmat[0][0], &nd, w, work, &lwork, &info);
#else
  LAPACKsyev_("V", "U", &nd, &cmat[0][0], &nd, w, work, &lwork, &info);
#endif

  diag.zero();

  for (int i=0; i<3; i++)
    diag(i,i) = std::pow(w[i],0.5);

  for (unsigned int i=0; i<3; i++)
    for (unsigned int j=0; j<3; j++)
      evec(i,j) = cmat[i][j];

  rot = a * ((evec.transpose()*diag*evec).inverse());

  return rot;
}


void
CDDFSPlasticStrainGradMaterial::get_euler_ang()
{
  std::ifstream fileeuler;
  Real vec[3];
  int elemno;

  if (_euler_angle_file_name.length() != 0)
  {
    fileeuler.open(_euler_angle_file_name.c_str());

    if (!fileeuler)
      mooseError("Can't open euler angle input file ");
    else
    {
      while (!fileeuler.eof())
      {
        fileeuler >> elemno;
        for (unsigned int i=0; i<3; i++)
          fileeuler >> vec[i];

        if (static_cast<dof_id_type>(elemno-1) == _current_elem->id())
        {
          _Euler_angles(0) = vec[0];
          _Euler_angles(1) = vec[1];
          _Euler_angles(2) = vec[2];

          fileeuler.close();
          return;
        }
      }
      fileeuler.close();
    }
  }
}

void CDDFSPlasticStrainGradMaterial::computeQpElasticityTensor()
{
  // Fill in the matrix stiffness material property
  _elasticity_tensor[_qp] = _Cijkl;

  ElasticityTensorR4 tan_mod;
  RankTwoTensor fe,fp_inv,pk2fet,fepk2;
  RankFourTensor dfedf,deedfe,dsigdpk2dfe,temp;

  fp_inv=_fp[_qp].inverse();
  fe=_deformation_gradient[_qp]*fp_inv;

  for (unsigned int i=0; i<3; i++)
    for (unsigned int j=0; j<3; j++)
      for (unsigned int l=0; l<3; l++)
        dfedf(i,j,i,l) = fp_inv(l,j);

  for (unsigned int i=0; i<3; i++)
    for (unsigned int j=0; j<3; j++)
      for (unsigned int k=0; k<3; k++)
      {
        deedfe(i,j,k,i) = deedfe(i,j,k,i) + fe(k,j)*0.5;
        deedfe(i,j,k,j) = deedfe(i,j,k,j) + fe(k,i)*0.5;
      }


  for (unsigned int i=0; i<3; i++)
    for (unsigned int j=0; j<3; j++)
      for (unsigned int k=0; k<3; k++)
        for (unsigned int l=0; l<3; l++)
          temp(i,j,k,l) = fe(i,k) * fe(j,l);

  dsigdpk2dfe = temp * _elasticity_tensor[_qp] * deedfe;

  pk2fet = _pk2[_qp] * fe.transpose();
  fepk2 = fe * _pk2[_qp];

  for (unsigned int i=0; i<3; i++)
    for (unsigned int j=0; j<3; j++)
      for (unsigned int l=0; l<3; l++)
      {
        tan_mod(i,j,i,l) = tan_mod(i,j,i,l) + pk2fet(l,j);
        tan_mod(i,j,j,l) = tan_mod(i,j,j,l) + fepk2(i,l);
      }

  tan_mod += dsigdpk2dfe;

  Real je = fe.det();
  tan_mod /= je;

  _Jacobian_mult[_qp] = tan_mod;
}

void CDDFSPlasticStrainGradMaterial::calc_l_inverse()
{
  Real l_inv_sum = 0.0;
  RankTwoTensor sd_x, sd_y, sd_z;
  Real tensor_contraction1 = 0.0, tensor_contraction2 = 0., tensor_contraction3 = 0.0;

  for (unsigned int i=0; i<_nss; ++i)
    l_inv_sum = l_inv_sum + (_rhoM[_qp][i]) + (_rhoI[_qp][i]);

  sd_x = _second_disp_x[_qp];
  sd_y = _second_disp_y[_qp];
  sd_z = _second_disp_z[_qp];

  tensor_contraction1 = sd_x.doubleContraction(_second_disp_x[_qp]);
  tensor_contraction2 = sd_y.doubleContraction(_second_disp_y[_qp]);
  tensor_contraction3 = sd_z.doubleContraction(_second_disp_z[_qp]);

  // Now find the square root of the sum of the three scalar products:
  _rhoSG[_qp] = sqrt(tensor_contraction1 + tensor_contraction2 + tensor_contraction3) * _alpha_sg/_b;
  _l_inv[_qp] = sqrt(std::abs(l_inv_sum)); // + _rhoSG[_qp];
}
