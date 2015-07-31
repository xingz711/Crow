#ifndef RECONAUXVARIC_H
#define RECONAUXVARIC_H

#include "InitialCondition.h"
#include "MooseEnum.h"
#include "EBSDReader.h"

// Forward Declarations
class ReconAuxVarIC;

template<>
InputParameters validParams<ReconAuxVarIC>();

/**
 * ReconAuxVarIC creates an AuxVariable initial condition from an EBSD dataset

*/
class ReconAuxVarIC : public InitialCondition, EBSDAccessFunctors
{
public:
  ReconAuxVarIC(const InputParameters & parameters);
  virtual Real value(const Point & p);

private:
  const EBSDReader & _ebsd_reader;
  const MooseEnum _data_name;
  EBSDPointDataFunctor * _val;
};

#endif //RECONAUXVARIC_H
