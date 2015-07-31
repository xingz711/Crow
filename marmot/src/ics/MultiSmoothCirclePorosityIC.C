#include "MultiSmoothCirclePorosityIC.h"
#include "MooseRandom.h"
#include "MathUtils.h"

template<>
InputParameters validParams<MultiSmoothCirclePorosityIC>()
{
  InputParameters params = validParams<SmoothCircleBaseIC>();
  params.addRequiredRangeCheckedParam<Real>("bubspac", "bubspac >= 0", "minimum spacing of pores, measured from center to center");
  params.addParam<unsigned int>("rand_seed", 2000, "random seed");
  params.addParam<unsigned int>("numtries", 1000, "The number of tries");
  params.addRequiredParam<Real>("radius_mean", "Mean radius value for the circles");
  params.addParam<Real>("radius_variation", 0.0, "Plus or minus fraction of random variation in the pore radius for uniform, standard deviation for normal");
  params.addRequiredRangeCheckedParam<Real>("porosity", "porosity >= 0", "Porosity of the simulation cell");
  params.addRequiredRangeCheckedParam<Real>("radius_min_geom", "radius_min_geom >= 0", "Minimum radius to be considered for geometric pore construction");
  params.addRequiredRangeCheckedParam<Real>("radius_min", "radius_min >= 0", "Minimum radius allowed as valid");
  params.addRangeCheckedParam<Real>("edge_min_dist", 0.0, "edge_min_dist >= 0", "Minimum distance from edge of simulation cell");
  MooseEnum rand_options("uniform normal lognormal none", "none");
  params.addParam<MooseEnum>("radius_variation_type", rand_options, "Type of distribution that random circle radii will follow");
  params.addRequiredParam<bool>("include_small_pore", "Include small pore in porosity calculation");
  params.addRequiredRangeCheckedParam<Real>("lx_size", "lx_size > 0", "Unit cell size for assigning property values: x dir");
  params.addRequiredRangeCheckedParam<Real>("ly_size", "ly_size > 0", "Unit cell size for assigning property values: y dir");
  params.addRangeCheckedParam<Real>("lz_size", 0.0, "lz_size >= 0", "Unit cell size for assigning property values: z dir");
  params.addRangeCheckedParam<Real>("max_loc_porosity", 0.0, "max_loc_porosity >= 0", "Maximum porosity of the local cell or grid");
  params.addParam<unsigned int>("num_property", 2, "Number of properties initialized by this class (porosity and maximum_radius)");
  MooseEnum property_options("concentration porosity maximum_radius none", "none");
  params.addParam<MooseEnum>("property_type", property_options, "Property to set/get values from this IC");

  return params;
}

MultiSmoothCirclePorosityIC::MultiSmoothCirclePorosityIC(const InputParameters & parameters) :
    SmoothCircleBaseIC(parameters),
    _bubspac(getParam<Real>("bubspac")),
    _rand_seed(getParam<unsigned int>("rand_seed")),
    _numtries(getParam<unsigned int>("numtries")),
    _radius(getParam<Real>("radius_mean")),
    _radius_variation(getParam<Real>("radius_variation")),
    _porosity(getParam<Real>("porosity")),
    _radius_min_geom(getParam<Real>("radius_min_geom")),
    _radius_min(getParam<Real>("radius_min")),
    _edge_min_dist(getParam<Real>("edge_min_dist")),
    _radius_variation_type(getParam<MooseEnum>("radius_variation_type")),
    _include_small_pore(getParam<bool>("include_small_pore")),
    _lx_size(getParam<Real>("lx_size")),
    _ly_size(getParam<Real>("ly_size")),
    _lz_size(getParam<Real>("lz_size")),
    _max_loc_porosity(getParam<Real>("max_loc_porosity")),
    _num_prop(getParam<unsigned int>("num_property")),
    _property_type(getParam<MooseEnum>("property_type"))
{
}

void
MultiSmoothCirclePorosityIC::initialSetup()
{
  MooseRandom::seed(_rand_seed);

  //Set up domain bounds with mesh tools
  for (unsigned int i = 0; i < LIBMESH_DIM; ++i)
  {
    _bottom_left(i) = _mesh.getMinInDimension(i);
    _top_right(i) = _mesh.getMaxInDimension(i);
  }
  _range = _top_right - _bottom_left;
  _range.print();

  _sdim = 3;
  if (_range(LIBMESH_DIM-1) == 0)
    _sdim  = 2;

  _box_vol = _range(0);
  for (unsigned int i = 1; i < _sdim; ++i)
    _box_vol *= _range(i);

  _tot_vol_pore = _porosity * _box_vol;

  if (_radius_variation_type == 3 && _radius_variation > 0.0)
    mooseError("If radius_variation > 0.0, you must pass in a radius_variation_type in MultiSmoothCirclePorosityIC");

  initializeProperty();
}

void
MultiSmoothCirclePorosityIC::computeCircleRadii()
{
  Real vol = 0.0;
  _numbub = 0;
  _numbubprop = 0;

  while (vol < _tot_vol_pore)
  {
    Real radii = 0.0;
    switch (_radius_variation_type)
    {
      case 0: //Uniform distrubtion
        radii = _radius*(1.0 + (1.0 - 2.0*MooseRandom::rand())*_radius_variation);
        break;
      case 1: //Normal distribution
        radii = MooseRandom::randNormal(_radius,_radius_variation);
        break;
      case 2: //LogNormal distribution
        radii = std::exp(MooseRandom::randNormal(_radius,_radius_variation));
        break;
      case 3: //No variation
        radii = _radius;
    }

    if (radii < _radius_min)
      radii = 0.0;

    Real volpore;

    if (_sdim == 3)
      volpore = 4.0/3.0 * libMesh::pi * std::pow(radii,3.0);
    else
      volpore = libMesh::pi * std::pow(radii,2.0);

    if (volpore < _box_vol && radii > _radius_min)
    {
      vol += volpore;

      if (vol > _tot_vol_pore)
      {
        vol -= volpore;
        volpore = _tot_vol_pore - vol;

        if (_sdim == 3)
          radii = std::pow(3.0/4.0 * volpore/ libMesh::pi , 1.0/3.0);
        else
          radii = std::pow( volpore/ libMesh::pi , 0.5);

        if (radii >= _radius_min_geom || (radii < _radius_min_geom && _include_small_pore))
          vol = _tot_vol_pore;
        else
          break;
      }
      if (radii >= _radius_min_geom)
      {
        _radii.resize(_numbub+1);
        _radii[_numbub] = radii;
        _numbub++;
      }
      else
      {
        if (_include_small_pore)
        {
          _numbubprop++;
          switch (_property_type)
          {
            case 1:
            case 2:
              _radiiprop.resize(_numbubprop);
              _radiiprop[_numbubprop-1] = radii;
              break;
          }
        }
        else
          vol-=volpore;
      }
    }
  }
}

void
MultiSmoothCirclePorosityIC::computeCircleCenters()
{
  _centers.resize(_numbub);

  for (unsigned int i = 0; i < _numbub; ++i)
  {
    //Vary circle center positions
    Point newcenter = 0.0;
    Point adjusted_range, adjusted_bottom;
    unsigned int num_tries = 0;
    bool gap_flag = true;

    for(unsigned int j = 0; j < LIBMESH_DIM; ++j)
    {
      adjusted_range(j) = _range(j) - 2 * (_radii[i]+_edge_min_dist);
      adjusted_bottom(j) = _bottom_left(j) + _edge_min_dist + _radii[i];
    }

    while (gap_flag && num_tries < _numtries)
    {
      Real ran1 = MooseRandom::rand();
      Real ran2 = MooseRandom::rand();
      Real ran3 = MooseRandom::rand();

      newcenter(0) = adjusted_bottom(0) + ran1*adjusted_range(0);
      newcenter(1) = adjusted_bottom(1) + ran2*adjusted_range(1);
      newcenter(2) = _sdim == 3 ? adjusted_bottom(2) + ran3*adjusted_range(2) : 0.0;

      gap_flag = false;
      for (unsigned int j = 0; j < i; ++j)
      {
        Real dis = std::pow((newcenter - _centers[j]).size_sq(), 0.5);

        if (dis - _radii[i] - _radii[j] < _bubspac)
        {
          gap_flag = true;
          break;
        }
      }
      num_tries++;
    }

    if (num_tries == _numtries && gap_flag)
      mooseError("Too many tries in MultiSmoothCirclePorosityIC");

    _centers[i] = newcenter;
  }
}

void
MultiSmoothCirclePorosityIC::computeGrid()
{
  Point lsize;

  lsize(0) = _lx_size;
  lsize(1) = _ly_size;
  lsize(2) = _lz_size;

  for (unsigned int i = 0; i < _sdim; ++i)
  {
    if (floor( _range(i)/ lsize(i) ) != _range(i)/lsize(i))
      mooseError("Error: local cell size should integer multiple of simulation box size");
  }

  _n_grid_x = _range(0)/_lx_size + 1;
  _n_grid_y = _range(1)/_ly_size + 1;

  if (_sdim == 3)
    _n_grid_z = _range(0)/_lz_size + 1;

  _x_grid.resize(_n_grid_x);
  _y_grid.resize(_n_grid_y);

  if (_sdim == 3)
    _z_grid.resize(_n_grid_z);

  for (unsigned int i = 0; i < _n_grid_x; ++i)
    _x_grid[i]=i*_lx_size;

  for (unsigned int i = 0; i < _n_grid_y; ++i)
    _y_grid[i]=i*_ly_size;

  if (_sdim == 3)
    for (unsigned int i = 0; i < _n_grid_y; ++i)
      _y_grid[i]=i*_lz_size;

  _prop_grid.resize( _n_grid_x * _n_grid_y , std::vector<Real>(_num_prop,0.0) );
  if (_sdim==3)
    _prop_grid.resize( _n_grid_x * _n_grid_y * _n_grid_z , std::vector<Real>(_num_prop,0.0) );
}

void
MultiSmoothCirclePorosityIC::setGridProp()
{
  Real box_diag = std::pow(_lx_size *_lx_size +_ly_size *_ly_size + _lz_size * _lz_size,0.5);

  for (unsigned int i = 0; i < _numbubprop; ++i)
  {
    bool set_loc_flag = true;
    unsigned int iter = 0;

    while (set_loc_flag && iter < _numtries)
    {
      //Pores with size less than threshold are located randomly on grid points
      Real ran1 = MooseRandom::rand();
      Real ran2 = MooseRandom::rand();

      Point newcenter;

      unsigned int xind = floor(ran1*_range(0)/_lx_size);
      unsigned int yind = floor(ran2*_range(1)/_ly_size);
      unsigned int zind = 0;
      if (_sdim == 3)
      {
        Real ran3 = MooseRandom::rand();
        zind = floor(ran3*_range(2)/_lz_size);
      }
      newcenter(0) = _bottom_left(0) + xind *_lx_size;
      newcenter(1) = _bottom_left(1) + yind *_ly_size;
      newcenter(2) = _bottom_left(2) + zind *_lz_size;

      set_loc_flag = false;

      for (unsigned int j = 0; j < _numbub; ++j)
      {
        Real dis = std::pow((newcenter - _centers[j]).size_sq(),0.5);

        if ( dis < _radii[j] + box_diag/2)
        {
          set_loc_flag = true;
          break;
        }
      }

      Real volpore;
      if (_sdim == 3)
        volpore = 4.0/3.0 * libMesh::pi * std::pow(_radiiprop[i],3.0);
      else
        volpore = libMesh::pi * std::pow(_radiiprop[i],2.0);

      Real loc_vol;
      if (_sdim == 3)
        loc_vol = _lx_size * _ly_size * _lz_size;
      else
        loc_vol = _lx_size * _ly_size;

      unsigned int grid_offset;

      if (_sdim == 3)
        grid_offset = xind * _n_grid_y * _n_grid_z + yind * _n_grid_z + zind;
      else
        grid_offset = xind * _n_grid_y + yind;

      Real loc_porosity = _prop_grid[grid_offset][0] + volpore/loc_vol;

      if (!set_loc_flag  && loc_porosity < _max_loc_porosity)
      {
        _prop_grid[grid_offset][0] = loc_porosity;

        Real max_radius = _prop_grid[grid_offset][1];
        if ( _radiiprop[i] > max_radius )
            _prop_grid[grid_offset][1] = _radiiprop[i];
      }
      else
        set_loc_flag = true;

      iter++;
    }

    if (iter == _numtries && set_loc_flag)
      mooseError("MultiSmoothCirclePorosityIC error: Too many iterations in computeGridProp - Increase max_loc_porosity/numtries or reduce lx_size or ly_size");
  }
}

void
MultiSmoothCirclePorosityIC::initializeProperty()
{
  switch (_property_type)
  {
    case 0:
      SmoothCircleBaseIC::initialSetup();
      break;
    case 1:
    case 2:
      SmoothCircleBaseIC::initialSetup();
      computeGrid();
      setGridProp();
      break;
    case 3:
    default:
      mooseError("Pass a valid property type: concentration/porosity/maximum_radius");
  }
}

Real
MultiSmoothCirclePorosityIC::value(const Point &p)
{
  switch(_property_type)
  {
    case 0:
      return SmoothCircleBaseIC::value(p);
    case 1:
    case 2:
      return computePropertyValue(p);
    case 3:
    default:
      mooseError("Pass a valid property type: concentration/porosity/maximum_radius");
  }
}

Real
MultiSmoothCirclePorosityIC::computePropertyValue(const Point &p)
{
  unsigned int prop_index;
  Real offset_factor = 0.1;

  switch (_property_type)
  {
    case 1:
      prop_index = 0;
      break;
    case 2:
      prop_index = 1;
      break;
  }
  unsigned int xindex[2],yindex[2],zindex[2];
  Real ptmp1, ptmp2, ptmp3;

  ptmp1 = p(0);
  if (ptmp1 == _range(0))//Offset point if on edge to calculate grid no
    ptmp1 = ptmp1 - offset_factor * _lx_size;

  xindex[0] = std::floor(ptmp1/_lx_size);
  xindex[1] = xindex[0]+1;

  ptmp2 = p(1);
  if (ptmp2 == _range(1))//Offset point if on edge to calculate grid no
    ptmp2 = ptmp2 - offset_factor * _ly_size;

  yindex[0] = std::floor(ptmp2/_ly_size);
  yindex[1] = yindex[0]+1;

  if (_sdim == 3)
  {
    ptmp3 = p(2);
    if (ptmp3 == _range(2))//Offset point if on edge to calculate grid no
      ptmp3 = ptmp3 - offset_factor * _lz_size;

    zindex[0] = floor(ptmp3/_lz_size);
    zindex[1] = zindex[0]+1;
  }

  const Real xc = (_x_grid[xindex[1]]+_x_grid[xindex[0]])/2.0;
  const Real yc = (_y_grid[yindex[1]]+_y_grid[yindex[0]])/2.0;
  const Real zc = _sdim == 3 ? (_z_grid[zindex[1]]+_z_grid[zindex[0]])/2.0 : 0.0;

  const Real etax = (p(0)-xc)/(_x_grid[xindex[1]]-_x_grid[xindex[0]]);
  const Real etay = (p(1)-yc)/(_y_grid[yindex[1]]-_y_grid[yindex[0]]);
  const Real etaz = _sdim == 3 ? (p(2)-zc)/(_z_grid[zindex[1]]-_z_grid[zindex[0]]) : 0.0;

  Real val = 0;
  if (_sdim == 3)
  {
    //Tri linear interpolation
    Real boundval[8];

    boundval[0] = _prop_grid[xindex[0]*_n_grid_y*_n_grid_z + yindex[0]*_n_grid_z + zindex[0]][prop_index];
    boundval[1] = _prop_grid[xindex[0]*_n_grid_y*_n_grid_z + yindex[0]*_n_grid_z + zindex[1]][prop_index];
    boundval[2] = _prop_grid[xindex[0]*_n_grid_y*_n_grid_z + yindex[1]*_n_grid_z + zindex[0]][prop_index];
    boundval[3] = _prop_grid[xindex[0]*_n_grid_y*_n_grid_z + yindex[1]*_n_grid_z + zindex[1]][prop_index];
    boundval[4] = _prop_grid[xindex[1]*_n_grid_y*_n_grid_z + yindex[0]*_n_grid_z + zindex[0]][prop_index];
    boundval[5] = _prop_grid[xindex[1]*_n_grid_y*_n_grid_z + yindex[0]*_n_grid_z + zindex[1]][prop_index];
    boundval[6] = _prop_grid[xindex[1]*_n_grid_y*_n_grid_z + yindex[1]*_n_grid_z + zindex[0]][prop_index];
    boundval[7] = _prop_grid[xindex[1]*_n_grid_y*_n_grid_z + yindex[1]*_n_grid_z + zindex[1]][prop_index];

    val =   (1.0-etax)*(1.0-etay)*(1.0-etaz)/8.0*boundval[0]
          + (1.0-etax)*(1.0-etay)*(1.0+etaz)/8.0*boundval[1]
          + (1.0-etax)*(1.0+etay)*(1.0-etaz)/8.0*boundval[2]
          + (1.0-etax)*(1.0+etay)*(1.0+etaz)/8.0*boundval[3]
          + (1.0+etax)*(1.0-etay)*(1.0-etaz)/8.0*boundval[4]
          + (1.0+etax)*(1.0-etay)*(1.0+etaz)/8.0*boundval[5]
          + (1.0+etax)*(1.0+etay)*(1.0-etaz)/8.0*boundval[6]
          + (1.0+etax)*(1.0+etay)*(1.0+etaz)/8.0*boundval[7];
  }
  else
  {
    //Bilinear interpolation
    Real boundval[4];

    boundval[0] = _prop_grid[xindex[0]*_n_grid_y+yindex[0]][prop_index];
    boundval[1] = _prop_grid[xindex[0]*_n_grid_y+yindex[1]][prop_index];
    boundval[2] = _prop_grid[xindex[1]*_n_grid_y+yindex[0]][prop_index];
    boundval[3] = _prop_grid[xindex[1]*_n_grid_y+yindex[1]][prop_index];

    val = ((1.0-etax)*(1.0-etay)/4.0*boundval[0]+(1.0-etax)*(1.0+etay)/4.0*boundval[1]+(1.0+etax)*(1.0-etay)/4.0*boundval[2]+(1.0+etax)*(1.0+etay)/4.0*boundval[3]);
  }

  return val;
}
