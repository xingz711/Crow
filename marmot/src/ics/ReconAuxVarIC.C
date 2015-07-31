#include "ReconAuxVarIC.h"

template<>
InputParameters validParams<ReconAuxVarIC>()
{

  InputParameters params = validParams<InitialCondition>();
  MooseEnum field_types = EBSDAccessFunctors::getPointDataFieldType();
  params.addRequiredParam<MooseEnum>("data_name", field_types, "The data type from EBSD input file");
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  return params;
}

ReconAuxVarIC::ReconAuxVarIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _data_name(getParam<MooseEnum>("data_name")),
    _val(getPointDataAccessFunctor(_data_name))
{
}

Real
ReconAuxVarIC::value(const Point &)
{
  // Output initial AuxVariable value for each element by selecting EBSDReader Enum variable
  const Point p = _current_elem->centroid();
  const EBSDReader::EBSDPointData & d = _ebsd_reader.getData(p);

  return (*_val)(d);
}
