#ifndef AVEULERANGLEPOSTPROCESSORACTION_H
#define AVEULERANGLEPOSTPROCESSORACTION_H

#include "Action.h"
#include "ExecStore.h"

//Forward Declarations
class AvEulerAnglePostprocessorAction;

template<>
InputParameters validParams<AvEulerAnglePostprocessorAction>();

/**
 * Set up PolycrystalAvEulerAngle postprocessors for multiple garins and Euler angles.
 */
class AvEulerAnglePostprocessorAction: public Action
{
public:
  AvEulerAnglePostprocessorAction(const InputParameters & params);

  virtual void act();

protected:
  unsigned int _op_num;
  std::string _var_name_base;
  MultiMooseEnum _execute_on;
};

#endif //AVEULERANGLEPOSTPROCESSORACTION_H
