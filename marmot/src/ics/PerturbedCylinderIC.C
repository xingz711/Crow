#include "PerturbedCylinderIC.h"

template<>
InputParameters validParams<PerturbedCylinderIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addClassDescription("Initial condition to set up a Cylinder perturbed by a sinusoid to simulate Rayleigh instability.");
  params.addRequiredParam<Real>("x1", "The x coordinate of the cylinder axis in the x-y plane");
  params.addRequiredParam<Real>("y1", "The y coordinate of the cylinder axis in the x-y plane");
  params.addRequiredParam<Real>("radius", "The initial radius of the unperturbed cylinder");
  params.addRequiredParam<Real>("cyl_bottom", "The z-coordinate of the bottom of the cylinder");
  params.addRequiredParam<Real>("cyl_top", "The z-coordinate of the top of the cylinder");
  MooseEnum var_type("conserved nonconserved");
  params.addRequiredParam<MooseEnum>("var_type", var_type, "The index for the current variable");
  params.addRequiredParam<unsigned int>("ncycl", "The number of periods/cycles along z-axis");
  params.addRequiredParam<Real>("int_width", "The initial width of the interface");
  params.addRequiredParam<Real>("amplitude", "The amplitude of the sinusoidal perturbation");

  return params;
}

PerturbedCylinderIC::PerturbedCylinderIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _mesh(_fe_problem.mesh()),
    _var_type(getParam<MooseEnum>("var_type")),
    _ncycl(getParam<unsigned int>("ncycl")),
    _x1(getParam<Real>("x1")),
    _y1(getParam<Real>("y1")),
    _radius(getParam<Real>("radius")),
    _cyl_bottom(getParam<Real>("cyl_bottom")),
    _cyl_top(getParam<Real>("cyl_top")),
    _wi(getParam<Real>("int_width")),
    _Amp(getParam<Real>("amplitude"))
{
}

void PerturbedCylinderIC::initialSetup()
{
  // Set up domain bounds with mesh tools
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    _bottom_left(i) = _mesh.getMinInDimension(i);
    _top_right(i) = _mesh.getMaxInDimension(i);
  }
  _range = _top_right - _bottom_left;
}

Real PerturbedCylinderIC::value(const Point & p)
{
  Real val   = 0.0;
  Real x     = p(0);
  Real y     = p(1);
  Real z     = p(2);
  Real w2    = _wi/2.0;
  Real l     = _cyl_top - _cyl_bottom; //Cyliner Length
  Real wave_n = _ncycl/l; //wave_number in (R(z)=R0+Acos(kz))

  if (z >= _cyl_bottom && z <= _cyl_top)
  {
    Real temp = _radius + _Amp * std::cos(2.0 * libMesh::pi * wave_n * (z - _cyl_bottom)); //R(z)
    Real temp1 = (x - _x1) * (x - _x1) + (y - _y1) * (y - _y1); //Distance from the center

    if (temp1 <= (temp - w2) * (temp - w2))
    {
      if (_var_type == 0) val = 1.0;
      else if (_var_type == 1) val = 0.0;
    }
    else if (temp1 >= (temp + w2) * (temp + w2))
    {
      if (_var_type == 0) val = 0.0;
      else if (_var_type == 1) val = 1.0;
    }
    else
    {
      Real rb = std::abs(sqrt(temp1) - (temp - w2))/_wi;
      Real hb = 1.0 - (3.0*rb*rb - 2.0*rb*rb*rb);
      if (_var_type == 0) val = hb;
      else if (_var_type == 1) val = 1.0 - hb;
    }
  }
  else
  {
    if (_var_type == 0)
      val = 0.0;
    else if (_var_type == 1)
      val = 1.0;
  }

  return val;
}
