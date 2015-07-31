#include "CalphadAB1CD1Material.h"

template<>
InputParameters validParams<CalphadAB1CD1Material>()
{
  InputParameters params = validParams<CalphadBaseMaterial>();
  params.addClassDescription("Derivative free energy material using the CALPHAD AB1CD1 formulation.");
  return params;
}

CalphadAB1CD1Material::CalphadAB1CD1Material(const InputParameters & parameters) :
    CalphadBaseMaterial(parameters)
{
  // First Lattice G-HSER
  EBFunction L1GminusHser;
  L1GminusHser(_T) = _GTemperature(_ep_lo[0], _ep_lo[1], _ep_lo[2], _ep_lo[3], _ep_lo[4], _T);

  // Second Lattice G-HSER
  EBFunction L2GminusHser;
  L2GminusHser(_T) =   _mix[0] + _mix[1]*_T
                       + _GTemperature(_ep_lo[0], _ep_lo[1], _ep_lo[2], _ep_lo[3], _ep_lo[4], _T)
                       + 0.5*_GTemperature(_ep_hi[0], _ep_hi[1], _ep_hi[2], _ep_hi[3], _ep_hi[4], _T);

  // Reference free energy
  _GRef(_c,_T) = (1 - 2*_c)*L1GminusHser(_T) + _c*L2GminusHser(_T);

  // Ideal solution
  _GIdeal(_c,_T) = _R*_T*(_c*plog(_c/(1 - _c), _plog_tol) + (1 - 2*_c)*plog((1 - 2*_c)/(1 - _c), _plog_tol));

  // set up free energy and derivatives
  finalize();
}
