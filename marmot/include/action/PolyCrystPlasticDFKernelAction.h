#ifndef POLYCRYSTPLASTICDFKERNELACTION_H
#define POLYCRYSTPLASTICDFKERNELACTION_H

#include "Action.h"

class PolyCrystPlasticDFKernelAction;

template<>
InputParameters validParams<PolyCrystPlasticDFKernelAction>();

class PolyCrystPlasticDFKernelAction: public Action
{
public:
  PolyCrystPlasticDFKernelAction(const InputParameters & params);
  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  std::string _disp_x_name;
  std::string _disp_y_name;
  std::string _disp_z_name;
  bool _Tensor_form;
  bool _elastic_driving_force;
  bool _plastic_driving_force;
};
#endif //POLYCRYSTPLASTICDFKERNELACTION_H
