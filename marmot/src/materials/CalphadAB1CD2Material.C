#include "CalphadAB1CD2Material.h"

template<>
InputParameters validParams<CalphadAB1CD2Material>()
{
  InputParameters params = validParams<CalphadBaseMaterial>();
  params.addClassDescription("Derivative free energy material using the CALPHAD AB1CD2 formulation.");
  params.addRequiredRangeCheckedParam<std::vector<Real> >("L0", "L0_size = 2", "Redlich-Kister L0 polynomial coefficients (2 values)");
  params.addRequiredRangeCheckedParam<std::vector<Real> >("L1", "L1_size = 2", "Redlich-Kister L1 polynomial coefficients (2 values)");
  params.addRequiredRangeCheckedParam<std::vector<Real> >("pure_ep1", "pure_ep1_size = 5", "Coefficients of pure endpoint at low composition in the first phase");
  return params;
}

CalphadAB1CD2Material::CalphadAB1CD2Material(const InputParameters & parameters) :
    CalphadBaseMaterial(parameters),
    _L0(getParam<std::vector<Real> >("L0")),
    _L1(getParam<std::vector<Real> >("L1")),
    _pure_ep1(getParam<std::vector<Real> >("pure_ep1"))
{
  // First Lattice G-HSER
  EBFunction L1GminusHser;
  L1GminusHser(_T) = _GTemperature(_ep_lo[0], _ep_lo[1], _ep_lo[2], _ep_lo[3], _ep_lo[4], _T);

  // Second Lattice G-HSER
  EBFunction L2GminusHser;
  L2GminusHser(_T) =   _mix[0] + _mix[1]*_T + _mix[2]*_T*log(_T)
                       + _GTemperature(_pure_ep1[0], _pure_ep1[1], _pure_ep1[2], _pure_ep1[3], _pure_ep1[4], _T)
                       + _GTemperature(_ep_hi[0], _ep_hi[1], _ep_hi[2], _ep_hi[3], _ep_hi[4], _T);

  // Reference free energy
  _GRef(_c,_T) = 0.5 * ((2 - 3*_c) * L1GminusHser(_T) + _c * L2GminusHser(_T));

  // Ideal solution
  _GIdeal(_c,_T) = _R*_T*(_c*plog(_c/(2 - 2*_c), _plog_tol) + (2 - 3*_c)*plog((2 - 3*_c)/(2 - 2*_c), _plog_tol));

  // Excess free energy
  EBFunction GExBase;
  EBTerm L0("L0"), L1("L1");
  GExBase(L0,L1, _c) = (3*_c*_c - 2*_c)/(4*(_c - 1)*(_c - 1))*((_c - 1)*L0 + (1 - 2*_c)*L1);
  _GEx(_c,_T) = GExBase(_L0[0] + _L0[1]*_T, _L1[0] + _L1[1]*_T, _c);

  // set up free energy and derivatives
  finalize();
}
