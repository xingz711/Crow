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

#ifndef POLYCRYSTALBCACTION_H
#define POLYCRYSTALBCACTION_H

#include "Action.h"

//Forward Declarations
class PolycrystalBCAction;

template<>
InputParameters validParams<PolycrystalBCAction>();

class PolycrystalBCAction: public Action
{
public:
  PolycrystalBCAction(const InputParameters & params);

  virtual void act();

private:
  std::string _type;
  unsigned int _op_num;
  std::string _var_name_base;
  std::vector<BoundaryName> _boundary;
  Real _value;
};

#endif //POLYCRYSTALBCACTION_H
