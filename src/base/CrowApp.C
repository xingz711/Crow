#include "CrowApp.h"
#include "Moose.h"
#include "AppFactory.h"
//#include "ModulesApp.h"
#include "PhaseFieldApp.h"
#include "SolidMechanicsApp.h"
#include "TensorMechanicsApp.h"
//#include "HeatTransferApp.h"
#include "MiscApp.h"


#include "CHChemPotential.h"
#include "CHTemp.h"
#include "ACSinteringGrowth.h"
#include "ACParticleGrowth.h"
#include "VacancySourceTermKernel.h"
#include "RandomVacancySourceTermKernel.h"
#include "VacancyAnnihilationKernel.h"
#include "LangevinNoiseVoid.h"
#include "PFDiffusion.h"
#include "TempDiffusion.h"
#include "PFDiffusionGrowthConst.h"
#include "PFDiffusionGrowth.h"
#include "PFDiffusionGrowth1.h"
#include "PFParticleGrowth.h"
#include "PFEigenStrainMaterial.h"
#include "PFEigenStrainMaterial1.h"
#include "TwoParticleGrainsIC.h"
#include "PFTempMobility.h"
#include "Mobility.h"
#include "RandomVacancySourceTermMaterial.h"
#include "RandomNumberGeneration.h"
#include "ConservedLangevinNoiseVoidSource.h"
#include "BicrystalIC.h"
#include "VacancyRecombinationTermKernel.h"
#include "RigidBodyMotionKernel.h"
#include "SinteringFreeEnergy.h"

#include "PolycrystalSinteringKernelAction.h"
#include "PolycrystalSinteringMaterialAction.h"
#include "TwoParticleGrainsICAction.h"
#include "TwoParticleDensityIC.h"
#include "BicrystalICAction.h"
#include "MultiSmoothParticleIC.h"
#include "MultiSmoothParticleICAction.h"

#include "RandomVoidSourceAux.h"

#include "ConservedUniformVoidSource.h"

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
//  ModulesApp::registerObjects(_factory);
   PhaseFieldApp::registerObjects(_factory);
   SolidMechanicsApp::registerObjects(_factory);
   TensorMechanicsApp::registerObjects(_factory);
//+  HeatTransferApp:: registerObjects(_factory);
   MiscApp::registerObjects(_factory);
//  CombinedApp::registerObjects(_factory);
//+  MooseTestApp::registerObjects(_factory);
   CrowApp::registerObjects(_factory);

   Moose::associateSyntax(_syntax, _action_factory);
//  ModulesApp::associateSyntax(_syntax, _action_factory);
   PhaseFieldApp::associateSyntax(_syntax, _action_factory);
   SolidMechanicsApp::associateSyntax(_syntax, _action_factory);
   TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
//+  HeatTransferApp::associateSyntax(_syntax, _action_factory);
   MiscApp::associateSyntax(_syntax, _action_factory);
//  CombinedApp::associateSyntax(_syntax, _action_factory);
//+  MooseTestApp::associateSyntax(_syntax, _action_factory);
  CrowApp::associateSyntax(_syntax, _action_factory);
}

CrowApp::~CrowApp()
{
}


extern "C" void CrowApp__registerApps() { CrowApp::registerApps(); }
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
  registerKernel(ACParticleGrowth);
  registerKernel(VacancySourceTermKernel);
  registerKernel(RandomVacancySourceTermKernel);
  registerKernel(VacancyAnnihilationKernel);
  registerKernel(ConservedLangevinNoiseVoidSource);
  registerKernel(LangevinNoiseVoid);
  registerKernel(VacancyRecombinationTermKernel);
  registerKernel(RigidBodyMotionKernel);
  registerFunction(RandomNumberGeneration);

  registerMaterial(PFDiffusion);
  registerMaterial(TempDiffusion);
  registerMaterial(PFDiffusionGrowthConst);
  registerMaterial(PFDiffusionGrowth);
  registerMaterial(PFDiffusionGrowth1);
  registerMaterial(PFEigenStrainMaterial);
  registerMaterial(PFEigenStrainMaterial1);
  registerMaterial(PFParticleGrowth);
  registerMaterial(PFTempMobility);
  registerMaterial(Mobility);
  registerMaterial(RandomVacancySourceTermMaterial);
  registerMaterial(SinteringFreeEnergy);

  registerInitialCondition(TwoParticleGrainsIC);
  registerInitialCondition(TwoParticleDensityIC);
  registerInitialCondition(BicrystalIC);
  registerInitialCondition(MultiSmoothParticleIC);
  //registerFunction(MobilityFn);
  registerAux(RandomVoidSourceAux);

  registerUserObject(ConservedUniformVoidSource);


}


void
CrowApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("PolycrystalSinteringKernelAction", "Kernels/PolycrystalSinteringKernel");
  syntax.registerActionSyntax("PolycrystalSinteringMaterialAction", "Materials/PolycrystalSinteringMaterial");
  syntax.registerActionSyntax("TwoParticleGrainsICAction", "ICs/PolycrystalICs/TwoParticleGrainsIC");
  syntax.registerActionSyntax("BicrystalICAction", "ICs/PolycrystalICs/BicrystalIC");
  syntax.registerActionSyntax("MultiSmoothParticleICAction", "ICs/PolycrystalICs/MultiSmoothParticleIC");

  registerAction(PolycrystalSinteringKernelAction, "add_kernel");
  registerAction(PolycrystalSinteringMaterialAction, "add_material");
  registerAction(TwoParticleGrainsICAction, "add_ic");
  registerAction(BicrystalICAction, "add_ic");
  registerAction(MultiSmoothParticleICAction, "add_ic");
}
