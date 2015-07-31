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

#ifndef POLYCRYSTALAVGRAREA_H
#define POLYCRYSTALAVGRAREA_H

#include "ElementIntegralVariablePostprocessor.h"

//Forward Declarations
class PolycrystalAvGrArea;

template<>
InputParameters validParams<PolycrystalAvGrArea>();

/**
 * Compute the average grain area in a polycrystal
 */
class PolycrystalAvGrArea : public ElementIntegralVariablePostprocessor
{
public:
  PolycrystalAvGrArea(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

protected:
  virtual Real computeQpIntegral();

private:
  std::vector<Real> _integral_value;
  unsigned int _current_var;

  //unsigned int _op_num;
  //std::string _var_name_base;

  const unsigned int _nvals;
  std::vector<VariableValue *> _vals;
};

#endif //POLYCRYSTALAVGRAREA_H
