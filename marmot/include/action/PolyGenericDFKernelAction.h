#ifndef POLYGenericDFKERNELACTION_H
#define POLYGenericDFKERNELACTION_H

#include "Action.h"

//Forward Declarations
class PolyGenericDFKernelAction;

template<>
InputParameters validParams<PolyGenericDFKernelAction>();

/**
 * This custom action adds the temperature gradient driving force kernel ACGrGrGeneric for each order parameter
 */
class PolyGenericDFKernelAction: public Action
{
public:
  PolyGenericDFKernelAction(const InputParameters & params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;

  VariableName _c;


};

#endif //POLYGenericDFKERNELACTION_H
