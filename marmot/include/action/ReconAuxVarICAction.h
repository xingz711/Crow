#ifndef RECONAUXVARICACTION_H
#define RECONAUXVARICACTION_H

#include "InputParameters.h"
#include "Action.h"

// Forward Declarations
class ReconAuxVarICAction;

template<>
InputParameters validParams<ReconAuxVarICAction>();

class ReconAuxVarICAction: public Action
{
public:
  ReconAuxVarICAction(const InputParameters & params);

  virtual void act();

private:
  UserObjectName _ebsd_reader_name;
  unsigned int _sd;
};

#endif //RECONAUXVARICACTION_H
