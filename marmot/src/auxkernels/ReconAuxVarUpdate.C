#include "ReconAuxVarUpdate.h"

template<>
InputParameters validParams<ReconAuxVarUpdate>()
{
  InputParameters params = validParams<AuxKernel>();
  MooseEnum field_types = EBSDAccessFunctors::getAvgDataFieldType();
  params.addRequiredParam<MooseEnum>("data_name", field_types, "The data to be extracted from the EBSD data by this AuxKernel");
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  // params.addParam<unsigned int>("sd", 3, "An integer representing reference sample direction");
  MultiMooseEnum execute_options(SetupInterface::getExecuteOptions());
  execute_options = "timestep";
  params.set<MultiMooseEnum>("execute_on") = execute_options;
  return params;
}

ReconAuxVarUpdate::ReconAuxVarUpdate(const InputParameters & parameters) :
    AuxKernel(parameters),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _grain_index(getMaterialProperty<unsigned int>("grain")),
    //_op_index(getMaterialProperty<unsigned int>("op")),
    _data_name(getParam<MooseEnum>("data_name")),
    _val(getAvgDataAccessFunctor(_data_name))
{
}

Real
ReconAuxVarUpdate::computeValue()
{
  // Output updated AuxVariable value for each element. For convenience, the value from quadrature
  // point 3 is used.  This is an arbitrary choice.  Future work will look at all of the quadrature
  // points within the element and only change the element when more than half of the qp have switched
  // to a new grain index.

  return (*_val)(_ebsd_reader.getAvgData(_grain_index[3]));

  /*
  unsigned int index1 = _op_index[3];

  else if (_Aux_param == "rgb")
  {
    const EBSDReader::EBSDAvgData & a = _ebsd_reader.getAvgData(index);
    return Euler2RGB(_sd, a.phi1, a.phi, a.phi2, a.phase, a.symmetry);
  }
  else if (_Aux_param == "op")
  {
    _op[index] = index1;
    return _op[index];

    mooseError("Error: Please input supported Aux_param");
  }*/
}
