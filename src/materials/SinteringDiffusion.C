#include "SinteringDiffusion.h"

template<>
InputParameters validParams<SinteringDiffusion>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("Dvol0", 0.01, "Volumetric diffusion coefficient ");
  params.addParam<Real>("Dvap0", 0.001, "Vapor Diffusion ");
  params.addParam<Real>("Dsurf0", 4, "surface diffusion");
  params.addParam<Real>("Dgb0", 0.4, "Grain Boundary diffusion");
  params.addRequiredCoupledVar("rho","phase field variable");
  params.addRequiredCoupledVar("v","array of order parameters");
  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addParam<Real>("length_scale", 1.0e-9,"defines the base length scale of the problem in m");
  params.addParam<Real>("time_scale", 1.0e-9, "Time scale in s, where default is ns");
  params.addRequiredParam<Real>("int_width","The interfacial width in the lengthscale of the problem");
  params.addRequiredParam<Real>("GB_energy", "GB energy in J/m2");
  params.addRequiredParam<Real>("surface_energy", "Surface energy in J/m2");
  params.addParam<Real>("GBmob0", 0, "Grain boundary mobility prefactor in m^4/(J*s)");
  params.addParam<Real>("Qgbm", 0, "Grain boundary migration activation energy in eV");
  params.addRequiredParam<Real>("Qv", "Vacancy migration energy in eV");
  params.addParam<Real>("Qs", 0, "Surface Diffusion activation energy in eV");
  params.addParam<Real>("Qgb", 0, "GB Diffusion activation energy in eV");
  params.addParam<Real>("Vm", 1.5829e-29, "Atomic volume in m^3/atom");
  params.addParam<Real>("GBMobility", -1, "GB mobility input in m^4/(J*s), that overrides the temperature dependent calculation");

  return params;
}

SinteringDiffusion::SinteringDiffusion(const InputParameters & parameters) :
    Material(parameters),
    _Dvol0(getParam<Real>("Dvol0")),
    _Dvap0(getParam<Real>("Dvap0")),
    _Dsurf0(getParam<Real>("Dsurf0")),
    _Dgb0(getParam<Real>("Dgb0")),
    _int_width(getParam<Real>("int_width")),
    _length_scale(getParam<Real>("length_scale")),
    _time_scale(getParam<Real>("time_scale")),
    _Qv(getParam<Real>("Qv")),
    _GB_energy(getParam<Real>("GB_energy")),
    _surface_energy(getParam<Real>("surface_energy")),
    _GBmob0(getParam<Real>("GBmob0")),
    _Q(getParam<Real>("Qgbm")),
    _Qs(getParam<Real>("Qs")),
    _Qgb(getParam<Real>("Qgb")),
    _Vm(getParam<Real>("Vm")),
    _GBMobility(getParam<Real>("GBMobility")),
    _kb(8.617343e-5),

    _rho(coupledValue("rho")),
    _grad_rho(coupledGradient("rho")),
    // _v(coupledValue("v")),
    _T(coupledValue("T")),

    _D(declareProperty<Real>("D")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _dDdc(declareProperty<Real>("dDdc")),
    _M(declareProperty<Real>("M")),
    _dMdc(declareProperty<Real>("dMdc")),
    _L(declareProperty<Real>("L")),
    _kappa_op(declareProperty<Real>("kappa_op"))
{
  // Array of coupled variables is created in the constructor
  _ncrys = coupledComponents("v"); // determine number of grains from the number of names passed in.
  _vals.resize(_ncrys); // Size variable arrays
  _vals_var.resize(_ncrys);

  // Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

void
SinteringDiffusion::computeProperties()
{
  const Real tol = 1.0e-8;
  Real energy_scale = _GB_energy / _length_scale; // energy density scale in J/m^3
  Real int_width_c = _int_width; // The interfacial width is input in the length scale of the problem, so no conversion is necessary
  Real GB_energy = _GB_energy / (energy_scale * _length_scale); // Non-dimensionalized GB energy
  Real surface_energy = _surface_energy / (energy_scale * _length_scale); //Non-dimensionalized surface energy

  Real Dvol0 = _Dvol0 * _time_scale /(_length_scale * _length_scale);
  Real Dsurf0 = _Dsurf0 * _time_scale /(_length_scale * _length_scale);
  Real Dgb0 = _Dgb0 * _time_scale /(_length_scale * _length_scale);

  Real Vm = _Vm / (_length_scale * _length_scale * _length_scale);

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    Real Dvol = Dvol0 * std::exp(-_Qv/(_kb * _T[_qp])); // (_kb * _T[_qp]);
    Real Dgb = Dgb0 * std::exp(-_Qgb/(_kb * _T[_qp])); // (_kb * _T[_qp]);
    Real Dsurf = Dsurf0 * std::exp(-_Qs/(_kb * _T[_qp])); // (_kb * _T[_qp]);
    Real Dvap = Dvol / 10.0;

    Real GBmob;
    if (_GBMobility < 0)
      GBmob = _GBmob0 * std::exp(-_Q / (_kb * _T[_qp])) * _time_scale * energy_scale / _length_scale ;
    else
    {
      // Convert to lengthscale^4/(eV*timescale);;
      GBmob = _GBMobility * _time_scale / (_length_scale * _length_scale * _length_scale * _length_scale);
    }

    Real SumEtaj = 0.0;
    for (unsigned int i = 0; i < _ncrys; ++i)
    {
      for (unsigned int j = 0; j < _ncrys; ++j)
      {
        if(j!=i)
          SumEtaj += (*_vals[i])[_qp] * (*_vals[j])[_qp]; //Sum all other order parameters
      }
    }
    Real c = _rho[_qp];

    if (c < tol)
      c = tol;
    if (c > 1.0 - tol)
      c = 1.0 - tol;
    // c = c>1.0 ? 1.0 : (c<0.0 ? 0.0 : c);

    Real phi = c * c * c * (10.0 - 15.0 * c + 6.0 * c * c);
    phi = phi>1.0 ? 1.0 : (phi<0.0 ? 0.0 : phi);
    _D[_qp] = (Dvol * phi + Dvap * (1.0 - phi) + Dsurf * c * (1.0-c) + Dgb * SumEtaj);// + _Dvap*(1 - phi) ;
    _M[_qp] = _D[_qp] * Vm / (_kb * _T[_qp]);
    Real dphidc =  30.0 * c * c * (1.0 - 2.0 * c + c * c);
    _dDdc[_qp] = Dvol * dphidc - Dvap * dphidc + Dsurf * (1.0 - 2.0 * c);
    _dMdc[_qp] = _dDdc[_qp] * Vm / (_kb * _T[_qp]); // * energy_scale;

    _kappa_c[_qp] =  3.0/4.0 * (2.0 * surface_energy - GB_energy) * int_width_c;
    _kappa_op[_qp] = 3.0/4.0 * GB_energy * int_width_c;
    // _A[_qp] = (12.0 * _surface_energy - 7.0 * _GB_energy) / _int_width;
    // _B[_qp] = GB_energy / int_width_c;

    _L[_qp] = 4.0/3.0 * GBmob / int_width_c;
  }
}
