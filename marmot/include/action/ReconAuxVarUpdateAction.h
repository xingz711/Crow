#ifndef RECONAUXVARUPDATEACTION_H
#define RECONAUXVARUPDATEACTION_H

#include "Action.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
#include "EBSDReader.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
//#include "libmesh/libmesh.h"
//#include "libmesh/exodusII_io.h"
//#include "libmesh/equation_systems.h"
//#include "libmesh/nonlinear_implicit_system.h"
//#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

// Forward Declarations
class ReconAuxVarUpdateAction;

template<>
InputParameters validParams<ReconAuxVarUpdateAction>();

class ReconAuxVarUpdateAction: public Action
{
public:
  ReconAuxVarUpdateAction(const InputParameters & params);

  virtual void act();

private:
  unsigned int _sd;
  std::string _phi1, _PHI, _phi2, _grn, _phase, _sym, _rgb, _op;
};

#endif //RECONAUXVARUPDATEACTION_H
