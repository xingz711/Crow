#include "MooseSyntax.h"
#include "Parser.h"
#include "ActionFactory.h"
// Actions
#include "PolycrystalVoidVariablesAction.h"
#include "PolycrystalBubbleKernelAction.h"
#include "PolyCrystPlasticDFKernelAction.h"
#include "PolyTGradDFKernelAction.h"
#include "PolyGenericDFKernelAction.h"
#include "PolycrystalBCAction.h"
#include "CrystalPlasticityMaterialAction.h"
#include "AvEulerAnglePostprocessorAction.h"
#include "PolycrystalVoronoiVoidICAction.h"
#include "BicrystalCircleGrainVoidICAction.h"
#include "ReconstructionAction.h"
#include "ReconAuxVarICAction.h"
#include "ReconAuxVarUpdateAction.h"
#include "PoreGBPurdueKernelAction.h"
// #include "PolycrystalGrTrackerKernelAction.h"


namespace Marmot
{

void
associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
#undef registerAction
#define registerAction(tplt, action) action_factory.reg<tplt>(stringifyName(tplt), action)

  /**
   * Note: the optional third parameter is used to differentiate which action_name is
   * satisfied based on the syntax encountered for classes which are registered
   * to satisfy more than one action_name
   */
  syntax.registerActionSyntax("AvEulerAnglePostprocessorAction", "Postprocessors/AvEulerAngle");
  syntax.registerActionSyntax("BicrystalCircleGrainVoidICAction", "ICs/PolycrystalICs/BicrystalCircleGrainVoidIC");
  syntax.registerActionSyntax("CrystalPlasticityMaterialAction", "Materials/CrystalPlasticityMaterial");
  syntax.registerActionSyntax("EmptyAction", "ICs/PolycrystalICs");  // placeholder
  syntax.registerActionSyntax("PolycrystalBCAction","BCs/PolycrystalBC");
  syntax.registerActionSyntax("PolycrystalBubbleKernelAction", "Kernels/PolycrystalBubbleKernel");
  syntax.registerActionSyntax("PolycrystalVoidVariablesAction", "Variables/PolycrystalVoidVariables", "add_ic");
  syntax.registerActionSyntax("PolycrystalVoidVariablesAction", "Variables/PolycrystalVoidVariables", "add_variable");
  syntax.registerActionSyntax("PolycrystalVoronoiVoidICAction", "ICs/PolycrystalICs/PolycrystalVoronoiVoidIC");
  syntax.registerActionSyntax("PolyCrystPlasticDFKernelAction", "Kernels/PolyCrystPlasticDFKernel");
  syntax.registerActionSyntax("PolyGenericDFKernelAction", "Kernels/PolyGenericDFKernel");
  syntax.registerActionSyntax("PolyTGradDFKernelAction", "Kernels/PolyTGradDFKernel");
  syntax.registerActionSyntax("PoreGBPurdueKernelAction", "Kernels/PoreGBPurdueKernel");
  syntax.registerActionSyntax("ReconAuxVarICAction", "ICs/PolycrystalICs/ReconAuxVarIC");
  syntax.registerActionSyntax("ReconAuxVarUpdateAction", "AuxKernels/ReconAuxVarUpdate/*");
  syntax.registerActionSyntax("ReconstructionAction", "ICs/PolycrystalICs/ReconstructionIC");
  // syntax.registerActionSyntax("ReconstructionAction", "Variables/ReconstructedVariables");
  // syntax.registerActionSyntax("PolycrystalGrTrackerKernelAction", "Kernels/PolycrystalGrTrackerKernel");

  // Actions
  registerAction(AvEulerAnglePostprocessorAction, "add_postprocessor");
  registerAction(BicrystalCircleGrainVoidICAction, "add_ic");
  registerAction(CrystalPlasticityMaterialAction, "add_material");
  registerAction(PolycrystalBCAction, "add_bc");
  registerAction(PolycrystalBubbleKernelAction, "add_kernel");
  registerAction(PolycrystalVoidVariablesAction, "add_ic");
  registerAction(PolycrystalVoidVariablesAction, "add_variable");
  registerAction(PolycrystalVoronoiVoidICAction, "add_ic");
  registerAction(PolyCrystPlasticDFKernelAction, "add_kernel");
  registerAction(PolyGenericDFKernelAction, "add_kernel");
  registerAction(PolyTGradDFKernelAction, "add_kernel");
  registerAction(PoreGBPurdueKernelAction, "add_kernel");
  registerAction(ReconAuxVarICAction, "add_aux_variable");
  registerAction(ReconAuxVarICAction, "add_ic");
  registerAction(ReconAuxVarUpdateAction, "add_aux_kernel");
  registerAction(ReconstructionAction, "add_ic");
  // registerAction(ReconstructionAction, "add_variable");
  // registerAction(ReconstructionAction, "add_aux_variable");
  // registerAction(PolycrystalGrTrackerKernelAction, "add_kernel");

#undef registerAction
#define registerAction(tplt, action) action_factory.regLegacy<tplt>(stringifyName(tplt), action)

}

}
