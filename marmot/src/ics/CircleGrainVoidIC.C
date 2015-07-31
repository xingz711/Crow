#include "CircleGrainVoidIC.h"

template<>
InputParameters validParams<CircleGrainVoidIC>()
{
  InputParameters params = validParams<MultiSmoothCircleIC>();

  MooseEnum structure_options("grains voids");
  params.addRequiredParam<MooseEnum>("structure_type", structure_options, "Which structure type is being initialized, grains or voids");
  params.addRequiredParam<unsigned int>("op_num", "Number of order parameters");
  params.addParam<unsigned int>("op_index",0,"The index for the current order parameter, not needed if sructure_type = voids");
  params.addParam<unsigned int>("rand_seed", 12444, "The random seed");
  params.addParam<bool>("columnar_3D", false, "3D microstructure will be columnar in the z-direction?");
  params.addRequiredParam<Point>("grain_center", "Center point of circle grain");
  params.addRequiredParam<Real>("grain_radius", "Radius of the circular grain");

  return params;
}

CircleGrainVoidIC::CircleGrainVoidIC(const InputParameters & parameters) :
    MultiSmoothCircleIC(parameters),
    _structure_type(getParam<MooseEnum>("structure_type")),
    _op_num(getParam<unsigned int>("op_num")),
    _op_index(getParam<unsigned int>("op_index")),
    _columnar_3D(getParam<bool>("columnar_3D")),
    _grain_center(getParam<Point>("grain_center")),
    _grain_radius(getParam<Real>("grain_radius"))
{
}

void
CircleGrainVoidIC::initialSetup()
{
  if (_invalue < _outvalue)
    mooseError("CircleGrainVoidIC requires that the voids be represented with invalue > outvalue");

  if (_op_num <= _op_index)
    mooseError("op_index is too large in CircleGrainVoidIC");

  //Initialize seed
  MooseRandom::seed(getParam<unsigned int>("rand_seed"));
  //Calls computeCircleCenters and computeCircleRadii
  MultiSmoothCircleIC::initialSetup();
}

void
CircleGrainVoidIC::computeCircleCenters()
{
  _centers.resize(_numbub);

  for (unsigned int v = 0; v < _numbub; ++v)
  {
    bool too_close;
    do
    {
      too_close = false;

      //Randomly generate angles
      Real theta = 2.0*libMesh::pi*MooseRandom::rand();
      Real phi = libMesh::pi/2.0;
      if (_range(2) > 0.0 && !_columnar_3D) //3D spherical grain
        phi = libMesh::pi*MooseRandom::rand();

      //Set center values
      _centers[v](0) = _grain_center(0) + _grain_radius*std::cos(theta)*std::sin(phi);
      _centers[v](1) = _grain_center(1) + _grain_radius*std::sin(theta)*std::sin(phi);
      if (_range(2) > 0.0)
      {
        if (_columnar_3D)
          _centers[v](2) = _bottom_left(2) + _radii[v] + (_range(2) - 2*_radii[v])*MooseRandom::rand();
        else
          _centers[v](2) = _grain_center(2) + _grain_radius*std::cos(phi);
      }

      //Check to make sure centers are greater than _bubspac away from each other
      for (unsigned int i = 0; i < v; ++i)
      {
        Real distance = _mesh.minPeriodicDistance(_var.number(), _centers[v], _centers[i]);
        if (distance < _bubspac)
        {
          too_close = true;
          break;
        }
      }
    }
    while (too_close);
  }
}

Real
CircleGrainVoidIC::value(const Point & p)
{
  Real value = 0.0;

  Real void_value = MultiSmoothCircleIC::value(p);
  //Save in and out values for voids
  Real v_invalue = _invalue;
  Real v_outvalue = _outvalue;

  //Change in and out values for grains
  if (_op_index == 0)
  {
    _invalue = 1.0;
    _outvalue = 0.0;
  }
  if (_op_index == 1)
  {
    _invalue = 0.0;
    _outvalue = 1.0;
  }
  //Change 3D_spheres to set center grain behavior
  bool t3D_spheres = _3D_spheres;
  _3D_spheres = !_columnar_3D;

  Real grain_value = computeCircleValue(p, _grain_center, _grain_radius);
  //Change in and out values back for voids
  _invalue = v_invalue;
  _outvalue = v_outvalue;

  //Change 3D_spheres back for the voids
  _3D_spheres = t3D_spheres;

  switch (_structure_type)
  {
    case 0: //assigning values for grains (order parameters)
      if (grain_value == 0.0) //Not in this grain
        value = grain_value;
      else //In this grain, but might be in a void
        if (void_value == v_outvalue) //Not in a void
          value = grain_value;
        else if (void_value > v_outvalue && void_value < v_invalue)  //On void interface
        {
          Real grain_value2 = 1.0 - (void_value - v_outvalue)/(v_invalue - v_outvalue);
          if (grain_value < grain_value2) //Transition between void and grain interfaces
            value = grain_value;
          else
            value = grain_value2;
        }
        else if (void_value == v_invalue) //In a void, so op = 0
          value = 0.0;
      break;

    case 1: //assigning values for voids (concentration)
      value = void_value;
      break;
  }

  return value;
}

RealGradient
CircleGrainVoidIC::gradient(const Point & p)
{
  RealGradient gradient = Gradient(0.0, 0.0, 0.0);
  RealGradient void_gradient = MultiSmoothCircleIC::gradient(p);

  //Order parameter assignment assumes zero gradient (sharp interface)
  switch (_structure_type)
  {
    case 1: //assigning gradient for voids
      gradient = void_gradient;
      break;
  }

  return gradient;
}



