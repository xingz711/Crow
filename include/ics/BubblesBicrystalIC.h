#ifndef BUBBLESBICRYSTALIC_H
#define BUBBLESBICRYSTALIC_H

#include "InitialCondition.h"

// Forward Declarations
class BubblesBicrystalIC;

template<>
InputParameters validParams<BubblesBicrystalIC>();

/**
 * BubblesBicrystalIC creates multiple lenticular pores along a straight bicrystal grain boundary.
 */
class BubblesBicrystalIC : public InitialCondition
{
public:
  BubblesBicrystalIC(const std::string & name,
                     InputParameters parameters);

  virtual Real value(const Point & p);

  virtual void initialSetup();

private:

  MooseMesh & _mesh;

  unsigned int _phase;
  unsigned int _nbub;

  Real _ra;
  Real _wi;
  Real _theta;
  Real _c_outvalue;

  Point _bottom_left;
  Point _top_right;
  Point _range;
};

#endif //BUBBLESBICRYSTALIC_H
