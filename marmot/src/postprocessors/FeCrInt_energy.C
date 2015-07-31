#include "FeCrInt_energy.h"

template<>
InputParameters validParams<FeCrInt_energy>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<PostprocessorName>("alphaConc", "Alpha phase concentration");
  params.addRequiredParam<PostprocessorName>("alphaPrimeConc", "Alpha prime phase concentration");
  params.addRequiredParam<PostprocessorName>("F_alpha_density", "Alpha free energy extreme density");
  params.addRequiredParam<PostprocessorName>("F_alphaPrime_density", "Alpha prime free energy extreme density");
  params.addRequiredParam<PostprocessorName>("Tot_F", "Total free energy within the system");
  params.addRequiredParam<Real>("Temperature", "The temperature the alloy resides at in Kelvin");
  // params.addRequiredCoupledVar("Temperature", "The temperature the alloy resides at in Kelvin");
  return params;
}

FeCrInt_energy::FeCrInt_energy(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _alphaConc(getPostprocessorValue("alphaConc")),
    _alphaPrimeConc(getPostprocessorValue("alphaPrimeConc")),
    _FreeAlpha_density(getPostprocessorValue("F_alpha_density")),
    _FreeAlphaPrime_density(getPostprocessorValue("F_alphaPrime_density")),
    _Tot_F(getPostprocessorValue("Tot_F")),
    _T(getParam<Real>("Temperature"))
    // _T(coupledValue("Temperature"))
{
}

void
FeCrInt_energy::initialize()
{
}

void
FeCrInt_energy::execute()
{
}

PostprocessorValue
FeCrInt_energy::getValue()
{
  /**
   * Recall that _FreeAlpha and _FreeAlphaPrime are energy densities!!!!!! (look at the picture).
   * Therefore one has to multiply the densities by the length of the alpha (int_point) or alpha' (1-int_point) block and the width of the strip
   * The width of the strip is 1.0
   */

  Real interfacialPoint = (0.50-_alphaPrimeConc)/(_alphaConc - _alphaPrimeConc);
  Real AlphaFree_energy_block = _FreeAlpha_density * interfacialPoint * 10.0 * 1.0;
  Real AlphaPrimeFree_energy_block = _FreeAlphaPrime_density * (1 - interfacialPoint) * 10.0 * 1.0;
  Real System_Interfacial_energy = _Tot_F - (AlphaPrimeFree_energy_block + AlphaFree_energy_block);
  Real Schwen_Int_energy_density = 2.7552425208 - (0.0006809487 * _T) - (2.67099151e-7 * std::pow(_T, 2.0)); // eV/( nm^2) agree Dependent precipitate on size

  // Multiply Schwen_int_Density by the area of the alpha block which I assume to be one big precipitate
  Real Schwen_Int_energy = Schwen_Int_energy_density * (1 - interfacialPoint) * 10.0 * 1.0;

  Real Energy_difference = Schwen_Int_energy - System_Interfacial_energy; // disagree

  /*
  _console << " interfacial Point " << interfacialPoint
              << " Alpha Free_energy_block " << AlphaFree_energy_block
              << " Alpha Prime Free_energy_block " << AlphaPrimeFree_energy_block
              << " System_Interfacial_energy " << System_Interfacial_energy
              << " Schwen_Int_energy " << Schwen_Int_energy
              << " Energy_difference " << Energy_difference
              << " Tot_F " << _Tot_F << '\n';
  _console << "Tot_F" << _Tot_F << " " << AlphaFree_energy_block << " " << AlphaPrimeFree_energy_block << std::endl;
  */

  return Energy_difference;
}
