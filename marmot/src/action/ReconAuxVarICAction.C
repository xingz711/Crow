#include "ReconAuxVarICAction.h"
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
InputParameters validParams<ReconAuxVarICAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  params.addParam<unsigned int>("sd", 3, "An integer representing reference sample direction");
  return params;
}

ReconAuxVarICAction::ReconAuxVarICAction(const InputParameters & params) :
    Action(params),
    _ebsd_reader_name(getParam<UserObjectName>("ebsd_reader")),
    _sd(getParam<unsigned int>("sd"))
{
}

void
ReconAuxVarICAction::act()
{
  // get all possible data_name values
  std::vector<std::string> vars = EBSDAccessFunctors::getPointDataFieldType().getNames();

  if (_current_task == "add_aux_variable")
  {
    // Create AuxVariables

    FEType fe_type(CONSTANT, MONOMIAL);
    for (std::vector<std::string>::iterator i = vars.begin(); i != vars.end(); ++i)
      _problem->addAuxVariable(*i, fe_type);

    _problem->addAuxVariable("rgb", fe_type);
  }
  else if (_current_task == "add_ic")
  {
    // Set Initial Conditions

    for (std::vector<std::string>::iterator i = vars.begin(); i != vars.end(); ++i)
    {
      InputParameters poly_params = _factory.getValidParams("ReconAuxVarIC");
      poly_params.set<VariableName>("variable") = *i;
      poly_params.set<UserObjectName>("ebsd_reader") = _ebsd_reader_name;
      poly_params.set<MooseEnum>("data_name") = *i;
      _problem->addInitialCondition("ReconAuxVarIC", "Initialize_" + (*i), poly_params);
    }

    InputParameters poly_params = _factory.getValidParams("Recon2RGBIC");
    poly_params.applyParameters(_pars);
    poly_params.set<VariableName>("variable") = "rgb";
    poly_params.set<UserObjectName>("ebsd_reader") = _ebsd_reader_name;
    poly_params.set<unsigned int>("sd") = _sd;
    _problem->addInitialCondition("Recon2RGBIC", "Initialize_RGB", poly_params);
  }
}
