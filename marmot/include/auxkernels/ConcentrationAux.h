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

#ifndef CONCENTRATIONAUX_H
#define CONCENTRATIONAUX_H

#include "AuxKernel.h"

//Forward Declarations
class ConcentrationAux;

template<>
InputParameters validParams<ConcentrationAux>();

/**
 * Concentration auxiliary value. Returns a random value in [-1:1)
 * \todo Should be using the new RandomInterface
 */
class ConcentrationAux : public AuxKernel
{
public:
  ConcentrationAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();
};

#endif //CONCENTRATIONAUX_H
