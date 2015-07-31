#ifndef MULTISMOOTHCIRCLEPOROSITYIC_H
#define MULTISMOOTHCIRCLEPOROSITYIC_H

#include "Kernel.h"
#include "SmoothCircleBaseIC.h"

// System includes
#include <string>

// Forward Declarations
class MultiSmoothCirclePorosityIC;

template<>
InputParameters validParams<MultiSmoothCirclePorosityIC>();

/**
 * MultismoothCirclePorosityIC distributes pores in a simulation cell based on porosity
 * Pores with radius above a threshold is incorporated geometrically
 * For radius below the threshold the pores are incorporated as porosity in local cell
 * For considering porosity, this ic should be used in conjunction with PropertyInterpolationFunction
 */
class MultiSmoothCirclePorosityIC : public SmoothCircleBaseIC
{
public:
  MultiSmoothCirclePorosityIC(const InputParameters & parameters);

  virtual void initialSetup();
  virtual void computeCircleRadii();
  virtual void computeCircleCenters();

protected:
  virtual void initializeProperty();
  virtual void computeGrid();
  virtual void setGridProp();
  virtual Real computePropertyValue(const Point & p);
  virtual Real value(const Point & p);

  /// Number of pores
  unsigned int _numbub;
  /// Spacing between pores
  Real _bubspac;
  /// Random seed
  unsigned int _rand_seed;
  ///Number of tries
  unsigned int _numtries;
  /// Mean radius
  Real _radius;
  /// Radius variation
  Real _radius_variation;
  /// Porosity
  Real _porosity;
  /// Threshold radius for geometric pore
  Real _radius_min_geom;
  /// Minimum allowable pore radius
  Real _radius_min;
  /// Distance from edge
  Real _edge_min_dist;
  /// Type of radius variation uniform random, normal, lognormal
  MooseEnum _radius_variation_type;
  /// If true, pores with radius below threshold is included as porosity
  bool _include_small_pore;

  ///@{
  /// Cell size to assign porosity values (grid points)
  Real _lx_size;
  Real _ly_size;
  Real _lz_size;
  ///@}

  /// Maximum allowable in local cells
  Real _max_loc_porosity;

  /// Number of properties to be assigned on grid
  unsigned int _num_prop;
  /// Type of property
  MooseEnum _property_type;

  Point _bottom_left;
  Point _top_right;
  Point _range;

  unsigned int _sdim;
  Real _box_vol;
  Real _tot_vol_pore;

  /// Number of pores to be considered as porosity
  unsigned int _numbubprop;

  ///@{
  /// Number of grid points
  unsigned int _n_grid_x;
  unsigned int _n_grid_y;
  unsigned int _n_grid_z;
  ///@}

  std::vector<Real> _radiiprop;

  ///@{
  /// Grid coordinates
  std::vector<Real> _x_grid;
  std::vector<Real> _y_grid;
  std::vector<Real> _z_grid;
  ///@}

  /// Property grid used by IC
  std::vector< std::vector<Real> > _prop_grid;
};

#endif //MULTISMOOTHCIRCLEPOROSITYIC_H
