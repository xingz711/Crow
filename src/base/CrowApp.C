#include "CrowApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

#include "CHChemPotential.h" 
#include "CHTemp.h" 
#include "ACSinteringGrowth.h"
#include "PFDiffusion.h"
#include "PFDiffusionGrowthConst.h"
#include "PFDiffusionGrowth.h"
#include "PFDiffusionGrowth1.h"
#include "PFParticleGrowth.h"
#include "PFEigenStrainMaterial.h"
#include "PFEigenStrainMaterial1.h"
#include "TwoParticleGrainsIC.h"
#include "PFTempMobility.h"

#include "PolycrystalSinteringKernelAction.h"
#include "PolycrystalSinteringMaterialAction.h"
#include "TwoParticleGrainsICAction.h"
#include "TwoParticleDensityIC.h"



//#include "MobilityFn.h" 

template<>
InputParameters validParams<CrowApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

CrowApp::CrowApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  CrowApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  CrowApp::associateSyntax(_syntax, _action_factory);
}

CrowApp::~CrowApp()
{
}


void
CrowApp::registerApps()
{
  registerApp(CrowApp);
}

void
CrowApp::registerObjects(Factory & factory)
{
  // Register any custom objects you have built on the MOOSE Framework
  registerKernel(CHChemPotential);  // <- registration
  registerKernel(CHTemp);
  registerKernel(ACSinteringGrowth);
  
  registerMaterial(PFDiffusion);
  registerMaterial(PFDiffusionGrowthConst);
  registerMaterial(PFDiffusionGrowth);
  registerMaterial(PFDiffusionGrowth1);
  registerMaterial(PFEigenStrainMaterial);
  registerMaterial(PFEigenStrainMaterial1);
  registerMaterial(PFParticleGrowth);
  registerMaterial(PFTempMobility);
  
  registerInitialCondition(TwoParticleGrainsIC);
  registerInitialCondition(TwoParticleDensityIC);
  
  //registerFunction(MobilityFn);
}


void
CrowApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("PolycrystalSinteringKernelAction", "Kernels/PolycrystalSinteringKernel");
  syntax.registerActionSyntax("PolycrystalSinteringMaterialAction", "Materials/PolycrystalSinteringMaterial");
  syntax.registerActionSyntax("TwoParticleGrainsICAction", "ICs/PolycrystalICs/TwoParticleGrainsIC");


  registerAction(PolycrystalSinteringKernelAction, "add_kernel"); 
  registerAction(PolycrystalSinteringMaterialAction, "add_material");  
  registerAction(TwoParticleGrainsICAction, "add_ic");
}
