#include "ReconstructionAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

const Real ReconstructionAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<ReconstructionAction>()
{
  InputParameters params = validParams<Action>();
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grain order parameters to create");
  params.addRequiredParam<std::string>("var_name_base","specifies the base name of the variables");
  params.addRequiredParam<FileName>("EBSD_file_name", "Name of the file containing the EBSD data.");
  params.addRequiredParam<Real>("x1", "The x coordinate of the lower left-hand corner of the box");
  params.addRequiredParam<Real>("y1", "The y coordinate of the lower left-hand corner of the box");
  params.addParam<Real>("z1", 0.0, "The z coordinate of the lower left-hand corner of the box");
  params.addRequiredParam<Real>("x2", "The x coordinate of the upper right-hand corner of the box");
  params.addRequiredParam<Real>("y2", "The y coordinate of the upper right-hand corner of the box");
  params.addParam<Real>("z2", 0.0, "The z coordinate of the upper right-hand corner of the box");
  params.addParam<unsigned int>("sd", 3, "An integer representing reference sample direction");
  MooseEnum EBSD_param("eta grn phase rgb");
  params.addParam<MooseEnum>("EBSD_param", EBSD_param, "The data type from EBSD input file");
  return params;
}

ReconstructionAction::ReconstructionAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _scaling(getParam<Real>("scaling")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _eta("eta"),
    _grn("grn"),
    _phase("phase"),
    _rgb("rgb"),
    _EBSD_file_name(getParam<FileName>("EBSD_file_name")),
    _sd(getParam<unsigned int>("sd")),
    _x1(getParam<Real>("x1")),
    _y1(getParam<Real>("y1")),
    _z1(getParam<Real>("z1")),
    _x2(getParam<Real>("x2")),
    _y2(getParam<Real>("y2")),
    _z2(getParam<Real>("z2"))
{
}

void
ReconstructionAction::act()
{
  // Set initial condition for "eta" parameter

  // Loop through order parameters for each grain
  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    // Add variable

    if (_current_task == "add_variable")
    {
      FEType fe_type(FIRST, LAGRANGE);
      _problem->addVariable(var_name, fe_type, _scaling);
    }
    else if (_current_task == "add_ic")
    {
      // Define parameters for ReconstructionIC
      InputParameters poly_params = _factory.getValidParams("ReconstructionIC");
      poly_params.set<VariableName>("variable") = var_name;
      poly_params.set<MooseEnum>("EBSD_param") = _eta;
      poly_params.set<unsigned int>("op_num") = _op_num;
      poly_params.set<unsigned int>("op_index") = op;
      poly_params.set<FileName>("EBSD_file_name") = _EBSD_file_name;
      poly_params.set<unsigned int>("sd") = _sd;
      poly_params.set<Real>("x1") = _x1;
      poly_params.set<Real>("y1") = _y1;
      poly_params.set<Real>("z1") = _z1;
      poly_params.set<Real>("x2") = _x2;
      poly_params.set<Real>("y2") = _y2;
      poly_params.set<Real>("z2") = _z2;

      _problem->addInitialCondition("ReconstructionIC", "Initialize_eta", poly_params);
    }
  }

  if (_current_task == "add_aux_variable")
  {
    FEType fe_type_grn(CONSTANT, MONOMIAL);
    _problem->addAuxVariable(_grn, fe_type_grn);

    FEType fe_type_phase(CONSTANT, MONOMIAL);
    _problem->addAuxVariable(_phase, fe_type_phase);

    FEType fe_type_rgb(CONSTANT, MONOMIAL);
    _problem->addAuxVariable(_rgb, fe_type_rgb);
  }
  else if (_current_task == "add_ic")
  {
    // Set initial condition for "grn" parameter
    {
      InputParameters poly_params = _factory.getValidParams("ReconstructionIC");
      poly_params.set<VariableName>("variable") = _grn;
      poly_params.set<MooseEnum>("EBSD_param") = _grn;
      poly_params.set<unsigned int>("op_num") = _op_num;
      poly_params.set<unsigned int>("op_index") = 0;
      poly_params.set<FileName>("EBSD_file_name") = _EBSD_file_name;
      poly_params.set<unsigned int>("sd") = _sd;
      poly_params.set<Real>("x1") = _x1;
      poly_params.set<Real>("y1") = _y1;
      poly_params.set<Real>("z1") = _z1;
      poly_params.set<Real>("x2") = _x2;
      poly_params.set<Real>("y2") = _y2;
      poly_params.set<Real>("z2") = _z2;

      _problem->addInitialCondition("ReconstructionIC", "Initialize_grn", poly_params);
    }

    // Set initial condition for "phase" parameter
    {
      FEType fe_type(CONSTANT, MONOMIAL);
      _problem->addAuxVariable(_phase, fe_type);
      InputParameters poly_params = _factory.getValidParams("ReconstructionIC");
      poly_params.set<VariableName>("variable") = _phase;
      poly_params.set<MooseEnum>("EBSD_param") = _phase;
      poly_params.set<unsigned int>("op_num") = _op_num;
      poly_params.set<unsigned int>("op_index") = 0;
      poly_params.set<FileName>("EBSD_file_name") = _EBSD_file_name;
      poly_params.set<unsigned int>("sd") = _sd;
      poly_params.set<Real>("x1") = _x1;
      poly_params.set<Real>("y1") = _y1;
      poly_params.set<Real>("z1") = _z1;
      poly_params.set<Real>("x2") = _x2;
      poly_params.set<Real>("y2") = _y2;
      poly_params.set<Real>("z2") = _z2;

      _problem->addInitialCondition("ReconstructionIC", "Initialize_phase", poly_params);
    }

    // Set initial condition for "rgb" parameter
    {
      FEType fe_type(CONSTANT, MONOMIAL);
      _problem->addAuxVariable(_rgb, fe_type);
      InputParameters poly_params = _factory.getValidParams("ReconstructionIC");
      poly_params.set<VariableName>("variable") = _rgb;
      poly_params.set<MooseEnum>("EBSD_param") = _rgb;
      poly_params.set<unsigned int>("op_num") = _op_num;
      poly_params.set<unsigned int>("op_index") = 0;
      poly_params.set<FileName>("EBSD_file_name") = _EBSD_file_name;
      poly_params.set<unsigned int>("sd") = _sd;
      poly_params.set<Real>("x1") = _x1;
      poly_params.set<Real>("y1") = _y1;
      poly_params.set<Real>("z1") = _z1;
      poly_params.set<Real>("x2") = _x2;
      poly_params.set<Real>("y2") = _y2;
      poly_params.set<Real>("z2") = _z2;

      _problem->addInitialCondition("ReconstructionIC", "Initialize_rgb", poly_params);
    }
  }
}
