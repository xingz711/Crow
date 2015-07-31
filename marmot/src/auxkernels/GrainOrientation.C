#include "GrainOrientation.h"

template<>
InputParameters validParams<GrainOrientation>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addParam<bool>("output_angles", false, "Output angles if true, only the order parameter numbers if false");
  params.addParam<FileName>("Euler_angles_file_name", "no_angles", "specifies the orientation file name");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

GrainOrientation::GrainOrientation(const InputParameters & parameters) :
    AuxKernel(parameters),
    _output_angles(getParam<bool>("output_angles")),
    _angle_file_name(getParam<FileName>("Euler_angles_file_name"))
{
  unsigned int n = coupledComponents("v");
  if (_output_angles && _angle_file_name.compare("no_angles") == 0)
    mooseError("No orientation file name has been passed in");

  _vals.resize(n);
  for (unsigned int i = 0; i < n; ++i)
    _vals[i] = &coupledValue("v", i);

  if (_output_angles)
  {
    // Read in Euler angles from "angle_file_name"
    std::ifstream inFile(_angle_file_name.c_str());

    if (!inFile)
      mooseError("Can't open input file ");

    for (unsigned int i = 0; i < 4; ++i)
      inFile.ignore(255, '\n'); // ignore line

    // Resize vector of neutron impact positions
    _Euler_Angles.resize(n);
    Real weight;

    for (unsigned int crys = 0; crys < n; ++crys)
    {
      for (unsigned int j = 0; j < 3; ++j)
        inFile >> _Euler_Angles[crys](j);

      inFile >> weight;
    }

    inFile.close();
  }
}

Real
GrainOrientation::computeValue()
{
  Real mxvalue = 0;
  unsigned int mx_ind = 0;

  for (unsigned int i = 0; i < _vals.size(); ++i)
  {
    if ((*_vals[i])[_qp] > mxvalue)
    {
      mxvalue = (*_vals[i])[_qp];
      mx_ind = i;
    }
  }

  if (_output_angles)
    return _Euler_Angles[mx_ind](0);
  else
    return mx_ind;
}
