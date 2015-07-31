#include "UO2AnisoConsTempXeUVa2.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<UO2AnisoConsTempXeUVa2>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("temp", "Constant temperature in Kelvin");
  params.addParam<Real>("kappa_cg",1.0, "The kappa parameter for the gas concentration");
  params.addParam<Real>("kappa_cu",1.0, "The kappa parameter for the uranium concentration");
  params.addParam<Real>("kappa_cv",1.0, "The kappa parameter for the vacancy concentration");
  params.addParam<Real>("kappa_cgv",1.0, "The kappa parameter for the gas-vacancy concentration");
  params.addParam<Real>("kappa_cvv",1.0, "The kappa parameter for the vacancy-vacancy concentration");
  params.addParam<Real>("kappa_cui",1.0, "The kappa parameter for the interstitial concentration");
  params.addParam<Real>("kappa_cgvv",1.0, "The kappa parameter for the cgv concentration");
  params.addParam<Real>("ki2",1.0, "The kappa parameter for the interstitial concentration");
  params.addParam<Real>("kv2",1.0, "The kappa parameter for the interstitial concentration");
  params.addParam<Real>("f0s", 0.125,"The GB energy constant ");
  params.addRequiredParam<Real>("M0GB", "GB mobility constant in m^4/(J-s)");
  params.addRequiredParam<Real>("QGB", "GB energy of migration in eV");
  params.addRequiredParam<Real>("sigma_GB", "GB Surface energy in J/m^2");
  params.addRequiredParam<Real>("LXeVa", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LUVa", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeU", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeVaU", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeVaVa", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeVaXe", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeVa2", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LXeVaVa2", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LUVa2", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LVaVa2", "Regular solution parameter in eV");
  params.addRequiredParam<Real>("LTXeVa", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTUVa", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeU", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeVaU", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeVaVa", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeVaXe", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeVa2", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTXeVaVa2", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTUVa2", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("LTVaVa2", "Regular solution parameter T in kB");
  params.addRequiredParam<Real>("EBXeVa", "XeVa binding energy in eV");
  params.addRequiredParam<Real>("EBXeVa2", "XeVaVa binding energy in eV");
  params.addRequiredParam<Real>("EBXeVa2B", "XeVaVa binding energy in eV");
  params.addRequiredParam<Real>("EBVaVa", "VaVa binding energy in eV");
  params.addRequiredParam<Real>("SBXeVa", "XeVa binding entropy in kB");
  params.addRequiredParam<Real>("SBXeVa2", "XeVaVa binding entropy in kB");
  params.addRequiredParam<Real>("SBXeVa2B", "XeVaVa binding entropy in kB");
  params.addRequiredParam<Real>("SBVaVa", "VaVa binding entropy in kB");
  params.addRequiredParam<Real>("HXe", "Xe migration barrier in eV");
  params.addRequiredParam<Real>("D0Xe", "Xe pre-exponential factor in nm^2/s");
  params.addRequiredParam<Real>("HV", "U migration barrier in eV");
  params.addRequiredParam<Real>("D0V", "U pre-exponential factor in nm^2/s");
  params.addRequiredParam<Real>("HV2", "U migration barrier via bound divacancy in eV");
  params.addRequiredParam<Real>("D0V2", "U pre-exponential factor via bound divacancy in nm^2/s");
  params.addRequiredParam<Real>("HI", "U interstitial migration barrier in eV");
  params.addRequiredParam<Real>("D0I", "U interstitial pre-exponential factor in nm^2/s");
  params.addRequiredParam<Real>("D0GV2", "CGV2 (XeVaVa) pre-exponential factor in nm^2/s");
  params.addRequiredParam<Real>("HGV2", "CGV2 (XeVaVa) migration barrier in eV");
  params.addRequiredParam<Real>("Z", "Recombination parameter");
  params.addRequiredParam<Real>("dHVaUI", "Va+UI recombination energy");
  params.addRequiredParam<Real>("dHVa2", "VaVa formation energy");
  params.addRequiredParam<Real>("dSVa2", "VaVa recombination entropy");
  params.addRequiredParam<Real>("dHVa", "Va formation energy");
  params.addRequiredParam<Real>("dSVa", "Va formation entropy");
  params.addRequiredParam<Real>("dHUI", "UI formation energy");
  params.addRequiredParam<Real>("dHXeVaUI", "XeVa+UI recombination energy");
  params.addRequiredParam<Real>("dHXeVa2UI", "XeVaVa+UI recombination energy");
  //c1=cg, c2=cv, c3=cgv, c4=cvv, c5=cui, c6=cgvv,
  params.addRequiredCoupledVar("c1", "Concentration");
  params.addRequiredCoupledVar("c2", "Concentration");
  params.addRequiredCoupledVar("c3", "Concentration");
  params.addRequiredCoupledVar("c4", "Concentration");
  params.addRequiredCoupledVar("c6", "Concentration");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");

  return params;
}

UO2AnisoConsTempXeUVa2::UO2AnisoConsTempXeUVa2(const InputParameters & parameters) :
    Material(parameters),
    _c1(coupledValue("c1")),
    _c2(coupledValue("c2")),
    _c3(coupledValue("c3")),
    _c4(coupledValue("c4")),
    _c6(coupledValue("c6")),
    _grad_c1(coupledGradient("c1")),
    _grad_c2(coupledGradient("c2")),
    _grad_c3(coupledGradient("c3")),
    _grad_c4(coupledGradient("c4")),
    _grad_c6(coupledGradient("c6")),
    _temp(getParam<Real>("temp")),
    _kappa_cg(getParam<Real>("kappa_cg")),
    _kappa_cu(getParam<Real>("kappa_cu")),
    _kappa_cv(getParam<Real>("kappa_cv")),
    _kappa_cgv(getParam<Real>("kappa_cgv")),
    _kappa_cvv(getParam<Real>("kappa_cvv")),
    _kappa_cui(getParam<Real>("kappa_cui")),
    _kappa_cgvv(getParam<Real>("kappa_cgvv")),
    _ki2(getParam<Real>("ki2")),
    _kv2(getParam<Real>("kv2")),
    _f0s(getParam<Real>("f0s")),
    _M0GB(getParam<Real>("M0GB")),
    _QGB(getParam<Real>("QGB")),
    _sigma_GB(getParam<Real>("sigma_GB")),
    _LXeVa(getParam<Real>("LXeVa")),
    _LUVa(getParam<Real>("LUVa")),
    _LXeU(getParam<Real>("LXeU")),
    _LXeVaU(getParam<Real>("LXeVaU")),
    _LXeVaVa(getParam<Real>("LXeVaVa")),
    _LXeVaXe(getParam<Real>("LXeVaXe")),
    _LXeVa2(getParam<Real>("LXeVa2")),
    _LXeVaVa2(getParam<Real>("LXeVaVa2")),
    _LUVa2(getParam<Real>("LUVa2")),
    _LVaVa2(getParam<Real>("LVaVa2")),
    _LTXeVa(getParam<Real>("LTXeVa")),
    _LTUVa(getParam<Real>("LTUVa")),
    _LTXeU(getParam<Real>("LTXeU")),
    _LTXeVaU(getParam<Real>("LTXeVaU")),
    _LTXeVaVa(getParam<Real>("LTXeVaVa")),
    _LTXeVaXe(getParam<Real>("LTXeVaXe")),
    _LTXeVa2(getParam<Real>("LTXeVa2")),
    _LTXeVaVa2(getParam<Real>("LTXeVaVa2")),
    _LTUVa2(getParam<Real>("LTUVa2")),
    _LTVaVa2(getParam<Real>("LTVaVa2")),
    _EBXeVa(getParam<Real>("EBXeVa")),
    _EBXeVa2(getParam<Real>("EBXeVa2")),
    _EBXeVa2B(getParam<Real>("EBXeVa2B")),
    _EBVaVa(getParam<Real>("EBVaVa")),
    _SBXeVa(getParam<Real>("SBXeVa")),
    _SBXeVa2(getParam<Real>("SBXeVa2")),
    _SBXeVa2B(getParam<Real>("SBXeVa2B")),
    _SBVaVa(getParam<Real>("SBVaVa")),
    _HXe(getParam<Real>("HXe")),
    _D0Xe(getParam<Real>("D0Xe")),
    _HV(getParam<Real>("HV")),
    _D0V(getParam<Real>("D0V")),
    _HV2(getParam<Real>("HV2")),
    _D0V2(getParam<Real>("D0V2")),
    _HI(getParam<Real>("HI")),
    _D0I(getParam<Real>("D0I")),
    _HGV2(getParam<Real>("HGV2")),
    _D0GV2(getParam<Real>("D0GV2")),
    _Z(getParam<Real>("Z")),
    _dHVaUI(getParam<Real>("dHVaUI")),
    _dHVa2(getParam<Real>("dHVa2")),
    _dSVa2(getParam<Real>("dSVa2")),
    _dHVa(getParam<Real>("dHVa")),
    _dSVa(getParam<Real>("dSVa")),
    _dHUI(getParam<Real>("dHUI")),
    _dHXeVaUI(getParam<Real>("dHXeVaUI")),
    _dHXeVa2UI(getParam<Real>("dHXeVa2UI")),
    //    _dHGVa2(getParam<Real>("dHGVa2")),
    //    _dSGVa2(getParam<Real>("dSGVa2")),
    _EBgXeVa(declareProperty<Real>("EBgXeVa")),
    _EBgXeVa2(declareProperty<Real>("EBgXeVa2")),
    _EBgXeVa2B(declareProperty<Real>("EBgXeVa2B")),
    _EBgVaVa(declareProperty<Real>("EBgVaVa")),
    _SBgXeVa(declareProperty<Real>("SBgXeVa")),
    _SBgXeVa2(declareProperty<Real>("SBgXeVa2")),
    _SBgXeVa2B(declareProperty<Real>("SBgXeVa2B")),
    _SBgVaVa(declareProperty<Real>("SBgVaVa")),
    _LgXeVa(declareProperty<Real>("LgXeVa")),
    _LgUVa(declareProperty<Real>("LgUVa")),
    _LgXeU(declareProperty<Real>("LgXeU")),
    _LgXeVaU(declareProperty<Real>("LgXeVaU")),
    _LgXeVaVa(declareProperty<Real>("LgXeVaVa")),
    _LgXeVaXe(declareProperty<Real>("LgXeVaXe")),
    _LgXeVa2(declareProperty<Real>("LgXeVa2")),
    _LgXeVaVa2(declareProperty<Real>("LgXeVaVa2")),
    _LgUVa2(declareProperty<Real>("LgUVa2")),
    _LgVaVa2(declareProperty<Real>("LgVaVa2")),
    _Dg(declareProperty<Real>("Dg")),
    _Mg(declareProperty<Real>("Mg")),
    _Dv(declareProperty<Real>("Dv")),
    _Mv(declareProperty<Real>("Mv")),
    _grad_Mg(declareProperty<RealGradient>("grad_Mg")),
    _grad_Mv(declareProperty<RealGradient>("grad_Mv")),
    _Dv2(declareProperty<Real>("Dv2")),
    _Mv2(declareProperty<Real>("Mv2")),
    _grad_Mv2(declareProperty<RealGradient>("grad_Mv2")),
    _DI(declareProperty<Real>("DI")),
    _MI(declareProperty<Real>("MI")),
    _grad_MI(declareProperty<RealGradient>("grad_MI")),
    _Dgv2(declareProperty<Real>("Dgv2")),
    _Mgv2(declareProperty<Real>("Mgv2")),
    _grad_Mgv2(declareProperty<RealGradient>("grad_Mgv2")),
    _kT(declareProperty<Real>("kT")),
    _Zg(declareProperty<Real>("Zg")),
    _dgHVaUI(declareProperty<Real>("dgHVaUI")),
    _dgHVa2(declareProperty<Real>("dgHVa2")),
    _dgSVa2(declareProperty<Real>("dgSVa2")),
    _dgHVa(declareProperty<Real>("dgHVa")),
    _dgSVa(declareProperty<Real>("dgSVa")),
    _dgHUI(declareProperty<Real>("dgHUI")),
    _dgHXeVaUI(declareProperty<Real>("dgHXeVaUI")),
    _dgHXeVa2UI(declareProperty<Real>("dgHXeVa2UI")),
    //    _dgHGVa2(declareProperty<Real>("dgHGVa2")),
    //    _dgSGVa2(declareProperty<Real>("dgSGVa2")),
    _vC_i(declareProperty<std::vector<Real> >("vC_i")),
    _vm_i(declareProperty<std::vector<Real> >("vm_i")),
    _vb_i(declareProperty<std::vector<Real> >("vb_i")),
    _vVaC_i(declareProperty<std::vector<Real> >("vVaC_i")),
    _vVam_i(declareProperty<std::vector<Real> >("vVam_i")),
    _vVab_i(declareProperty<std::vector<Real> >("vVab_i")),
    _vUIC_i(declareProperty<std::vector<Real> >("vUIC_i")),
    _vUIm_i(declareProperty<std::vector<Real> >("vUIm_i")),
    _vUIb_i(declareProperty<std::vector<Real> >("vUIb_i")),
    _kb(declareProperty<Real>("kb")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _kappa_cU(declareProperty<Real>("kappa_cU")),
    _kappa_cV(declareProperty<Real>("kappa_cV")),
    _kappa_cGV(declareProperty<Real>("kappa_cGV")),
    _kappa_cVV(declareProperty<Real>("kappa_cVV")),
    _kappa_cVU(declareProperty<Real>("kappa_cVU")),
    _kappa_cGVU(declareProperty<Real>("kappa_cGVU")),
    _kappa_cVVU(declareProperty<Real>("kappa_cVVU")),
    _kappa_cUI(declareProperty<Real>("kappa_cUI")),
    _kappa_cGVV(declareProperty<Real>("kappa_cGVV")),
    _kappa_cGVVU(declareProperty<Real>("kappa_cGVVU")),
    _kI2(declareProperty<Real>("ki2")),
    _kV2(declareProperty<Real>("kv2")),
    _kappa_GB(declareProperty<Real>("kappa_op")),
    _gamma(declareProperty<Real>("gamma_asymm")),
    _L(declareProperty<Real>("L")),
    _mu(declareProperty<Real>("mu")),
    _cu(declareProperty<Real>("cu")),
    _grad_cu(declareProperty<RealGradient>("grad_cu")),
    // _cu_old(declareProperty<Real>("cu_old")),
    // _grad_cu_old(declareProperty<RealGradient>("grad_cu_old")),
    _LogC(declareProperty<Real>("LogC")),
    _LogC2(declareProperty<Real>("LogC2")),
    _LogTol(declareProperty<Real>("LogTol")),
    _tgrad_corr_mult(declareProperty<Real>("tgrad_corr_mult")),
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);

  // For vacancies
  // _nVa = coupledComponents("vVa");
  // _valsVa.resize(_nVa);
  // _grad_valsVa.resize(_nVa);

  for (unsigned int i=0; i<_n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }

  // For vacancies
  // for (int i=0; i<_nVa; ++i)
  // {
  //   _valsVa[i] = &coupledValue("v", i);
  //   _grad_valsVa[i] = &coupledGradient("v", i);
  // }

  //Convert parameter units
  const Real JtoeV = 6.24150974e18;// joule to eV conversion
  _sigma_GB *= JtoeV * (1e-9*1e-9);//convert of eV/nm^2
  _M0GB *= 1.0e0/(JtoeV * (1.e-9*1.e-9*1.e-9*1.e-9));//Convert to nm^4/(eV*s)
}

void
UO2AnisoConsTempXeUVa2::computeProperties()
{

  //Materils file for Xe, Va, XeVa, VaVa, UI, XeVaVa system

  Real vC_i[] =
  {
    3.55, 3.55, 0.64, 0.64, 4.79, 4.79
    //  4.09, 4.09, 0.97, 0.97, 6.42, 6.42
  };
  Real vm_i[] =
  {
    0.872, 0.872, 0.346, 0.346, 0.954, 0.954
    //      0.7, 0.7, 0.171, 0.171, 0.9, 0.9
  };
  Real vb_i[] =
  {
    -0.444, -0.444, -0.278, -0.278, 1.485, 1.485
    //      0.9, 0.9, 0.315, 0.315, 2.562, 2.562
  };
  Real gbw_i[] =
  {
    //                                                           20, 20, 20, 20, 20, 20
    3.704, 3.704, 4.128, 4.128, 3.642, 3.642
    //      5.0, 5.0, 3.688, 3.688, 1.876, 1.876
  };

  //For vacancies
  Real vVaC_i[] =
  {
    3.56, 3.56, 1.23, 1.23, 4.80, 4.80
    //     4.09, 4.09, 0.97, 0.97, 6.42, 6.42
  };
  Real vVam_i[] =
  {
    1.40287, 1.40287, 0.31681, 0.31681, 0.856, 0.856
    //      0.7, 0.7, 0.171, 0.171, 0.9, 0.9
  };
  Real vVab_i[] =
  {
    -2.76788, -2.76788, 0.11084, 0.11084, 1.78, 1.78
    //      0.9, 0.9, 0.315, 0.315, 2.562, 2.562
  };
  /*Real gbwVa_i[] =
    {
    5.0, 5.0, 3.688, 3.688, 1.876, 1.876
    };*/

  //For inersttials
  Real vUIC_i[] =
  {
    3.56, 3.56, 1.23, 1.23, 4.80, 4.80
    //     4.09, 4.09, 0.97, 0.97, 6.42, 6.42
  };
  Real vUIm_i[] =
  {
    1.40287, 1.40287, 0.31681, 0.31681, 0.856, 0.856
    //      0.7, 0.7, 0.171, 0.171, 0.9, 0.9
  };
  Real vUIb_i[] =
  {
    -2.76788, -2.76788, 0.11084, 0.11084, 1.78, 1.78
    //      0.9, 0.9, 0.315, 0.315, 2.562, 2.562
  };


  Real D0 = _D0V;// nm^2/s
  Real D0G = _D0Xe;// nm^2/s
  Real D0B = _D0V2;// nm^2/s
  Real D0I = _D0I;// nm^2/s
  Real D0GV2 = _D0GV2;// nm^2/s

  //  Real q_i = 0.0;
  //Real DQ0 = 4.39; //eV
  Real DH0 = _HXe; // eV
  Real DHV0 = _HV; // //eV
  Real DHV20 = _HV2; // //eV
  Real DHI0 = _HI; // //eV

  Real DHGV20 = _HGV2; // //eV

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kb[_qp] = 8.617343e-5;//Boltzmann constant in eV/K
    _kT[_qp] = _kb[_qp]*_temp;

    _Zg[_qp] = _Z;

    _dgHVaUI[_qp] = _dHVaUI;
    _dgHVa2[_qp] = _dHVa2;
    _dgSVa2[_qp] = _dSVa2;
    _dgHVa[_qp] = _dHVa;
    _dgSVa[_qp] = _dSVa;
    _dgHUI[_qp] = _dHUI;
    _dgHXeVaUI[_qp] = _dHXeVaUI;
    _dgHXeVa2UI[_qp] = _dHXeVa2UI;
    //    _dgHGVa2[_qp] = _dHGVa2;
    //    _dgSGVa2[_qp] = _dSGVa2;



    Real f_i = 0.0;
    RealGradient grad_f_i = 0.0;
    Real w_GB = 0.0;
    unsigned int ind=0;
    Real smphi = 0.0;

    for (unsigned int i = 0; i < _n; ++i)
      for (unsigned int j = i + 1; j < _n; ++j)
      {
        //Real phis = (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

        f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
               * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i += -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp]
                    * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                    * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

        w_GB += gbw_i[ind] * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        smphi +=  (*_vals[i])[_qp] * (*_vals[i])[_qp]
                  * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        ind++;
      }

    if (smphi > 0)
      w_GB/=smphi;
    else
      w_GB = gbw_i[0];

    // if (w_GB > 5.0)
    //   w_GB = 5.0;

    // For gas atoms
    // Real DQ = DQ0 - q_i*f_i;
    // RealGradient grad_DQ = -q_i*grad_f_i;
    Real DH = DH0;
    RealGradient grad_DH = 0;
    Real DHV = DHV0;
    RealGradient grad_DHV = 0;
    Real DHV2 = DHV20;
    RealGradient grad_DHV2 = 0;
    Real DHI = DHI0;
    RealGradient grad_DHI = 0;

    Real DHGV2 = DHGV20;
    RealGradient grad_DHGV2 = 0;

    _Dg[_qp] = D0G * std::exp(-DH/_kT[_qp]); // nm^2/s
    _Mg[_qp] = _Dg[_qp]/_kT[_qp]; // nm^2/(s*eV);
    _Dv[_qp] = D0 * std::exp(-DHV/_kT[_qp]); // nm^2/s
    _Mv[_qp] = _Dv[_qp]/_kT[_qp]; // nm^2/(s*eV);
    _Dv2[_qp] = D0B * std::exp(-DHV2/_kT[_qp]); // nm^2/s
    _Mv2[_qp] = _Dv2[_qp]/_kT[_qp]; // nm^2/(s*eV);
    _DI[_qp] = D0I * std::exp(-DHI/_kT[_qp]); // nm^2/s
    _MI[_qp] = _DI[_qp]/_kT[_qp]; // nm^2/(s*eV);

    _Dgv2[_qp] = D0GV2 * std::exp(-DHGV2/_kT[_qp]); // nm^2/s
    _Mgv2[_qp] = _Dgv2[_qp]/_kT[_qp]; // nm^2/(s*eV);

    RealGradient grad_D = -D0G * grad_DH/_kT[_qp] * std::exp(-DH/_kT[_qp]);
    RealGradient grad_DV = -D0 * grad_DHV/_kT[_qp] * std::exp(-DHV/_kT[_qp]);
    RealGradient grad_DV2 = -D0B * grad_DHV2/_kT[_qp] * std::exp(-DHV2/_kT[_qp]);
    RealGradient grad_DI = -D0B * grad_DHI/_kT[_qp] * std::exp(-DHI/_kT[_qp]);
    RealGradient grad_DGV2 = -D0B * grad_DHGV2/_kT[_qp] * std::exp(-DHGV2/_kT[_qp]);

    // _grad_Mg[_qp] = _Dg[_qp]/_kT[_qp]*_grad_c[_qp] + grad_D/_kT[_qp]*_c[_qp];

    _grad_Mg[_qp] = grad_D/_kT[_qp];
    _grad_Mv[_qp] = grad_DV/_kT[_qp];
    _grad_Mv2[_qp] = grad_DV2/_kT[_qp];
    _grad_MI[_qp] = grad_DI/_kT[_qp];
    _grad_Mgv2[_qp] = grad_DGV2/_kT[_qp];

    _LgXeVa[_qp] = _LXeVa-_LTXeVa*_temp; // eV
    _LgUVa[_qp] = _LUVa-_LTUVa*_temp; // eV
    _LgXeU[_qp] = _LXeU-_LTXeU*_temp; // eV

    _LgXeVaU[_qp] = _LXeVaU-_LTXeVaU*_temp; // eV
    _LgXeVaVa[_qp] = _LXeVaVa-_LTXeVaVa*_temp; // eV
    _LgXeVaXe[_qp] = _LXeVaXe-_LTXeVaXe*_temp; // eV
    _LgXeVa2[_qp] = _LXeVa2-_LTXeVa2*_temp; // eV
    _LgXeVaVa2[_qp] = _LXeVaVa2-_LTXeVaVa2*_temp; // eV
    _LgUVa2[_qp] = _LUVa2-_LTUVa2*_temp; // eV
    _LgVaVa2[_qp] = _LVaVa2-_LTVaVa2*_temp; // eV

    _EBgXeVa[_qp]= _EBXeVa; // eV
    _EBgXeVa2[_qp]= _EBXeVa2; // eV
    _EBgXeVa2B[_qp]= _EBXeVa2B; // eV
    _EBgVaVa[_qp]= _EBVaVa; // eV

    _SBgXeVa[_qp]= _SBXeVa; // kB
    _SBgXeVa2[_qp]= _SBXeVa2; // kB
    _SBgXeVa2B[_qp]= _SBXeVa2B; // kB
    _SBgVaVa[_qp]= _SBVaVa; // kB


    // Calculate other concentrations based on c1, c2, cgp, cvap;
    // these are the ones that enter the new model
    _cu[_qp]=1-_c1[_qp]-_c2[_qp]-2*_c3[_qp]-2*_c4[_qp]-3*_c6[_qp];
    _grad_cu[_qp]=-_grad_c1[_qp]-_grad_c2[_qp]-2*_grad_c3[_qp]-2*_grad_c4[_qp] - 3*_grad_c6[_qp];

    _vC_i[_qp].resize(6);
    _vm_i[_qp].resize(6);
    _vb_i[_qp].resize(6);

    _vVaC_i[_qp].resize(6);
    _vVam_i[_qp].resize(6);
    _vVab_i[_qp].resize(6);

    _vUIC_i[_qp].resize(6);
    _vUIm_i[_qp].resize(6);
    _vUIb_i[_qp].resize(6);

    for (unsigned int i = 0; i < 6; ++i)
    {
      _vC_i[_qp][i] = vC_i[i];
      _vm_i[_qp][i] = vm_i[i];
      _vb_i[_qp][i] = vb_i[i];
    }

    for (unsigned int i = 0; i < 6; ++i)
    {
      _vVaC_i[_qp][i] = vVaC_i[i];
      _vVam_i[_qp][i] = vVam_i[i];
      _vVab_i[_qp][i] = vVab_i[i];
    }

    for (unsigned int i = 0; i < 6; ++i)
    {
      _vUIC_i[_qp][i] = vUIC_i[i];
      _vUIm_i[_qp][i] = vUIm_i[i];
      _vUIb_i[_qp][i] = vUIb_i[i];
    }

    _kappa_c[_qp] = _kappa_cg;
    _kappa_cU[_qp] = _kappa_cu;
    _kappa_cV[_qp] = _kappa_cv;
    _kappa_cGV[_qp] = _kappa_cgv;
    _kappa_cGVV[_qp] = _kappa_cgvv;

    _kappa_cVV[_qp] = _kappa_cvv;
    _kappa_cVU[_qp] = -_kappa_cu;
    _kappa_cGVU[_qp] = -2.0 * _kappa_cu;
    _kappa_cVVU[_qp] = -2.0 * _kappa_cu;
    _kappa_cUI[_qp] = _kappa_cui;
    _kappa_cGVVU[_qp] = -3.0*_kappa_cgvv;


    _kI2[_qp] = _ki2;
    _kV2[_qp] = _kv2;

    //For GB
    Real M_GB = _M0GB * std::exp(-_QGB/(_kb[_qp]*_temp));
    _L[_qp] = 4.0/3.0 * M_GB/w_GB;
    _kappa_GB[_qp] = 3.0/4.0 * _sigma_GB * w_GB;

    _mu[_qp] = 3.0/4.0*1/_f0s*_sigma_GB/w_GB;
    _tgrad_corr_mult[_qp] = _mu[_qp]*9.0/8.0;

    _gamma[_qp] = 1.5;

    //Cutoffs for mathematical functions
    _LogC[_qp] = 1e-12;
    _LogC2[_qp] = 1e-12;
    _LogTol[_qp] = 1e-12;
  }
}
