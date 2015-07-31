#include "UO2AnisoConsTempXeL.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UO2AnisoConsTempXeL>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("temp", "Constant temperature in Kelvin");
  params.addParam<Real>("kappa_cg",1.0, "The kappa parameter for the gas concentration");
  params.addParam<Real>("f0s", 0.125,"The GB energy constant ");
  params.addParam<Real>("tstop", 0.001,"Stop time for grain growth ");
  params.addRequiredParam<Real>("M0GB","GB mobility constant in m^4/(J-s)");
  params.addRequiredParam<Real>("QGB","GB energy of migration in eV");
  params.addRequiredParam<Real>("sigma_GB","GB Surface energy in J/m^2");
  params.addRequiredParam<unsigned int>("crys_num","number of grains");
  params.addRequiredParam<Real>("L0","Regular solution parameter in eV");
  params.addRequiredParam<Real>("HXe","Xe migration barrier in eV");
  params.addRequiredParam<Real>("D0Xe","Xe pre-exponential factor in nm^2/s");
  params.addRequiredCoupledVar("c","Concentration");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");

  return params;
}

UO2AnisoConsTempXeL::UO2AnisoConsTempXeL(const InputParameters & parameters) :
    Material(parameters),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _temp(getParam<Real>("temp")),
    _kappa_cg(getParam<Real>("kappa_cg")),
    _f0s(getParam<Real>("f0s")),
    _tstop(getParam<Real>("tstop")),
    _M0GB(getParam<Real>("M0GB")),
    _QGB(getParam<Real>("QGB")),
    _sigma_GB(getParam<Real>("sigma_GB")),
    _L0(getParam<Real>("L0")),
    _HXe(getParam<Real>("HXe")),
    _D0Xe(getParam<Real>("D0Xe")),
    _Lg(declareProperty<Real>("Lg")),
    _Dg(declareProperty<Real>("Dg")),
    _Mg(declareProperty<Real>("Mg")),
    _Mgm(declareProperty<Real>("Mgm")),
    _grad_Mg(declareProperty<RealGradient>("grad_Mg")),
    _grad_Mgm(declareProperty<RealGradient>("grad_Mgm")),
    _kT(declareProperty<Real>("kT")),
    _vC_i(declareProperty<std::vector<Real> >("vC_i")),
    _vm_i(declareProperty<std::vector<Real> >("vm_i")),
    _vb_i(declareProperty<std::vector<Real> >("vb_i")),
    _kb(declareProperty<Real>("kb")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _kappa_GB(declareProperty<Real>("kappa_op")),
    _gamma(declareProperty<Real>("gamma_asymm")),
    _L(declareProperty<Real>("L")),
    _mu(declareProperty<Real>("mu")),
    _LogC(declareProperty<Real>("LogC")),
    _LogTol(declareProperty<Real>("LogTol")),
    _tgrad_corr_mult(declareProperty<Real>("tgrad_corr_mult"))

{
  _n = coupledComponents("v");
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (unsigned int i=0; i<_n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }

  Real JtoeV = 6.24150974e18;// joule to eV conversion
  //Convert parameter units
  _sigma_GB *= JtoeV * (1e-9 * 1e-9);//convert of eV/nm^2
  _M0GB *= 1.0e0/(JtoeV * (1.e-9 * 1.e-9 * 1.e-9 * 1.e-9));//Convert to nm^4/(eV*s)

}

void
UO2AnisoConsTempXeL::computeProperties()
{

  //Materials file for Xe, U

  Real vC_i[] =
  {
    //3.55, 3.55, 0.64, 0.64, 4.79, 4.79
    0.64, 0.64, 0.64, 0.64, 4.79, 4.79
    //00 01 02 03; 10 11 12 13; 20 21 22 23; 30 31 32 33
    //00 01 02 03; 01 11 12 13; 02 12 22 23; 03 13 23 33
    //CORRECT  3.55, 3.55, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 3.55, 3.55
    //            3.55, 3.55, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 3.55, 3.55
    //  3.55, 0.64, 0.64, 4.79, 3.55,3.55, 0.64, 0.64, 4.79, 4.79, 3.55, 3.55, 0.64, 0.64, 4.79, 4.79, 3.55, 3.55, 0.64, 0.64, 4.79, 4.79
    //      3.55, 3.55, 0.64, 0.64, 4.79, 4.79
    //      4.09, 4.09, 0.97, 0.97, 6.42, 6.42
  };
  Real vm_i[] =
  {
    //0.872, 0.872, 0.346, 0.346, 0.954, 0.954
    0.346, 0.346, 0.346, 0.346, 0.954, 0.954
    //CORRECT 0.872, 0.872, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.872, 0.872
    //      0.872, 0.872, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.872, 0.872
    //      0.872, 0.872, 0.346, 0.346, 0.954, 0.954, 0.872, 0.872, 0.346, 0.346, 0.954, 0.954, 0.872, 0.872, 0.346, 0.346, 0.954, 0.954, 0.872, 0.872, 0.346, 0.346, 0.954, 0.954
    //      0.872, 0.872, 0.346, 0.346, 0.954, 0.954
    //      0.7, 0.7, 0.171, 0.171, 0.9, 0.9
  };
  Real vb_i[] =
  {
    //-0.444, -0.444, -0.278, -0.278, 1.485, 1.485
    -0.278, -0.278, -0.278, -0.278, 1.485, 1.485
    //CORRECT      -0.444, -0.444, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.444, -0.444
    //-0.444, -0.444, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.444, -0.444
    //      -0.444, -0.444, -0.278, -0.278, 1.485, 1.485, -0.444, -0.444, -0.278, -0.278, 1.485, 1.485, -0.444, -0.444, -0.278, -0.278, 1.485, 1.485, -0.444, -0.444, -0.278, -0.278, 1.485, 1.485
    //      -0.444, -0.444, -0.278, -0.278, 1.485, 1.485
    //      0.9, 0.9, 0.315, 0.315, 2.562, 2.562
  };
  Real gbw_i[] =
  {
    //3.704, 3.704, 4.128, 4.128, 3.642, 3.642
    4.128, 4.128, 4.128, 4.128, 3.642, 3.642
    //CORRECT      3.704, 3.704, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 3.704, 3.704
    //3.704, 3.704, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 3.704, 3.704
    //      3.704, 3.704, 4.128, 4.128, 3.642, 3.642, 3.704, 3.704, 4.128, 4.128, 3.642, 3.642, 3.704, 3.704, 4.128, 4.128, 3.642, 3.642, 3.704, 3.704, 4.128, 4.128, 3.642, 3.642
    //      3.704, 3.704, 4.128, 4.128, 3.642, 3.642
    //      5.0, 5.0, 3.688, 3.688, 1.876, 1.876
  };

  Real D0 = _D0Xe;// nm^2/s
  Real q_i = 0.0;
  Real DH0 = _HXe; //eV

  for(unsigned int qp = 0; qp<_qrule->n_points(); qp++)
  {
    _kb[qp] = 8.617343e-5;//Boltzmann constant in eV/K

    _kT[qp] = _kb[qp] * _temp;

    Real f_i = 0.0;
    RealGradient grad_f_i = 0.0;
    Real w_GB = 0.0;
    unsigned int ind = 0;
    Real smphi = 0.0;

    for (unsigned int i=0; i<_n; ++i)
      for (unsigned int j=i+1; j<_n; ++j)
      {
        if (((*_vals[i])[qp] + (*_vals[j])[qp]) > 0.9 || ((*_vals[i])[_qp] + (*_vals[j])[_qp]) < 0.1) {

          Real f_i = -8.0 * (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];
          RealGradient grad_f_i = -8.0 * 2.0 * (*_vals[i])[qp] * (*_grad_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];
          grad_f_i -= 8.0 * (*_vals[i])[qp] * (*_vals[i])[qp] * 2.0 * (*_vals[j])[qp] * (*_grad_vals[j])[qp];

          w_GB += gbw_i[ind] * (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];
          smphi += (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];

          //          ind++;
        }

        else {
          Real Scale_ij = (*_vals[i])[qp] + (*_vals[j])[qp];
          RealGradient grad_Scale_ij = (*_grad_vals[i])[qp] + (*_grad_vals[j])[qp];
          Real f_i = -8.0 * (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp] / (Scale_ij * Scale_ij * Scale_ij * Scale_ij);
          RealGradient grad_f_i = (-8.0 * 2.0 * (*_vals[i])[qp] * (*_grad_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp] - 8.0 * (*_vals[i])[qp] * (*_vals[i])[qp] * 2.0 * (*_vals[j])[qp] * (*_grad_vals[j])[qp]) * (Scale_ij * Scale_ij * Scale_ij * Scale_ij);
          grad_f_i -= 4 * grad_Scale_ij * Scale_ij * Scale_ij * Scale_ij * (-8.0 * (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp]);
          grad_f_i = grad_f_i / (Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij * Scale_ij);
          f_i = f_i * ((*_vals[i])[qp] + (*_vals[j])[qp]) / ((*_vals[i])[qp] + 1);
          grad_f_i = grad_f_i * ((*_vals[i])[qp] + (*_vals[j])[qp]) / (2) + f_i * ((*_grad_vals[i])[qp] + (*_grad_vals[j])[qp]) / (2);

          w_GB += gbw_i[ind] * (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];
          smphi += (*_vals[i])[qp] * (*_vals[i])[qp] * (*_vals[j])[qp] * (*_vals[j])[qp];

          //    ind++;
        }
        ind++;
      }

    /*    for (unsigned int i=0; i<_n; ++i)
          for (unsigned int j=i+1; j<_n; ++j)
          {
          ind=i*4+(j-1);

          //Real phis = (*_vals[i])[qp]*(*_vals[i])[qp]*(*_vals[j])[qp]*(*_vals[j])[qp];

          f_i += -8.0*(*_vals[i])[qp]*(*_vals[i])[qp]*(*_vals[j])[qp]*(*_vals[j])[qp];
          grad_f_i += -8.0*2.0*(*_vals[i])[qp]*(*_grad_vals[i])[qp]*(*_vals[j])[qp]*(*_vals[j])[qp];
          grad_f_i += -8.0*(*_vals[i])[qp]*(*_vals[i])[qp]*2.0*(*_vals[j])[qp]*(*_grad_vals[j])[qp];

          w_GB += gbw_i[ind]*(*_vals[i])[qp]*(*_vals[i])[qp]*(*_vals[j])[qp]*(*_vals[j])[qp];
          smphi += (*_vals[i])[qp]*(*_vals[i])[qp]*(*_vals[j])[qp]*(*_vals[j])[qp];

          //        ind++;
          }
    */

    if (smphi > 0)
      w_GB /= smphi;
    else
      w_GB = gbw_i[0];

    //            if (w_GB > 5.0)
    //    w_GB = 5.0;

    //For gas atoms
    //    Real DQ = DQ0 - q_i*f_i;
    //   RealGradient grad_DQ = -q_i*grad_f_i;
    Real DH = DH0;
    RealGradient grad_DH = 0;

    _Dg[qp] = D0*std::exp(-DH/_kT[qp]);//nm^2/s
    _Mg[qp] = _Dg[qp] / _kT[qp]; //nm^2/(s*eV);
    _Mgm[qp] = -_Dg[qp] / _kT[qp]; //nm^2/(s*eV);

    RealGradient grad_D = -D0*grad_DH*std::exp(-DH/_kT[qp]);
    _grad_Mg[qp] = grad_D/_kT[qp];
    _grad_Mgm[qp] = -grad_D/_kT[qp];

    _Lg[qp] = _L0;//eV

    //    _Ef[qp] = 4.35;//eV
    //   _grad_Ef[qp] = 0.0;//eV

    //    _vC_i[qp].resize(6);
    //    _vm_i[qp].resize(6);
    //    _vb_i[qp].resize(6);

    _vC_i[qp].resize(16);
    _vm_i[qp].resize(16);
    _vb_i[qp].resize(16);


    //  for (unsigned int i=0; i<6; ++i)
    //    {
    //      _vC_i[qp][i] = vC_i[i];
    //      _vm_i[qp][i] = vm_i[i];
    //      _vb_i[qp][i] = vb_i[i];
    //    }


    for (unsigned int i=0; i<16; ++i)
    {
      _vC_i[qp][i] = vC_i[i];
      _vm_i[qp][i] = vm_i[i];
      _vb_i[qp][i] = vb_i[i];
    }

    _kappa_c[qp] = _kappa_cg;

    //For GB
    Real M_GB=0.0;
    if (_t>_tstop) {
      M_GB=0.0;
    }
    else {
      M_GB = _M0GB*std::exp(-_QGB/(_kb[qp]*_temp));
    }
    _L[qp] = 4.0/3.0*M_GB/w_GB;

    _kappa_GB[qp] = 3.0/4.0*_sigma_GB*w_GB;
    _gamma[qp] = 1.5;

    _mu[qp] = 3.0/4.0*1/_f0s*_sigma_GB/w_GB;
    _tgrad_corr_mult[qp] = _mu[qp]*9.0/8.0;
    //Cutoffs for mathematical functions
    _LogC[qp] = 1e-12;
    _LogTol[qp] = 1e-12;

  }
}
