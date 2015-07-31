#include "CalphadBaseMaterial.h"

template<>
InputParameters validParams<CalphadBaseMaterial>()
{
  InputParameters params = validParams<DerivativeParsedMaterialHelper>();
  params.addClassDescription("Derivative free energy material G(c,T) using the CALPHAD formulation.");

  params.addRequiredRangeCheckedParam<std::vector<Real> >("ep_lo", "ep_lo_size = 5", "Coeffs of pure endpoint at low composition (5 values)");
  params.addRequiredRangeCheckedParam<std::vector<Real> >("ep_hi", "ep_hi_size = 5", "Coeffs of pure endpoint at high composition (5 values)");

  params.addRequiredRangeCheckedParam<std::vector<Real> >("mix", "mix_size = 3", "Mixture coefficients (3 values)");

  //Default in J/mol-K
  params.addParam<Real>("gas_constant", 8.3144621, "Universal gas constant");
  params.addParam<Real>("plog_tol", 1e-6, "Tolerance used in the plog function to keep natural log terms from going to infinity");
  params.addParam<Real>("scaling_factor", 1.0, "Scaling factor that will be multiplied by the Calphad free energy. Used for molar volume or to change units");

  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredCoupledVar("T", "Temperature");
  return params;
}

CalphadBaseMaterial::CalphadBaseMaterial(const InputParameters & parameters) :
    DerivativeParsedMaterialHelper(parameters),
    _ep_lo(getParam<std::vector<Real> >("ep_lo")),
    _ep_hi(getParam<std::vector<Real> >("ep_hi")),
    _mix(getParam<std::vector<Real> >("mix")),
    _plog_tol(getParam<Real>("plog_tol")),
    _scaling_factor(getParam<Real>("scaling_factor")),
    _c("c"),
    _T("T"),
    _R(getParam<Real>("gas_constant"))
{
  //
  // prepare sub expressions
  //

  // Define the temperature expansion utility function
  EBTerm A("A"), B("B"), C("C"), D("D"), E("E");
  _GTemperature(A,B,C,D,E, _T) = A + B*_T + C*_T*log(_T) + D*_T*_T + E/_T;

  // Excess free energy
  _GEx(_c,_T) = 0;
}

void
CalphadBaseMaterial::finalize()
{
  // Free energy
  EBFunction G;
  G(_c,_T) = _scaling_factor*(_GRef(_c,_T) + _GIdeal(_c,_T) + _GEx(_c,_T));
  functionParse(G);
}
