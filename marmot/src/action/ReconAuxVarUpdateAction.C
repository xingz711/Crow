#include "ReconAuxVarUpdateAction.h"

#include "AddKernelAction.h"
#include "Action.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "EBSDReader.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"


template<>
InputParameters validParams<ReconAuxVarUpdateAction>()
{
  InputParameters params = validParams<Action>();
  MooseEnum Aux_param("phi1 PHI phi2 grn phase sym rgb op");
  params.addParam<MooseEnum>("Aux_param", Aux_param, "Data type containing the AuxVariable names");
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  params.addParam<unsigned int>("sd", 3, "An integer representing reference sample direction");
  return params;
}

ReconAuxVarUpdateAction::ReconAuxVarUpdateAction(const InputParameters & params) :
    Action(params),
    _sd(getParam<unsigned int>("sd")),
    _phi1("phi1"),
    _PHI("PHI"),
    _phi2("phi2"),
    _grn("grn"),
    _phase("phase"),
    _sym("sym"),
    _rgb("rgb"),
    _op("op")
{
}

void
ReconAuxVarUpdateAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the ReconAuxVarUpdateAction Object\n";
#endif

  Moose::out << "Test 1" << std::endl;

  MultiMooseEnum execute_options(SetupInterface::getExecuteOptions());
  execute_options = "timestep";

  // Create AuxKernels
  {
    // Update evolved "phi1" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _phi1;
      params.set<MooseEnum>("Aux_param") = _phi1;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "PHI" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _PHI;
      params.set<MooseEnum>("Aux_param") = _PHI;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "phi2" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _phi2;
      params.set<MooseEnum>("Aux_param") = _phi2;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "grn" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _grn;
      params.set<MooseEnum>("Aux_param") = _grn;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "phase" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _phase;
      params.set<MooseEnum>("Aux_param") = _phase;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "sym" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _sym;
      params.set<MooseEnum>("Aux_param") = _sym;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }

    // Update evolved "rgb" AuxVariable
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _rgb;
      params.set<MooseEnum>("Aux_param") = _rgb;
      params.set<unsigned int>("sd") = _sd;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);

    }

    // Update evolved "op" AuxVariable (a.k.a _assigned_order_parameter)
    {
      InputParameters params = _factory.getValidParams("ReconAuxVarUpdate");
      params.applyParameters(_pars);
      params.set<AuxVariableName>("variable") = _op;
      params.set<MooseEnum>("Aux_param") = _op;
      params.set<MultiMooseEnum>("execute_on") = execute_options;
      _problem->addAuxKernel("ReconAuxVarUpdate", "variable", params);
    }
  }
}
