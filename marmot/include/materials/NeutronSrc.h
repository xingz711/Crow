#ifndef NEUTRONSRC_H
#define NEUTRONSRC_H

#include "Material.h"


//Forward Declarations
class NeutronSrc;

template<>
InputParameters validParams<NeutronSrc>();

/**
 * Uranium Oxide material properties used on the meso-scale.
 */
class NeutronSrc : public Material
{
public:
  NeutronSrc(const InputParameters & parameters);

protected:
  virtual void timestepSetup();
  virtual void computeProperties();

  const unsigned int _mesh_dimension;

  std::vector<Point> _NImpPos;

  VariableValue & _eta;

  MaterialProperty<Real> & _interstitial_increase;

  MaterialProperty<Real> & _vacancy_increase;

  Real _Vg;
  Real _x1, _y1, _z1;
  Real _x2, _y2, _z2;

  Point _bottom_left;
  Point _top_right;
  Point _range;

  bool _periodic;
  bool _coupled_rate;

  int _input_num_neutrons;
  int _num_neutrons;

  Real _input_rate;
  const PostprocessorValue & _expected_num;

  Real _mag;
  Real _vac_bias;
  Real _spacing_mult;
  Real _fission_rate_mult;
};

#endif //NEUTRONSRC_H
