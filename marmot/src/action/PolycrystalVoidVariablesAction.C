#include "PolycrystalVoidVariablesAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "Conversion.h"
#include "AddVariableAction.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

const Real PolycrystalVoidVariablesAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<PolycrystalVoidVariablesAction>()
{
  InputParameters params = validParams<Action>();

  MooseEnum familyEnum = AddVariableAction::getNonlinearVariableFamilies();
  params.addParam<MooseEnum>("family", familyEnum, "Specifies the family of FE shape functions to use for this variable");
  MooseEnum orderEnum = AddVariableAction::getNonlinearVariableOrders();
  params.addParam<MooseEnum>("order", orderEnum,  "Specifies the order of the FE shape function to use for this variable");

  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<unsigned int>("void_num", "specifies the number of voids to create");
  params.addParam<Real>("min_void_dist", 1.0, "Minimum distance between voids");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addParam<int>("ICType", 2, "Type of iniitial condition");
  params.addParam<unsigned int>("rand_seed", 12444, "The random seed");

  params.addParam<Real>("gr_radius", 0.0, "Radius of circle grain, if op_num = 2 and ICType = 0.");

  params.addRequiredParam<Real>("max_void_radius", "The Maximum radius of a void");
  params.addParam<Real>("min_void_radius", 0, "The Minimim radius of a void");

  params.addParam<bool>("edge_check", false, "If bubbles too close to the edge will be removed");

  params.addParam<unsigned int>("maxiter", 1000, "Maximum interation to obtain void centers with edge check and grain centers with minimum distance");
  params.addParam<bool>("use_min_grain_separation_dist", false, "Flag for using minimum center to center distance between grains - works only for ICType 2");
  params.addParam<Real>("min_grain_separation_dist", 0.0, "Minimum center to center distance between grains");
  params.addParam<bool>("use_min_edge_dist", false, "Flag for using minimum distance of voids from edge - works only for ICType 2");
  params.addParam<Real>("min_edge_dist", 0.0, "Minimum distance of void centers from edge");

  return params;
}

PolycrystalVoidVariablesAction::PolycrystalVoidVariablesAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{
}

void
PolycrystalVoidVariablesAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolycrystalVoidVariablesAction Object\n";
  Moose::err << "VariableBase: " << _var_name_base
            << "\torder: " << getParam<MooseEnum>("order")
            << "\tfamily: " << getParam<MooseEnum>("family") << std::endl;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    Real scale_factor = getParam<Real>("scaling");

    if (_current_task == "add_variable")
      _problem->addVariable(var_name,
                            FEType(Utility::string_to_enum<Order>(getParam<MooseEnum>("order")),
                                   Utility::string_to_enum<FEFamily>(getParam<MooseEnum>("family"))),
                            scale_factor);

    // Set initial condition
    if (_current_task == "add_ic")
    {
      InputParameters poly_params = _factory.getValidParams("PolycrystalVoidIC");
      poly_params.set<VariableName>("variable") = var_name;
      poly_params.set<unsigned int>("op_num") = _op_num;
      poly_params.set<unsigned int>("op_index") = op;
      poly_params.set<unsigned int>("void_num") = getParam<unsigned int>("void_num");
      poly_params.set<Real>("min_void_dist") = getParam<Real>("min_void_dist");
      poly_params.set<unsigned int>("void_index") = 0; // Always sets grains
      poly_params.set<Real>("gr_radius") = getParam<Real>("gr_radius");
      poly_params.set<Real>("max_void_radius") = getParam<Real>("max_void_radius");
      poly_params.set<Real>("min_void_radius") = getParam<Real>("min_void_radius");
      poly_params.set<int>("ICType") = getParam<int>("ICType");
      poly_params.set<unsigned int>("rand_seed") = getParam<unsigned int>("rand_seed");
      poly_params.set<bool>("edge_check") = getParam<bool>("edge_check");
      poly_params.set<unsigned int>("maxiter") = getParam<unsigned int>("maxiter");
      poly_params.set<bool>("use_min_grain_separation_dist") = getParam<bool>("use_min_grain_separation_dist");
      poly_params.set<Real>("min_grain_separation_dist") = getParam<Real>("min_grain_separation_dist");
      poly_params.set<bool>("use_min_edge_dist") = getParam<bool>("use_min_edge_dist");
      poly_params.set<Real>("min_edge_dist") = getParam<Real>("min_edge_dist");
      _problem->addInitialCondition("PolycrystalVoidIC", name() + "_" + Moose::stringify(op), poly_params);
    }
  }

  // This is where we can now create voids
}
