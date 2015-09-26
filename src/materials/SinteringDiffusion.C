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
  params.addRequiredParam<Real>("int_width","The interfacial width in the lengthscale of the problem");
  params.addRequiredParam<Real>("GB_energy", "GB energy in J/m2");
  params.addRequiredParam<Real>("surface_energy", "Surface energy in J/m2");
  params.addParam<Real>("GBmob0", 0, "Grain boundary mobility prefactor in m^4/(J*s)");
  params.addRequiredParam<Real>("Em", "Vacancy migration energy in eV");
  params.addParam<Real>("Q", 0, "Grain boundary migration activation energy in eV");
  params.addParam<Real>("Qs", 0, "Surface Diffusion activation energy in eV");
  params.addParam<Real>("Qgb", 0, "GB Diffusion activation energy in eV");

  return params;
}

SinteringDiffusion::SinteringDiffusion(const InputParameters & parameters) :
    Material(parameters),
    _Dvol0(getParam<Real>("Dvol")),
    _Dvap0(getParam<Real>("Dvap")),
    _Dsurf0(getParam<Real>("Dsurf")),
    _Dgb0(getParam<Real>("Dgb")),
    _int_width(getParam<Real>("int_width")),
    _Em(getParam<Real>("Em")),
    _GB_energy(getParam<Real>("GB_energy")),
    _surface_energy(getParam<Real>("surface_energy")),
    _GBmob0(getParam<Real>("GBmob0")),
    _Q(getParam<Real>("Q")),
    _Qs(getParam<Real>("Qs")),
    _Qgb(getParam<Real>("Qgb")),
    _kb(8.617343e-5),

    _rho(coupledValue("rho")),
    _grad_rho(coupledGradient("rho")),
    // _v(coupledValue("v")),
    _T(coupledValue("T")),

    _D(declareProperty<Real>("D")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _dDdc(declareProperty<Real>("dDdc")),
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
SinteringDiffusion::computeQpProperties()
{
  Real GBmob = _GBmob0 * std::exp(-_Q / (_kb * _T[_qp]));
  Real Dvol = _Dvol0 * std::exp(-_Em/(_kb * _T[_qp]));
  Real Dgb = _Dgb0 * std::exp(-_Qgb/(_kb * _T[_qp]));
  Real Dsurf = _Dsurf0 * std::exp(-_Qs/(_kb * _T[_qp]));
  Real Dvap = Dvol / 10.0;

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
  c = c>1.0 ? 1.0 : (c<0.0 ? 0.0 : c);

  Real phi = c * c * c * (10 - 15 * c + 6 * c * c);
  phi = phi>1.0 ? 1.0 : (phi<0.0 ? 0.0 : phi);
  _D[_qp] = Dvol * phi + Dvap * (1.0 - phi) + Dsurf * c * (1-c) + Dgb * SumEtaj;// + _Dvap*(1 - phi) ;

  Real dphidc =  30.0 * c * c * (1 - 2 * c + c * c);
  _dDdc[_qp] = Dvol * dphidc - Dvap * dphidc + Dsurf * (1.0 - 2.0 * c);

  _kappa_c[_qp] =  3.0/4.0 * (2.0 * _surface_energy - _GB_energy) * _int_width;
  _kappa_op[_qp] = 3.0/4.0 * _GB_energy * _int_width;
    // _A[_qp] = (12.0 * _surface_energy - 7.0 * _GB_energy) / _int_width;
    // _B[_qp] = GB_energy / int_width_c;
  _L[_qp] = 4.0/3.0 * GBmob / _int_width;
}
