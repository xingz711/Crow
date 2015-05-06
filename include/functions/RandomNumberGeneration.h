/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef RANDOMNUMBERGENERATION_H
#define RANDOMNUMBERGENERATION_H

#include "Function.h"

class RandomNumberGeneration;

template<>
InputParameters validParams<RandomNumberGeneration>();

class RandomNumberGeneration : public Function
{
public:
  RandomNumberGeneration(const std::string & name, InputParameters parameters);
  virtual ~RandomNumberGeneration();

  virtual Real value(Real /*t*/, const Point & /*p*/);

protected:
  Real _min;
  Real _max;
  Real _range;
};



#endif
