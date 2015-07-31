#ifndef POLYTGradDFKERNELACTION_H
#define POLYTGradDFKERNELACTION_H

#include "Action.h"

//Forward Declarations
class PolyTGradDFKernelAction;

template<>
InputParameters validParams<PolyTGradDFKernelAction>();

/**
 * This custom action adds the temperature gradient driving force kernel ACGrGrTGrad for each order parameter
 */
class PolyTGradDFKernelAction: public Action
{
public:
  PolyTGradDFKernelAction(const InputParameters & params);

  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;

  VariableName _T, _c;

};

#endif //POLYTGradDFKERNELACTION_H
