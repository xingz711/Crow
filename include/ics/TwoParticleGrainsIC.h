#ifndef TWOPARTICLEGRAINSIC_H
#define TWOPARTICLEGRAINSIC_H

#include "Kernel.h"
#include "InitialCondition.h"

// System includes
#include <string>

// Forward Declarations
class TwoParticleGrainsIC;

template<>
InputParameters validParams<TwoParticleGrainsIC>();

/**
 * TwoParticleGrainsIC creates a grain structure with 2 circle grains and one matrix grain
*/
class TwoParticleGrainsIC : public InitialCondition
{
public:
  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  TwoParticleGrainsIC(const std::string & name,
                InputParameters parameters);

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

protected:
  MooseMesh & _mesh;
  /// A reference to the nonlinear system
  NonlinearSystem & _nl;

  unsigned int _op_num;
  unsigned int _op_index;

  Point _bottom_left;
  Point _top_right;
  Point _range;
};

#endif //TWOPARTICLEGRAINSIC_H
