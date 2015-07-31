#include "MesoUO2.h"

template<>
InputParameters validParams<MesoUO2>()
{
  InputParameters params = validParams<Material>();
  // Reference temperature for Fink's material property fits is t_ref = 273.0 K
  params.set<Real>("t_ref") = 273.0;

  params.addCoupledVar("eta", 0.0, "Order Parameter");
  params.addRequiredCoupledVar("temp", "Temperature");
  params.addCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<Real>("length_scale", 1.0e-8, "Lengthscale of model");
  params.addParam<Real>("Gas_conductivity", 0.152, "Thermal conductivity in the pore in W/m-K, defaults to He");
  params.addParam<bool>("LANL_vals", false, "You wish to use the LANL values for thermal conductivity of UO2?");
  params.addParam<unsigned int>("GBtype", 1, "Number corresponding to the GB type: 0=no GB, 1=sym tilt, 2=transl tilt, 3=transl twist");
  params.addParam<Real>("GB_width", 1.0, "Grain boundary width");
  params.addParam<int>("kstep", 0, "flag corresponding to the GB conductivity type: 0=old model, 1=Paul&Karthik model");
  params.addParam<Real>("GB_cond", 0.0001, "conductivity in the GB for Paul & Karthik's model");
  params.addParam<Real>("ko", 4.5, "Bulk conductivity override (for Paul & Karthik's model)");

  return params;
}

MesoUO2::MesoUO2(const InputParameters & parameters) :
    Material(parameters),
    _has_v(isCoupled("v")),
    _length_scale(getParam<Real>("length_scale")),
    _LANL_vals(getParam<bool>("LANL_vals")),
    _Gas_conductivity(getParam<Real>("Gas_conductivity")),
    _GBtype(getParam<unsigned int>("GBtype")),
    _GB_width(getParam<Real>("GB_width")),
    _kstep(getParam<int>("kstep")),
    _GB_cond(getParam<Real>("GB_cond")),
    _ko(getParam<Real>("ko")),
    _temp(coupledValue("temp")),
    _eta(coupledValue("eta")),
    _thermal_conductivity(declareProperty<Real>("thermal_conductivity"))
{
  if (_has_v)
  {
    // Assign grain order parameters
    _n = coupledComponents("v");
    _vals.resize(_n);

    for (unsigned int i = 0; i < _n; ++i)
    {
      if (_kstep == 0) _vals[i] = &coupledValueOld("v", i);
      if (_kstep == 1) _vals[i] = &coupledValue("v", i);
    }
  }
  else
    _n = 0;
}

void
MesoUO2::computeProperties()
{
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // GB-defining Parameters

    Real f_i = 0.0;
    Real bnds = 1.0;

    if (_has_v)
    {
      bnds = 0.0;

      for (unsigned int i = 0; i < _n; ++i)
      {
        bnds += (*_vals[i])[_qp] * (*_vals[i])[_qp];
        for (unsigned int j = i + 1; j < _n; ++j)
          f_i += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      }
    }

    // Define the Bulk Conductivity (w/ Temp. dependence)

    Real UO2_cond = 0.0;
    if (_temp[_qp] > 0.0)
    {
      if (_LANL_vals)
      {
        Real x=_temp[_qp]/1000;
        Real a = 4.5192, b= 0.24409, c = 0.666, d = 0.7014, e = -0.10223;
        UO2_cond = a/(b + c*x) + (d / std::pow(x, 2.5)) * std::exp(-e / x); // W/mK
      }
      else
        UO2_cond = 1./(.0375+2.165e-4*_temp[_qp]) + 4.715e9/(_temp[_qp] * _temp[_qp]) * std::exp(-16361./_temp[_qp]);
    }

    Real local_cond = 0.0; // The output thermal conductivity
    Real Gas_cond = _Gas_conductivity; // Thermal cond
    Real GB_mult = 0.0; // No GB

    // Define the GB Conductivity (using LANL values)

    if (_GBtype == 1) // For sigma 5 symmetric tilt
    {
      Real x=_temp[_qp]/1000;
      if (x<=0.8)
        GB_mult = -12.92*x + 14.806;
      else
        GB_mult = -3.1286*x + 6.9729;
    }
    else if (_GBtype == 2) // For sigma 5 translated tilt
    {
      Real x=_temp[_qp]/1000;
      if (x<=0.8)
        GB_mult = -12.28*x + 13.324;
      else
        GB_mult = -2.7857*x + 5.7286;
    }
    else if (_GBtype == 3) // For sigma 5 translated twist
    {
      Real x=_temp[_qp]/1000;
      if (x<=0.8)
        GB_mult = -13.6*x + 14.48;
      else
        GB_mult = -2.8286*x + 5.8629;
    }
    else if (_GBtype == 4) // For amorphous, random boundary
    {
      Real x=_temp[_qp]/1000;
      if (x<=0.8)
        GB_mult = -13.26*x + 14.568;
      else
        GB_mult = -3.2429*x + 6.5543;
    }

    Real bulk_cond = UO2_cond * (1.0 - GB_mult*f_i);
    local_cond = bulk_cond * (1.0 - _eta[_qp]*_eta[_qp]) + _eta[_qp]*_eta[_qp] * Gas_cond;

    if (_eta[_qp] > 0.9999)
      local_cond = Gas_cond;

    // Define the GB Conductivity (Karthik & Paul's model)

    if (_kstep == 1)
    {
      Real p12 = 0;

      if (_has_v)
      {
        for (unsigned int i = 0; i < _n; ++i)
          for (unsigned int j = i + 1; j < _n; ++j)
            p12 += (*_vals[i])[_qp] * (*_vals[j])[_qp];
      }

      // UO2_cond = _ko;
      if (bnds > 0.90)
        local_cond = UO2_cond;

      if (bnds < 0.90)
      {
        if (p12 < 0.001) local_cond = UO2_cond * (1.0 - _eta[_qp]*_eta[_qp]) + Gas_cond * _eta[_qp]*_eta[_qp];
        if (p12 > 0.001) local_cond = UO2_cond;
        if (p12 > 0.01)  local_cond = _GB_cond;
      }

      if (bnds < 0.05)
        local_cond = Gas_cond;

      if (local_cond <= 0.0)
        local_cond = Gas_cond;
    }

    // Rescale the conductivity

    _thermal_conductivity[_qp] = local_cond * _length_scale; // Thermal conductivity in W/(10nm-K)
  }
}
