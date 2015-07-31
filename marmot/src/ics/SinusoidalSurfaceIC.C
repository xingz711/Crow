#include "SinusoidalSurfaceIC.h"

template<>
InputParameters validParams<SinusoidalSurfaceIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addClassDescription("Initial condition to set up a 1D sinusoidal surface between an amorphous phase (vapor/gas) and a solid phase.");
  MooseEnum dom_dim("2D 3D");
  params.addRequiredParam<MooseEnum>("dom_dim", dom_dim, "The dimension of the domain, e.g.,2D or 3D");
  MooseEnum var_type("nonconserved conserved");
  params.addRequiredParam<MooseEnum>("var_type", var_type, "The index for the current variable");
  params.addRequiredParam<unsigned int>("ncycl", "The number of periods/cycles along x-axis");
  params.addRequiredParam<Real>("sub_height", "The height of the solid substrate");
  params.addRequiredParam<Real>("int_width", "The initial width of the interface");
  params.addRequiredParam<Real>("amplitude", "The amplitude of the sinusoidal surface");

  return params;
}

SinusoidalSurfaceIC::SinusoidalSurfaceIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _mesh(_fe_problem.mesh()),
    _dom_dim(getParam<MooseEnum>("dom_dim")),
    _var_type(getParam<MooseEnum>("var_type")),
    _ncycl(getParam<unsigned int>("ncycl")),
    _SH(getParam<Real>("sub_height")),
    _wi(getParam<Real>("int_width")),
    _Amp(getParam<Real>("amplitude"))
{
}

void SinusoidalSurfaceIC::initialSetup()
{
  // Set up domain bounds with mesh tools
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    _bottom_left(i) = _mesh.getMinInDimension(i);
    _top_right(i) = _mesh.getMaxInDimension(i);
  }
  _range = _top_right - _bottom_left;
}

Real SinusoidalSurfaceIC::value(const Point & p)
{
  Real val   = 0.0;
  Real x     = p(0);
  Real y     = p(1);
  Real z     = p(2);
  Real w2    = _wi/2.0;
  Real wave_n = _ncycl/_top_right(0); // wave_number (y=y0+Asinkx)

  if (_dom_dim == 0) // 2D domain
  {
    // -----------------------------------------------------------------------------
    // Define the sinusoidal surface for a 2D domain, e.g., y(x) = y0 + A * sin(kx)
    // -----------------------------------------------------------------------------

    if (y <= (_SH+_Amp*std::sin(2.0 * libMesh::pi * wave_n * x)) - w2)
    {
      if (_var_type == 0) val = 1.0;
      else if (_var_type == 1) val = 0.0;
    }
    else if (y >= (_SH+_Amp*std::sin(2.0 * libMesh::pi * wave_n * x)) + w2)
    {
      if (_var_type == 0) val = 0.0;
      else if (_var_type == 1) val = 1.0;
    }
    else
    {
      Real rb = std::abs(y - (_SH + _Amp * std::sin(2.0 * libMesh::pi * wave_n * x) - w2)) / _wi;
      Real hb = 1.0 - (3.0*rb*rb - 2.0*rb*rb*rb);
      if (_var_type == 0) val = hb;
      else if (_var_type == 1) val = 1.0 - hb;
    }
  }

  else if (_dom_dim == 1) // 3D domain
  {
    // -----------------------------------------------------------------------------------
    // Define the sinusoidal surface for a 3D domain, e.g., z(x,y) = z0 + A * sin(kx)
    // -----------------------------------------------------------------------------------

    if (z <= (_SH + _Amp * std::sin(2.0 * libMesh::pi * wave_n * x)) - w2)
    {
      if (_var_type == 0)
        val = 1.0;
      else if (_var_type == 1)
        val = 0.0;
      else
        mooseError("Invalid var_type.");
    }
    else if (z >= (_SH + _Amp * std::sin(2.0 * libMesh::pi * wave_n * x)) + w2)
    {
      if (_var_type == 0)
        val = 0.0;
      else if (_var_type == 1)
        val = 1.0;
      else
        mooseError("Invalid var_type.");
    }
    else
    {
      Real rb = std::abs(z - (_SH + _Amp * std::sin(2.0 * libMesh::pi * wave_n * x) - w2)) / _wi;
      Real hb = 1.0 - (3.0*rb*rb - 2.0*rb*rb*rb);

      if (_var_type == 0)
        val = hb;
      else if (_var_type == 1)
        val = 1.0 - hb;
      else
        mooseError("Invalid var_type.");
    }
  }

  return val;
}
