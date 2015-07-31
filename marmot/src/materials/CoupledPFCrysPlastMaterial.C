#include "CoupledPFCrysPlastMaterial.h"
#include "RotationTensor.h"
#include "GrainTracker.h"
#include "MathUtils.h"

template<>
InputParameters validParams<CoupledPFCrysPlastMaterial>()
{
  InputParameters params = validParams<FiniteStrainCrystalPlasticity>();
  params.addRequiredParam<std::string>("Euler_angles_file_name", "Name of the file containing the Euler angles");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredParam<UserObjectName>("GrainTracker_object", "The GrainTracker UserObject to get values from.");
  params.addRequiredParam<unsigned int>("grain_num", "Number of initial grains that will be modeled");
  params.addParam<Real>("tao0",1,"1 MPa, reference resolved shear stress");
  params.addParam<Real>("gamma0",1,"reference slip value");
  params.addParam<Real>("time_scale", 1.0e-9, "ns, dimension of time in seconds");
  params.addParam<Real>("length_scale", 1.0e-9, "nm, dimension of length in meters");
  params.addParam<Real>("pressure_scale", 1.0e6, "MPa, pressure is in Pa");
  params.set<MooseEnum>("tan_mod_type") = "exact";
  return params;
}

CoupledPFCrysPlastMaterial::CoupledPFCrysPlastMaterial(const InputParameters & parameters) :
    FiniteStrainCrystalPlasticity(parameters),
    _Euler_angles_file_name(getParam<std::string>("Euler_angles_file_name")),
    _tao0(getParam<Real>("tao0")),
    _gamma0(getParam<Real>("gamma0")),
    _time_scale(getParam<Real>("time_scale")),
    _length_scale(getParam<Real>("length_scale")),
    _pressure_scale(getParam<Real>("pressure_scale")),
    _JtoeV(6.24150974e18), // Joule to eV conversion
    _grain_tracker(getUserObject<GrainTracker>("GrainTracker_object")),
    _grain_num(getParam<unsigned int>("grain_num")),
    _nop(coupledComponents("v")),
    _dgss(declareProperty<std::vector<Real> >("dgss")),
    _dgss_old(declarePropertyOld<std::vector<Real> >("dgss")),
    _gamma(declareProperty<std::vector<std::vector<Real> > >("slip_strain")),
    _gamma_old(declarePropertyOld<std::vector<std::vector<Real> > >("slip_strain")),
    _grain_id_container(declareProperty<std::vector<unsigned int> >("relative_grain_id_container")),
    _grain_id_container_old(declarePropertyOld<std::vector<unsigned int> >("relative_grain_id_container")),
    _elastic_potential(declareProperty<Real>("elastic_potential")),
    _plastic_potential(declareProperty<Real>("plastic_potential"))
{
  _iden.zero();
  _iden.addIa(1.0);

  // Initialize values for "grain" properties
  _C_rotated.resize(_grain_num);
  _rotensor.resize(_grain_num);
  _mo_rotated.resize(_grain_num);
  _no_rotated.resize(_grain_num);
  _s0_rotated.resize(_grain_num);

  for (unsigned int grn = 0; grn < _grain_num; ++grn) // initialize slip variables
  {
    _mo_rotated[grn].resize(LIBMESH_DIM*_nss);
    _no_rotated[grn].resize(LIBMESH_DIM*_nss);
    _s0_rotated[grn].resize(LIBMESH_DIM*_nss);
  }

  _sub_ppot.resize(_grain_num);

  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    std::string sub_ppot_name("sub_plastic_potential");
    std::stringstream out;
    out << grn;
    sub_ppot_name.append(out.str());

    _sub_ppot[grn] = &declareProperty<Real>(sub_ppot_name);
  }

  _slprsis.resize(_nss);
  _subslprsis.resize(_nss);
  _sd.resize(_nss*LIBMESH_DIM);
  _sn.resize(_nss*LIBMESH_DIM);
  _alpha_ab.resize(_grain_num);

  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    _alpha_ab[grn].resize(_nss);
    for (unsigned int i = 0; i < _nss; ++i)
      _alpha_ab[grn][i].resize(_nss);
  }

  // Initialize values for "order parameters" based properties
  _vals.resize(_nop);

  for (unsigned int op = 0; op < _nop; ++op)
    _vals[op] = &coupledValue("v", op);

  _edf.resize(_nop);
  _pdf.resize(_nop);

  for (unsigned int op = 0; op < _nop; ++op)
  {
    std::string edf_name("elastic_driving_force"),pdf_name("plastic_driving_force");
    std::stringstream out;
    out << op;
    edf_name.append(out.str());
    pdf_name.append(out.str());

    _edf[op] = &declareProperty<Real>(edf_name);
    _pdf[op] = &declareProperty<Real>(pdf_name);
  }

  getRotatedElasticityTensor();
  getRotatedSchmidTensor();
  getLatentHardeningProps();
}

void
CoupledPFCrysPlastMaterial::initAdditionalProps()
{
  //adjust flow rate to proper time scale
  for(unsigned int i = 0; i < _nss; ++i)
    _a0[i] *= _time_scale;

  _elastic_strain[_qp].zero();
  _elastic_strain_old[_qp].zero();

  _dgss[_qp].resize(_nss);
  _dgss_old[_qp].resize(_nss);

  for (unsigned i = 0; i < _nss; ++i)
  {
    _dgss[_qp][i]     = 0;
    _dgss_old[_qp][i] = 0;
  }
}

void
CoupledPFCrysPlastMaterial::initQpVariables()
{
  _Jacobian.zero();
  _CrysRot.zero();
  _sigma.zero();
  _PK2.zero();
  _pdfgrd.zero();
  _ee.zero();
  _eqv_slip_incr.zero();
  _sub_ework.clear();
  _sub_phi.clear();
  _eWork = 0;
  _Phi = 0;
  _gamma[_qp].clear();
  _grain_id_container[_qp].clear();

  for (unsigned int i = 0; i < _nss; ++i)
    _slprsis[i] = 0;

  for (unsigned int grn = 0; grn < _grain_num; ++grn)
    (*_sub_ppot[grn])[_qp] = 0;
}

void
CoupledPFCrysPlastMaterial::computeProperties()
{
  const std::vector<std::pair<unsigned int, unsigned int> > & active_ops = _grain_tracker.getElementalValues(_current_elem->id());

  computeStrain();
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    initQpVariables();

    unsigned int n_active_ops= active_ops.size();
    if (n_active_ops < 1 && _t_step > 0 )
      mooseError("No active order parameters");

    Real sum_h = 0.0;
    unsigned int add_grn(0);

    for (unsigned int op = 0; op<n_active_ops; ++op)
    {
      unsigned int grn_index = active_ops[op].first - 1;
      unsigned int op_index = active_ops[op].second;

      if ((*_vals[op_index])[_qp]<0.01)
        continue;

      preFSCPrun(grn_index);
      computeQpElasticityTensor();
      computeQpStress();

      if(_t == 0) // move this inilization to constructor later
        initSubPot(grn_index);
      else
        calcSubPot(grn_index); // grn_index should NOT change when grain grows and disappers.

      Real h = (1.0 + std::sin(libMesh::pi * ((*_vals[op_index])[_qp] - 0.5)))/2.0;

      // Add up Sub values of these properties
      _sigma         += _subsig*h;
      _PK2           += _subPK2*h;
      _ee            += _subee*h;
      _pdfgrd        += _subpdfgrd*h;
      _eqv_slip_incr += _sub_eqv_slip_incr*h;
      _CrysRot       += _rotensor[grn_index]*h;
      _eWork         += _sub_ework[add_grn]*h;
      _Phi           += _sub_phi[add_grn]*h;
      _Jacobian      += _subJacobian*h;

      for (unsigned int i = 0; i < _nss; ++i)
         _slprsis[i] += _subslprsis[i]*h;

      (*_sub_ppot[grn_index])[_qp] = _sub_phi[add_grn];
      sum_h += h;
      add_grn += 1;
    }
    if (add_grn < 1)
      mooseError("No revelant grains for current qp.");

    Real tol = 1.0e-10;
    if (sum_h < tol)
      sum_h = tol;

    postFSCPrun(sum_h);
    calcDrivingForce(sum_h);
  }
}

void
CoupledPFCrysPlastMaterial::initSubPot(unsigned int& grn_index)
{
  std::vector<Real> init(_nss);

  _gamma[_qp].push_back(init);
  _gamma_old[_qp].push_back(init);

  _grain_id_container[_qp].push_back(grn_index);
  _grain_id_container_old[_qp].push_back(grn_index);

  _sub_phi.push_back(0);
  _sub_ework.push_back(0);
}

void
CoupledPFCrysPlastMaterial::calcSubPot(unsigned int& grn_index)
{
  _grain_id_container[_qp].push_back(grn_index);
  _gamma[_qp].push_back(_slip_incr);
  unsigned int _current_grn = _gamma[_qp].size() - 1;
  unsigned int old_grn_size = _grain_id_container_old[_qp].size();

  for (unsigned int old_grn = 0; old_grn < old_grn_size; ++old_grn)
    if (grn_index == _grain_id_container_old[_qp][old_grn])
      for (unsigned int i = 0; i < _nss; ++i)
      {
        mooseAssert( _current_grn<_gamma[_qp].size(), "Cannot access this gamma entry");
        mooseAssert( old_grn<_gamma_old[_qp].size(), "Cannot access this gamma_old entry");
        mooseAssert( i<_gamma[_qp][_current_grn].size(), "Cannot access this gamma slip system entry");

        _gamma[_qp][_current_grn][i] += _gamma_old[_qp][old_grn][i];
      }

  Real pot(0);
  for (unsigned int i = 0; i < _nss; ++i)
    for (unsigned int j = i; j < _nss; ++j)
      pot += std::abs(_alpha_ab[grn_index][i][j] * _gamma[_qp][_current_grn][i] * _gamma[_qp][_current_grn][j]);
  pot /= (_gamma0*_gamma0);
  pot = std::pow(pot, 0.75);

  _sub_phi.push_back(pot*_tao0*_gamma0*2.0/3.0);

  Real elastic_work = 0.5*_pk2_tmp.doubleContraction(_subee);
  _sub_ework.push_back(elastic_work);
}

void
CoupledPFCrysPlastMaterial::calcDrivingForce(Real& sum_h)
{
  _elastic_potential[_qp] = _eWork / sum_h; // MPa
  _plastic_potential[_qp] = _Phi   / sum_h; // MPa

  for (unsigned int op = 0; op < _nop; ++op)
  {
    (*_edf[op])[_qp] = 0.0;
    (*_pdf[op])[_qp] = 0.0;
  }

  const std::vector<std::pair<unsigned int, unsigned int> > & active_ops = _grain_tracker.getElementalValues(_current_elem->id());

  unsigned int n_active_ops= active_ops.size();
  if (n_active_ops < 1 && _t_step > 0 )
    mooseError("No active order parameters");

  unsigned int new_grn_size = _grain_id_container[_qp].size();
  Real sub_p, sub_e;

  for (unsigned int op = 0; op < n_active_ops; ++op)
  {
    unsigned int grn_index = active_ops[op].first - 1;
    unsigned int op_index = active_ops[op].second;
    Real dhdopi = libMesh::pi*std::cos(libMesh::pi*((*_vals[op_index])[_qp] - 0.5))/2.0;

    for (unsigned int new_grn = 0; new_grn < new_grn_size; ++new_grn)
      if (grn_index == _grain_id_container[_qp][new_grn])
      {
        sub_e = _sub_ework[new_grn];
        sub_p = _sub_phi[new_grn];
      }

    sub_e -= _elastic_potential[_qp];
    sub_p -= _plastic_potential[_qp];
    sub_e *= dhdopi/sum_h;
    sub_p *= dhdopi/sum_h;

    // Convert from XPa to eV/(xm)^3, where X is pressure scale and x is length scale;
    sub_e *= _JtoeV*(_length_scale*_length_scale*_length_scale)*_pressure_scale;
    sub_p *= _JtoeV*(_length_scale*_length_scale*_length_scale)*_pressure_scale;

    (*_edf[op_index])[_qp] = sub_e;
    (*_pdf[op_index])[_qp] = sub_p;
  }
}

void
CoupledPFCrysPlastMaterial::preFSCPrun(unsigned int& grn_index) // set elasticity tensor and slip system for each op
{
  _fp_old_inv = _fp_old[_qp].inverse();
  _fp_inv = _fp_old_inv;
  _elasticity_tensor[_qp] = _C_rotated[grn_index];
  _s0 = _s0_rotated[grn_index];
}

void
CoupledPFCrysPlastMaterial::preSolveQp()
{
  _dfgrd_tmp = _deformation_gradient[_qp];
}

void
CoupledPFCrysPlastMaterial::preSolveStatevar()
{
  for (unsigned i = 0; i < _nss; ++i)
    _gss_tmp[i]=_gss_old[_qp][i] + _dgss_old[_qp][i];

  _accslip_tmp_old = _acc_slip_old[_qp];
}

void
CoupledPFCrysPlastMaterial::solveStatevar()
{
  Real gmax, gdiff;
  unsigned int iterg;
  std::vector<Real> gss_prev(_nss);

  gmax = 1.1 * _gtol;
  iterg = 0;

  while (gmax > _gtol && iterg < _maxiterg) // Check for slip system resistance update tolerance
  {
    preSolveStress();
    solveStress();
    postSolveStress();

    for (unsigned i = 0; i < _nss; ++i)
      gss_prev[i] = _gss_tmp[i];

    update_slip_system_resistance(); // Update slip system resistance

    gmax = 0.0;
    for (unsigned i = 0; i < _nss; ++i)
    {
      gdiff = std::abs(gss_prev[i] - _gss_tmp[i]); // Calculate increment size

      if (gdiff > gmax)
        gmax = gdiff;
    }

    iterg++;

  }

  if (iterg == _maxiterg)
    mooseError("FiniteStrainCrystalPLasticity: Hardness Integration error gmax" << gmax << "\n");

  Real slip_incr_max= 0.0;
  for (unsigned i = 0; i < _nss; ++i)
    if (std::abs(_slip_incr[i]) > slip_incr_max)
        slip_incr_max = std::abs(_slip_incr[i]);

  if (slip_incr_max > _slip_incr_tol)
    mooseError("FiniteStrainCrystalPLasticity: Slip increment exceeds tolerance - Element number" << _current_elem->id() << " Gauss point = " << _qp << " slip_incr_max = " << slip_incr_max << "\n");
}

void
CoupledPFCrysPlastMaterial::postSolveStatevar()
{
  // We don't care about _acc_slip evolution here, so I don't bother to sum its weighted values so far
  _acc_slip[_qp] = _accslip_tmp;
}

void
CoupledPFCrysPlastMaterial::preSolveStress()
{
  _pk2_tmp = _pk2_old[_qp] + _elasticity_tensor[_qp] * (_elastic_strain[_qp] - _elastic_strain_old[_qp]);
}

void
CoupledPFCrysPlastMaterial::solveStress()
{
  unsigned int iter = 0;
  RankTwoTensor resid, dpk2;
  RankFourTensor jac;

  calc_resid_jacob(resid, jac); // Calculate stress residual

  Real rnorm = resid.L2norm();

  while (rnorm > _rtol && iter <  _maxiter) // Check for stress residual tolerance
  {
    dpk2 = -jac.invSymm() * resid; // Calculate stress increment
    _pk2_tmp = _pk2_tmp + dpk2; // Update stress

    calc_resid_jacob(resid, jac); // Calculate stress residual

    rnorm=resid.L2norm();

    iter++;
  }
}

void
CoupledPFCrysPlastMaterial::postSolveQp()
{
  _subee = (_fe.transpose() * _fe - _iden) * 0.5; // elastic strain
  _subpdfgrd = _fp_inv.inverse(); // plastic dfgrd
  _subsig = _fe * _pk2_tmp * _fe.transpose()/_fe.det();
  _subPK2 = _pk2_tmp;
  _subJacobian = calcTangentModuli();
  for (unsigned int i = 0; i < _nss; ++i)
    _subslprsis[i] = _gss_tmp[i];

  // calculate the equivalent slip incr
  _sub_eqv_slip_incr.zero();
  for (unsigned int i = 0; i < _nss; ++i)
    _sub_eqv_slip_incr += _s0[i] * _slip_incr[i];
  _sub_eqv_slip_incr = _iden - _sub_eqv_slip_incr;
}

void
CoupledPFCrysPlastMaterial::postFSCPrun(Real& sum_h)
{
  // make average of properties
  _stress[_qp]         = _sigma / sum_h;
  _pk2[_qp]            = _PK2 / sum_h;
  _crysrot             = _CrysRot / sum_h;
  _fp[_qp]             = _pdfgrd / sum_h;  // plastic dfgrd
  _elastic_strain[_qp] = _ee / sum_h;      // elastic strain
  _eqv_slip_incr       = _eqv_slip_incr / sum_h;
  _Jacobian_mult[_qp]  = _Jacobian / sum_h;
  _matrot              = get_current_rotation(_deformation_gradient[_qp]); // Calculate material rotation
  _lag_e[_qp]          = (_deformation_gradient[_qp].transpose() * _deformation_gradient[_qp] - _iden) * 0.5; // total strain

  for (unsigned int i = 0; i < _nss; ++i)
  {
    _gss[_qp][i] = _slprsis[i] / sum_h;
    _dgss[_qp][i] = _gss[_qp][i] - _gss_old[_qp][i];
  }

  _update_rot[_qp] = _matrot * _crysrot;
}

void
CoupledPFCrysPlastMaterial::getRotatedElasticityTensor()
{
  std::ifstream fangle(_Euler_angles_file_name.c_str());
  if (!fangle)
    mooseError("Can't open Euler angle file ");
  for (unsigned int i = 0; i < 4; ++i)
    fangle.ignore(255, '\n'); // ignore line

  Real weight;
  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    RealVectorValue Euler_Angles;
    for (unsigned int j = 0; j < 3; ++j)
      fangle >> Euler_Angles(j);

    fangle >> weight;

    // Rotate one elasticity tensor for each grn
    RotationTensor R(Euler_Angles);
    RealTensorValue RT;

    for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
      for (unsigned int j = 0; j < LIBMESH_DIM; ++j)
        RT(i,j) = R(j,i); // transpose, work in intermediate config

    _C_rotated[grn] = _Cijkl;
    _C_rotated[grn].rotate(RT);
    _rotensor[grn] = RT;
  }

  fangle.close();
}

void
CoupledPFCrysPlastMaterial::getRotatedSchmidTensor()
{
  std::ifstream fslip(_slip_sys_file_name.c_str());
  Real vec[LIBMESH_DIM];
  if (!fslip)
    mooseError("Cannot open slip system file ");

  for(unsigned int i = 0; i < _nss; ++i)
  {
    for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      fslip >> vec[j];

    Real mag;
    mag = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    mag = sqrt(mag);

    for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      _sn[i*LIBMESH_DIM+j] = vec[j]/mag;
  }

  for(unsigned int i = 0; i < _nss; ++i)
  {
    for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      fslip >> vec[j];

    Real mag;
    mag = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    mag = sqrt(mag);

    for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      _sd[i*LIBMESH_DIM+j] = vec[j]/mag;
  }

  fslip.close();

  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    for(unsigned int i = 0; i < _nss; ++i)
    {
      for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      {
        _mo_rotated[grn][i*LIBMESH_DIM+j] = 0.0;

        for(unsigned int k = 0; k < LIBMESH_DIM; ++k)
          _mo_rotated[grn][i*LIBMESH_DIM+j] = _mo_rotated[grn][i*LIBMESH_DIM+j] + _rotensor[grn](j,k) * _sd[i*LIBMESH_DIM+k];
      }

      for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
      {
        _no_rotated[grn][i*LIBMESH_DIM+j] = 0.0;
        for(unsigned int k = 0; k < LIBMESH_DIM; ++k)
          _no_rotated[grn][i*LIBMESH_DIM+j] = _no_rotated[grn][i*LIBMESH_DIM+j] + _rotensor[grn](j,k) * _sn[i*LIBMESH_DIM+k];
      }

    }

    //Calculate rotated Schmid tensor
    for (unsigned int i = 0; i < _nss; ++i)
      for (unsigned int j = 0; j < LIBMESH_DIM; ++j)
        for (unsigned int k = 0; k < LIBMESH_DIM; ++k)
          _s0_rotated[grn][i](j,k) = _mo_rotated[grn][i*LIBMESH_DIM+j] * _no_rotated[grn][i*LIBMESH_DIM+k];
  }
}

void
CoupledPFCrysPlastMaterial::getLatentHardeningProps()
{
  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    for(unsigned int i = 0; i < _nss; ++i)
    {
      for(unsigned int j = 0; j < _nss; ++j)
      {
        Real _nab = _no_rotated[grn][i*3] * _no_rotated[grn][j*3] + _no_rotated[grn][i*3+1] * _no_rotated[grn][j*3+1] + _no_rotated[grn][i*3+2] * _no_rotated[grn][j*3+2];
        _nab = _nab * _nab;

       if ((_nab > 1.0) && (_nab < 1.0 + 1.0e-5))
         _nab = 1.0;

        mooseAssert(_nab <= 1.0, " (1-_nab) is less than zero in power operation");

        _alpha_ab[grn][i][j] = pow(1-_nab,0.5) * 2.0 / (libMesh::pi);

      }
    }
  }
}

/*
void
CoupledPFCrysPlastMaterial::calcResidual( RankTwoTensor &resid )
{
  RankTwoTensor iden, ce, ee, ce_pk2, eqv_slip_incr, pk2_new;

  iden.zero();
  iden.addIa(1.0);

  _fe = _dfgrd_tmp * _fp_old_inv;

  ce = _fe.transpose() * _fe;
  ce_pk2 = ce * _pk2_tmp;
  ce_pk2 = ce_pk2 / _fe.det();

  // Calculate Schmid tensor and resolved shear stresses
  for (unsigned int i = 0; i < _nss; ++i)
    _tau[i] = ce_pk2.doubleContraction(_s0[i]);


  getSlipIncrements(); // Calculate dslip,dslipdtau

  eqv_slip_incr.zero();
  for (unsigned int i = 0; i < _nss; ++i)
    eqv_slip_incr += _s0[i] * _slip_incr[i];

  eqv_slip_incr = iden - eqv_slip_incr;
  _fp_inv = _fp_old_inv * eqv_slip_incr;
  _fe = _dfgrd_tmp * _fp_inv;


  ce = _fe.transpose() * _fe;
  ee = ce - iden;
  ee *= 0.5;

  pk2_new = _elasticity_tensor[_qp] * ee;

  resid = _pk2_tmp - pk2_new;

} */

// Override but Empty the following functions

void
CoupledPFCrysPlastMaterial::getEulerAngles()
{
}

void
CoupledPFCrysPlastMaterial::getEulerRotations()
{
}

void
CoupledPFCrysPlastMaterial::getSlipSystems()
{
}

void
CoupledPFCrysPlastMaterial::postSolveStress()
{
}
