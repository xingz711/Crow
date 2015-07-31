#ifndef POLYCRYSTALVOIDIC_H
#define POLYCRYSTALVOIDIC_H

#include "InitialCondition.h"

// Forward Declarations
class PolycrystalVoidIC;

template<>
InputParameters validParams<PolycrystalVoidIC>();

/**
 * PolycrystalIC creates a polycrystal initial condition.
 * With 2 Grains, _typ = 0 results in a circular inclusion grain and _type = 1 gives a bicrystal.
 * With more than 2 grains, _typ = 0 gives set positions for 6 grains, _type = 1 gives hexagonal grains for 4 grains.
 *                          _typ = 2 Gives a random voronoi structure
 * \todo This code contains some potentially infinit while loops (while (too_close))!
 */
class PolycrystalVoidIC : public InitialCondition
{
public:
  PolycrystalVoidIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

protected:
  void genGrainCenterPt();

private:
  MooseMesh & _mesh;

  unsigned int _op_num;
  unsigned int _op_index;
  unsigned int _void_num;
  unsigned int _void_index;
  Real _min_void_dist;

  unsigned int _rand_seed;
  int _typ;

  Point _bottom_left;
  Point _top_right;
  Point _range;
  Real _max_void_radius;
  Real _min_void_radius;
  Real _outvalue;
  bool _edge_check;
  Real _gr_radius;
  unsigned int _maxiter;
  bool _use_min_grain_sep_dist;
  Real _min_grain_sep_dist;
  bool _use_min_edge_dist;
  Real _min_edge_dist;

  std::vector<Point> _centerpoints;
  std::vector<Point> _void_centerpoints;
  std::vector<Real> _void_radii;
};

#endif //POLYCRYSTALIC_H
