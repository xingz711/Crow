/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "PolycrystalBCAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<PolycrystalBCAction>()
{
  InputParameters params = validParams<Action>();
  // params.addRequiredParam<std::string>("variable", "The BC Name used in your model");
  // params.addRequiredParam<std::vector<int> >("boundary", "The boundary number from your input mesh which corresponds to this boundary");
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<std::vector<BoundaryName> >("boundary", "specifies boundary");
  params.addRequiredParam<Real>("value", "gives value for boundary condition");
  params.addRequiredParam<std::string>("type", "Specifies boundary condition type");

  return params;
}

PolycrystalBCAction::PolycrystalBCAction(const InputParameters & params) :
    Action(params),
    _type(getParam<std::string>("type")),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _boundary(getParam<std::vector<BoundaryName> >("boundary")),
    _value(getParam<Real>("value"))
{
}

void
PolycrystalBCAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolycrystalBCBlock Object\n";
  Moose::err << "BC: " << _type
            << "\tname: Polycrystal \n";
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable name
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    // Create BC name
    std::string BC_name = "BC_";
    BC_name.append(var_name);

    InputParameters poly_params = _factory.getValidParams(_type);
    poly_params.set<Real>("value") = _value;
    poly_params.set<std::vector<BoundaryName> >("boundary") = _boundary;
    poly_params.set<NonlinearVariableName>("variable") = var_name;

    _problem->addBoundaryCondition(_type, BC_name, poly_params);
  }
}
