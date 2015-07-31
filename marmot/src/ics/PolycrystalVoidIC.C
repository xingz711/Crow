#include "PolycrystalVoidIC.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<PolycrystalVoidIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<unsigned int>("op_num", "Number of grain order parameters");
  params.addParam<unsigned int>("op_index", 0, "The index for the current grain order parameter");
  params.addRequiredParam<unsigned int>("void_num", "Number of Voids on Grain Boundaries");
  params.addParam<unsigned int>("void_index", 1, "Whether we are setting the IC for a grain (0) or for the voids (1)");
  params.addParam<Real>("min_void_dist", 0.0, "This is the minimum distance that voids can be apart");
  params.addParam<unsigned int>("rand_seed", 12444, "The random seed");
  params.addRequiredParam<int>("ICType", "Type of grain structure");
  params.addParam<Real>("gr_radius", 0.0, "Radius of circle grain, if op_num = 2 and ICType = 0.");
  params.addRequiredParam<Real>("max_void_radius", "Maximum radius of voids on the Grain Boundaries");
  params.addParam<Real>("min_void_radius", 0, "Minimum radius of voids on the Grain Boundaries");
  params.addParam<Real>("outvalue", 0.0, "Value of concentration outside of void");
  params.addParam<bool>("edge_check", false, "If bubbles too close to the edge will be removed");
  params.addParam<unsigned int>("maxiter", 1000, "Maximum interation to obtain void centers with edge check and grain centers with minimum distance");
  params.addParam<bool>("use_min_grain_separation_dist", false, "Flag for using minimum center to center distance between grains - works only for ICType 2");
  params.addParam<Real>("min_grain_separation_dist", 0.0, "Minimum center to center distance between grains");
  params.addParam<bool>("use_min_edge_dist", false, "Flag for using minimum distance of voids from edge - works only for ICType 2");
  params.addParam<Real>("min_edge_dist",0.0, "Minimum distance of void centers from edge");

  return params;
}

bool check(Point A, Point B)
{
  return(A.size()<B.size());
}

PolycrystalVoidIC::PolycrystalVoidIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _mesh(_fe_problem.mesh()),
    _op_num(getParam<unsigned int>("op_num")),
    _op_index(getParam<unsigned int>("op_index")),
    _void_num(getParam<unsigned int>("void_num")),
    _void_index(getParam<unsigned int>("void_index")),
    _min_void_dist(getParam<Real>("min_void_dist")),
    _rand_seed(getParam<unsigned int>("rand_seed")),
    _typ(getParam<int>("ICType")),
    _max_void_radius(getParam<Real>("max_void_radius")),
    _min_void_radius(getParam<Real>("min_void_radius")),
    _outvalue(getParam<Real>("outvalue")),
    _edge_check(getParam<bool>("edge_check")),
    _gr_radius(getParam<Real>("gr_radius")),
    _maxiter(getParam<unsigned int>("maxiter")),
    _use_min_grain_sep_dist(getParam<bool>("use_min_grain_separation_dist")),
    _min_grain_sep_dist(getParam<Real>("min_grain_separation_dist")),
    _use_min_edge_dist(getParam<bool>("use_min_edge_dist")),
    _min_edge_dist(getParam<Real>("min_edge_dist"))
{

  if ( _use_min_grain_sep_dist && _min_grain_sep_dist == 0 )
    mooseError("PolycrystalVoidIC: Use positive min_grain_separation_distance");

  _void_radii.resize(_void_num);

  MooseRandom::seed(_rand_seed);

  // Set up domain bounds with mesh tools
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    _bottom_left(i) = _mesh.getMinInDimension(i);
    _top_right(i) = _mesh.getMaxInDimension(i);
  }
  _range = _top_right - _bottom_left;

  _centerpoints.resize(_op_num);

  std::vector<Point> holder;
  holder.resize(8);

  if (_typ == 0)
  {
    holder[0] = Point(0.35, 0.5, 0.0);
    holder[1] = Point(0.85, 0.5, 0.0);
    holder[2] = Point(0.1, 0.0, 0.0);
    holder[3] = Point(0.6, 0.0, 0.0);

  }
  else if (_typ == 1)
  {
    holder[0] = Point(0.35, 0.5, 0.0);
    holder[1] = Point(0.85, 0.5, 0.0);
    holder[2] = Point(0.1, 0.0, 0.0);
    holder[3] = Point(0.6, 0.0, 0.0);
  }

  // Assign Center Points to the Mesh
  if ( _typ == 2 )
    genGrainCenterPt();
  else
  {
    // Loop Through Crystals
    for (unsigned int op = 0; op < _op_num; ++op)
      // Loop through Dimensions
      for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
        // Assign Center points based on the holder points above
        _centerpoints[op](i) = _bottom_left(i) + _range(i) * holder[op](i);
  }

  // Create vector of radius values for voids
  for (unsigned int i = 0; i < _void_num; ++i)
  {
    // get range of void sizes
    Real radius_range = _max_void_radius - _min_void_radius;;

    _void_radii[i] = _min_void_radius + radius_range * MooseRandom::rand();
  }

  // Resize the void center point vector
  _void_centerpoints.resize(_void_num);

  if (_op_num == 2)
  {
    if (_typ == 1) // Void setup for a bicrystal
    {
      for (unsigned int void_point = 0; void_point < _void_num; ++void_point)
      {
        bool too_close;
        do
        {
          too_close = false;

          _void_centerpoints[void_point](0) = _bottom_left(0) + _range(0)/2.0;
          _void_centerpoints[void_point](1) = _void_radii[void_point] + (_range(1) - 2.0*_void_radii[void_point]) *MooseRandom::rand();
          for (unsigned int i = 0; i < void_point; ++i)
          {
            Point distance = _void_centerpoints[void_point] - _void_centerpoints[i];

            if (distance.size() <= _min_void_dist)
            {
              too_close = true;
              break;
            }
          }
        }
        while (too_close);
      }
    }
    else if (_typ == 0) // Void setup for circular grain
    {
      Point center_grain = _bottom_left + _range/2.0;
      Real radius;

      if (_gr_radius == 0.0)
        radius = _range(0)/3.0;
      else
        radius = _gr_radius;


      for (unsigned int void_point = 0; void_point < _void_num; ++void_point)
      {
        bool too_close;
        // _console << "Void Number: " << void_point << "\n";
        do
        {
          too_close = false;

          Real angle = 2*pi*MooseRandom::rand();
          _void_centerpoints[void_point](0) = center_grain(0) + radius*std::cos(angle);
          _void_centerpoints[void_point](1) = center_grain(1) + radius*std::sin(angle);
          if (_range(2) > 0.0)
          {
            _void_centerpoints[void_point](2) = _bottom_left(2) + 1.5*_void_radii[void_point] + (_range(2) - 1.5*_void_radii[void_point])*MooseRandom::rand();
          }
          for (unsigned int i = 0; i < void_point; ++i)
          {
            Point distance = _void_centerpoints[void_point] - _void_centerpoints[i];
            /*_console << _min_void_dist << std::endl;
              _console << distance.size() << std::endl;
              _console << std::endl;*/
            if (distance.size() < _min_void_dist)
            {
              too_close = true;
              break;
            }
          }
        }
        while (too_close);
      }
    }
    else if (_typ == 2) // Void setup for periodic bicrystal
    {
      Real GBpos[2];
      GBpos[0] = _bottom_left(0) + _range(0)*0.25;
      GBpos[1] = _bottom_left(0) + _range(0)*0.75;
      unsigned int GB_index;

      for (unsigned int void_point = 0; void_point < _void_num; ++void_point)
      {
        bool too_close;
        do
        {
          too_close = false;

          if (void_point*1.0 < _void_num*0.5)
            GB_index = 0;
          else
            GB_index = 1;

          _void_centerpoints[void_point](0) = GBpos[GB_index];
          _void_centerpoints[void_point](1) = _void_radii[void_point] + (_range(1) - 2.0*_void_radii[void_point]) *MooseRandom::rand();
          for (unsigned int i = 0; i < void_point; ++i)
          {
            Point distance = _void_centerpoints[void_point] - _void_centerpoints[i];

            if (distance.size() <= _min_void_dist)
            {
              too_close = true;
              break;
            }
          }
        }
        while (too_close);
      }
    }
  }

  if (_op_num > 2)
  {
    // This Code will place void centerpoints on grain boundaries
    for (unsigned int void_point = 0; void_point < _void_num; ++void_point)
    {
      bool too_close;
      unsigned iter = 0;

      do
      {
        too_close = false;

        Point rand_point;

        for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
        {
          rand_point(i) = _bottom_left(i) + _range(i) * MooseRandom::rand();
        }

        // Create a temp_centerpoints variable to act on without altering the first
        std::vector<Point> temp_centerpoints = _centerpoints;

        // Periodic Boundary conditions for temp_centerpoints
        for ( unsigned int cp = 0; cp < temp_centerpoints.size(); ++cp)
          for ( unsigned int i = 0; i < LIBMESH_DIM; ++i)
          {
            Real difference = temp_centerpoints[cp](i) - rand_point(i);
            if (std::abs(difference) > _range(i)/2.0)
              temp_centerpoints[cp](i) -= _range(i) * (difference/std::abs(difference));
          }

        // Allow the vectors to be sorted based on their distance from the rand_point
        for (unsigned int i = 0; i < temp_centerpoints.size(); ++i)
        {
          temp_centerpoints[i] -= rand_point;
        }

        // Sort the temp_centerpoints into order of magnitude
        std::sort (temp_centerpoints.begin(), temp_centerpoints.end(), check);

        // Compose the points back to center point coordinates
        for ( unsigned int i=0; i < temp_centerpoints.size(); ++i)
          temp_centerpoints[i] += rand_point;


        // ****Note that I am treating these points as position vectors****
        Point closest_point = temp_centerpoints[0];
        Point next_closest_point = temp_centerpoints[1];

        // Find Slope of Line in the plane orthoganal to the diff_centerpoint vector
        Point diff_centerpoints = next_closest_point - closest_point;
        Point diff_rand_center = rand_point - closest_point;
        Point normal_vector = diff_centerpoints.cross(diff_rand_center);
        Point slope = normal_vector.cross(diff_centerpoints);
        slope /= std::pow(slope.size_sq(),0.5);//Normalize slope

        // Midpoint position vector between two center points
        Point midpoint = closest_point + (0.5 * diff_centerpoints);

        // Solve for the scalar multipler solution on the line
        Real lambda = (rand_point - midpoint) * slope;

        // Assigning points to vector
        _void_centerpoints[void_point] = slope*lambda + midpoint;

        // Checking to see if points are in the domain
        for (unsigned int i=0; i<LIBMESH_DIM; i++)
        {
          if ( _void_centerpoints[void_point](i) > _top_right(i) )
            _void_centerpoints[void_point](i) -= _range(i);
          if ( _void_centerpoints[void_point](i) < _bottom_left(i))
            _void_centerpoints[void_point](i) += _range(i);
        }

        Real vr = 0;

        // Checking to see if points are too close to boundary
        for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
        {
          if ( _void_centerpoints[void_point](i) > _top_right(i) - vr )
            _void_centerpoints[void_point](i) = _top_right(i) - vr;
          if ( _void_centerpoints[void_point](i) < _bottom_left(i) + vr)
            _void_centerpoints[void_point](i) = _bottom_left(i) + vr;
        }

        unsigned int sdim = LIBMESH_DIM;
        if (_range(2) == 0.0)
          sdim = 2;

        if (_use_min_edge_dist)
        {
          for (unsigned int i = 0; i < sdim; ++i)
          {
            if(_void_centerpoints[void_point](i) + _void_radii[void_point] > _top_right(i) - _min_edge_dist)
              too_close = true;
            if(_void_centerpoints[void_point](i) - _void_radii[void_point] < _bottom_left(i) + _min_edge_dist)
              too_close = true;
          }
        }

        if (!too_close)
        {
          for (unsigned int i = 0; i < void_point; ++i)
          {
            Real dist = _mesh.minPeriodicDistance(_var.number(), _void_centerpoints[void_point], _void_centerpoints[i]);
            if (dist < _min_void_dist)
            {
              too_close = true;
              break;
            }
          }
        }

        iter++;
      }
      while (too_close && iter < _maxiter);

      if (iter == _maxiter && too_close)
        mooseError("PolycrystalVoidIC: Exceed maximum iteration - try reducing void_num or min_void_dist or min_edge_dist, or increase maxiter");

    }
  }
}

Real
PolycrystalVoidIC::value(const Point & p)
{

  Real val = 0.0;
  bool is_void = false;
  Real ave_radius = 0.0;

  // If statement to determine how close to void point is
  for (unsigned int i = 0; i < _void_num; ++i)
  {
    /*Point difference;
    for (unsigned int j=0; j<LIBMESH_DIM; j++)
    {
      difference(j) = std::abs(_void_centerpoints[i](j) - p(j));
      if (_periodic[j] && difference(j) > _range(j)/2.0)
        difference(j) = _range(j) - difference(j);
        }*/
    Real dist = _mesh.minPeriodicDistance(_var.number(), _void_centerpoints[i], p);
    if (dist < _void_radii[i])
      is_void = true;

    ave_radius += _void_radii[i]/_void_num;
  }
  // Set dimensionality of the current mesh
  unsigned int sdim = LIBMESH_DIM;
  if (_range(2) == 0.0)
    sdim = 2;
// Points too close to edge cannot be part of bubbles
  if (_edge_check)
    for (unsigned int j = 0; j < sdim; ++j)
    {
      Real edge1 = _bottom_left(j) + ave_radius;
      Real edge2 = _top_right(j) - ave_radius;
      // _console << "edge1 = " << edge1 << ", edge2 = " << edge2 << std::endl;
      if (p(j) < edge1 || p(j) > edge2)
        is_void = false;
    }


  if (is_void == false)
  {
    Real min_distance = 1e10;
    if (_op_num ==1)
      val = 1.0;
    else if (_op_num == 2)
    {
      if (_typ == 1) // Bicrystal
      {
        Real x_center = _bottom_left(0) + _range(0)/2.0;
        if ((p(0) < x_center && _op_index == 0 && _void_index < 1) || (p(0) >= x_center && _op_index ==1 && _void_index < 1))
          val = 1.0;
      }
      else if (_typ == 0) // Circular grain in center
      {
        Point grain_center = _bottom_left + _range/2.0;

        Real radius;

        if (_gr_radius == 0.0)
          radius = _range(0)/3.0;
        else
          radius = _gr_radius;

        Point rad_vec = p - grain_center;

        Real dist = std::sqrt(rad_vec(0)*rad_vec(0) + rad_vec(1)*rad_vec(1));

        if ((dist > radius && _op_index == 0 && _void_index < 1) || (dist <= radius && _op_index == 1 && _void_index < 1))
          val = 1.0;
      }
      else if (_typ == 2) // Periodic Bicrystal
      {
        Real GBpos[2];
        GBpos[0] = _bottom_left(0) + _range(0)*0.25;
        GBpos[1] = _bottom_left(0) + _range(0)*0.75;
        if (_op_index == 0 && _void_index < 1)
          if (p(0) < GBpos[0] || p(0) > GBpos[1])
            val = 1.0;

        if (_op_index == 1 && _void_index < 1)
          if (p(0) >= GBpos[0] && p(0) <= GBpos[1])
            val = 1.0;
      }

    }
    else
    {
      unsigned int min_index = 10000;
      for (unsigned int op = 0; op < _op_num; ++op)
      {
        Real dist =  _mesh.minPeriodicDistance(_var.number(), _centerpoints[op], p);

        /*Point difference = _centerpoints[op] - p;

        if (_periodic.size() == 1 && _periodic[1] == true)
        {
          for (unsigned int i=0; i<LIBMESH_DIM; i++)
            if (std::abs(difference(i)) > _range(i)/2.)
              difference(i) = _range(i) - std::abs(_centerpoints[op](i) - p(i));
        }
        else
          for (unsigned int i=0; i<LIBMESH_DIM; i++)
            if (std::abs(difference(i)) > _range(i)/2. && _periodic[i] == true)
            difference(i) = _range(i) - std::abs(_centerpoints[op](i) - p(i));*/


        if (min_distance > dist)
        {
          min_distance = dist;
          min_index = op;
        }

      }

      if (min_index > _op_num)
        mooseError("ERROR in PolycrystalVoidIC: didn't find minimum values");

      if (min_index == _op_index && _void_index < 1)
        val = 1.0;
    }
  }

  // Assign value to void
  if (_void_index > 0)
    val = is_void ? 1.0 : _outvalue;

  if (val > 1.0)
    val = 1.0;

  if (val < 0.0)
    val = 0.0;

  return val;
}

void
PolycrystalVoidIC::genGrainCenterPt()
{
  if (_use_min_grain_sep_dist)
  {
    for (unsigned int op = 0; op < _op_num; ++op)
    {
      bool too_close = true;
      unsigned int iter = 0;

      while (too_close && iter < _maxiter)
      {
        too_close = false;

        for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
          _centerpoints[op](i) = _bottom_left(i) + _range(i) * MooseRandom::rand();

        for (unsigned int i = 0; i < op; ++i)
        {
          Real dist = _mesh.minPeriodicDistance(_var.number(), _centerpoints[op], _centerpoints[i]);
          if (dist < _min_grain_sep_dist)
            too_close = true;
        }
        iter++;
      }
      if (iter == _maxiter && too_close)
        mooseError("PolycrystalVoidIC: Exceed maximum iteration - try reducing op_num or min_grain_separation_dist, increasing domain size or maxiter");
    }
  }
  else
  {
    for (unsigned int op = 0; op < _op_num; ++op)
      for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
        _centerpoints[op](i) = _bottom_left(i) + _range(i) * MooseRandom::rand();
  }
}
