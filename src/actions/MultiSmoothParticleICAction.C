/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "MultiSmoothParticleICAction.h"
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

const Real MultiSmoothParticleICAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<MultiSmoothParticleICAction>()
{
  InputParameters params = validParams<Action>();

  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<unsigned int>("op_num", "Number of order parameters");
  params.addRequiredParam<std::vector<Real> >("x_positions", "The x-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("y_positions", "The y-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("z_positions", "The z-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("radii", "The radius for each circle");
  params.addParam<Real>("int_width", 0.0, "The interfacial width of the void surface.  Defaults to sharp interface");
  params.addParam<bool>("3D_spheres", true, "in 3D, whether the objects are spheres or columns");
  //params.addRequiredParam<unsigned int>("numbub", "The number of bubbles to be placed on GB");
  //params.addRequiredParam<Real>("bubspac", "minimum spacing of bubbles, measured from center to center");
  //params.addParam<unsigned int>("rand_seed", 2000, "random seed");
  //params.addParam<unsigned int>("numtries", 1000, "The number of tries");
  //params.addRequiredParam<Real>("radius", "Mean radius value for the circels");
  //params.addParam<Real>("radius_variation", 0.0, "Plus or minus fraction of random variation in the bubble //radius for uniform, standard deviation for normal");
  //MooseEnum rand_options("uniform normal none","none");
  //params.addParam<MooseEnum>("radius_variation_type", rand_options, "Type of distribution that random circle //radii will follow");
  //params.addParam<bool>("avoid_bounds", true, "Don't place any bubbles on the simulation cell boundaries");


  return params;
}

MultiSmoothParticleICAction::MultiSmoothParticleICAction(InputParameters params) :
    Action(params),
    _var_name_base(getParam<std::string>("var_name_base")),
    _op_num(getParam<unsigned int>("op_num")),
    _x_positions(getParam<std::vector<Real> >("x_positions")),
    _y_positions(getParam<std::vector<Real> >("y_positions")),
    _z_positions(getParam<std::vector<Real> >("z_positions")),
    _input_radii(getParam<std::vector<Real> >("radii")),
    _int_width(getParam<Real>("int_width")),
    _3D_spheres(getParam<bool>("3D_spheres")),
    _num_dim(_3D_spheres ? 3 : 2)
    //_numbub(getParam<unsigned int>("numbub")),
    //_bubspac(getParam<Real>("bubspac")),
    //_numtries(getParam<unsigned int>("numtries")),
    //_radius(getParam<Real>("radius")),
    //_radius_variation(getParam<Real>("radius_variation")),
    //_radius_variation_type(getParam<MooseEnum>("radius_variation_type")),
    //_avoid_bounds(getParam<bool>("avoid_bounds"))
{
}

void
MultiSmoothParticleICAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the MultiSmoothParticleICAction Object\n";
#endif

  // Loop through the number of order parameters
  for (unsigned int op = 0; op < _op_num; op++)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());
    //_radii.resize(_op_num);
    //Set parameters for BoundingBoxIC
    InputParameters poly_params = _factory.getValidParams("SmoothCircleIC");
    poly_params.set<VariableName>("variable") = var_name;
    //poly_params.set<unsigned int>("numbub") = _op_num;
    poly_params.set<Real>("x1") = _x_positions[op];
    poly_params.set<Real>("y1") = _y_positions[op];
    if(_num_dim == 3)
    {
    poly_params.set<Real>("z1") = _z_positions[op];
    }

    poly_params.set<Real>("radius") = _input_radii[op];
    poly_params.set<unsigned int>("op_index") = op;
    poly_params.set<Real>("int_width") = _int_width;
    poly_params.set<bool>("3D_spheres") = _3D_spheres;
    //poly_params.set<unsigned int>("rand_seed") = getParam<unsigned int>("rand_seed");
    //poly_params.set<unsigned int>("numbub") = _numbub ;
    //poly_params.set<Real>("bubspac") = _bubspac ;
    //poly_params.set<unsigned int>("numtries") = _numtries;
    //poly_params.set<Real>("radius") = _radius;
    //poly_params.set<Real>("radius_variation") = _radius_variation;
    //poly_params.set<MooseEnum>("radius_variation_type") = _radius_variation_type;
    //poly_params.set<bool>("avoid_bounds") = _avoid_bounds;
    //unsigned int n = 0;
    //if(n == op)
    //{
      //Values for circle grain
      poly_params.set<Real>("invalue") = 1.0;
      poly_params.set<Real>("outvalue") = 0.0;
    //}
    

    //Add initial condition
    _problem->addInitialCondition("SmoothCircleIC", "InitialCondition", poly_params);
  }
}
