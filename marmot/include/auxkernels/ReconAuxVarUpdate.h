#ifndef RECONAUXVARUPDATE_H
#define  RECONAUXVARUPDATE_H

#include "AuxKernel.h"
#include "MooseEnum.h"
#include "EBSDReader.h"
#include "Euler2RGB.h"
#include "ReconAuxVarUpdateAction.h"

//Forward Declarations
class ReconAuxVarUpdate;

template<>
InputParameters validParams<ReconAuxVarUpdate>();

/**
 * Update Reconstruction AuxVariables based on grain and order parameter indexes input from Material.
 */
class ReconAuxVarUpdate : public AuxKernel, EBSDAccessFunctors
{
public:
  ReconAuxVarUpdate(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  const EBSDReader & _ebsd_reader;
  const MaterialProperty<unsigned int> & _grain_index;

  /// String and associated MooseEnum that stores the type of data
  /// this AuxKernel extracts.
  MooseEnum _data_name;

  /// Accessor functor to fetch the selected data field form the EBSD data point
  EBSDAvgDataFunctor * _val;

  //MaterialProperty<unsigned int> & _op_index;
};

#endif //RECONAUXVARUPDATE_H
