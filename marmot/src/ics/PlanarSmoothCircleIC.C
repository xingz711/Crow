#include "PlanarSmoothCircleIC.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<PlanarSmoothCircleIC>()
{
  InputParameters params = validParams<SmoothCircleBaseIC>();
  params.addRequiredParam<unsigned int>("numbub", "The number of bubbles to be placed on GB");
  params.addRequiredParam<Real>("bubspac", "minimum spacing of bubbles, measured from center to center");
  params.addRequiredParam<Real>("position", "position of the plane of bubbles");
  MooseEnum dir_options("x y z");
  params.addRequiredParam<MooseEnum>("direction", dir_options, "direction normal to the plane of bubbles");
  params.addParam<unsigned int>("rand_seed", 2000, "random seed");
  params.addParam<unsigned int>("numtries", 1000, "The number of tries");
  params.addRequiredParam<Real>("radius", "Mean radius value for the circels");
  params.addParam<Real>("radius_variation", 0.0, "Uniform: Plus or minus fraction of random variation in the bubble radius, Normal: standard deviation");
  MooseEnum rand_options("uniform normal none", "none");
  params.addParam<MooseEnum>("radius_variation_type", rand_options, "Type of distribution that random circle radii will follow");
  return params;
}

PlanarSmoothCircleIC::PlanarSmoothCircleIC(const InputParameters & parameters) :
    SmoothCircleBaseIC(parameters),
    _numbub(getParam<unsigned int>("numbub")),
    _bubspac(getParam<Real>("bubspac")),
    _position(getParam<Real>("position")),
    _direction(getParam<MooseEnum>("direction")),
    _numtries(getParam<unsigned int>("numtries")),
    _radius(getParam<Real>("radius")),
    _radius_variation(getParam<Real>("radius_variation")),
    _radius_variation_type(getParam<MooseEnum>("radius_variation_type"))
{
  MooseRandom::seed(getParam<unsigned int>("rand_seed"));
  switch (_radius_variation_type)
  {
    case 2: // No variation
      if (_radius_variation > 0.0)
        mooseError("If radius_variation > 0.0, you must pass in a radius_variation_type in PlanarSmoothCircleIC");
      break;
  }
}

void
PlanarSmoothCircleIC::computeCircleRadii()
{
  _radii.resize(_numbub);

  for (unsigned int i = 0; i < _numbub; i++)
  {
    // Vary bubble radius
    switch (_radius_variation_type)
    {
      case 0: // Uniform distrubtion
        _radii[i] = _radius * (1.0 + (1.0 - 2.0 * MooseRandom::rand()) * _radius_variation);
        break;
      case 1: // Normal distribution
        _radii[i] = MooseRandom::randNormal(_radius,_radius_variation);
        break;
      case 2: // No variation
        _radii[i] = _radius;
    }

    if (_radii[i] < 0.0)
      _radii[i] = 0.0;
  }
}

void
PlanarSmoothCircleIC::computeCircleCenters()
{
  _centers.resize(_numbub);

  Point bottom_left;
  Point top_right;
  Point range;

  // Set up domain bounds with mesh tools
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    bottom_left(i) = _mesh.getMinInDimension(i);
    top_right(i) = _mesh.getMaxInDimension(i);
  }
  range = top_right - bottom_left;

  for (unsigned int i = 0; i < _numbub; ++i)
  {
    // Vary circle positions
    unsigned int num_tries = 0;

    Real rr = 0.0;
    Point tst_center;
    while (rr < _bubspac && num_tries < _numtries)
    {
      num_tries++;

      Real ran1 = MooseRandom::rand();
      Real ran2 = MooseRandom::rand();

      switch (_direction)
      {
        case 0: // direction = x
          tst_center(0) = _position;
          tst_center(1) = bottom_left(1) + ran1*(range(1));
          tst_center(2) = bottom_left(2) + ran2*(range(2));
          break;

        case 1: // direction = y
          tst_center(0) = bottom_left(0) + ran1*(range(0));
          tst_center(1) = _position;
          tst_center(2) = bottom_left(2) + ran2*(range(2));
          break;

        case 2: // direction = z
          tst_center(0) = bottom_left(0) + ran1*(range(0));
          tst_center(1) = bottom_left(1) + ran2*(range(1));
          tst_center(2) = _position;
          break;
      }

      for (unsigned int j = 0; j < i; ++j)
      {
        if (j == 0)
          rr = 1000.0;

        Real tmp_rr = _mesh.minPeriodicDistance(_var.number(),tst_center,_centers[j]);

        if (tmp_rr < rr)
          rr = tmp_rr;
      }

      if (i == 0)
        rr = range.size();
     }

     if (num_tries == _numtries)
       mooseError("Too many tried in PlanarSmoothCircleIC");

    _centers[i] = tst_center;
  }
}
