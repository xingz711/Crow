#include "TensorPolycrystalGrTrackerElasticMaterial.h"
#include "RotationTensor.h"
#include "GrainTracker.h"

template<>
InputParameters validParams<TensorPolycrystalGrTrackerElasticMaterial>()
{
  InputParameters params = validParams<LinearElasticMaterial>();
  params.addRequiredParam<FileName>("Euler_angles_file_name", "Name of the file containing the Euler angles");
  params.addParam<Real>("length_scale", 1.0e-9, "Dimension of length in meters");
  params.addParam<Real>("pressure_scale", 1.0e6, "Pressure is in Mpa");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredParam<UserObjectName>("GrainTracker_object", "The GrainTracker UserObject to get values from.");
  params.addRequiredParam<unsigned int>("grain_num", "Number of initial grains that will be modeled");
  params.addParam<unsigned int>("stiffness_buffer",10,"Number of extra elastic stiffnesses that are created to handle new grains");
  return params;
}

TensorPolycrystalGrTrackerElasticMaterial::TensorPolycrystalGrTrackerElasticMaterial(const InputParameters & parameters) :
    LinearElasticMaterial(parameters),
    _length_scale(getParam<Real>("length_scale")),
    _pressure_scale(getParam<Real>("pressure_scale")),
    _Euler_angles_file_name(getParam<FileName>("Euler_angles_file_name")),
    _grain_tracker(getUserObject<GrainTracker>("GrainTracker_object")),
    _grain_num(getParam<unsigned int>("grain_num")),
    _stiffness_buffer(getParam<unsigned int>("stiffness_buffer")),
    _JtoeV(6.24150974e18), // Joule to eV conversion
    _nop(coupledComponents("v"))
{
  // Initialize values for crystals
  _vals.resize(_nop);
  _D_elastic_tensor.resize(_nop);
  _C_rotated.resize(_grain_num + _stiffness_buffer);

  // Read in Euler angles from "angle_file_name"
  std::ifstream inFile(_Euler_angles_file_name.c_str());

  if (!inFile)
    mooseError("Can't open input file ");

  for (unsigned int i = 0; i < 4; ++i)
    inFile.ignore(255, '\n'); // ignore line

  Real weight;

  // Loop over grains
  for (unsigned int grn = 0; grn < _grain_num; ++grn)
  {
    // Read in Euler angles
    RealVectorValue Euler_Angles;
    for (unsigned int j=0; j<3; ++j)
      inFile >> Euler_Angles(j);

    inFile >> weight;

    // Rotate one elasticity tensor for each grntal
    RotationTensor R(Euler_Angles);
    _C_rotated[grn] = _Cijkl;
    _C_rotated[grn].rotate(R);

    if (grn < _stiffness_buffer)
    {
    _C_rotated[grn + _grain_num] = _Cijkl;
    _C_rotated[grn + _grain_num].rotate(R);
    }

  }

  inFile.close();

  // Loop over variables (ops)
  for (unsigned int op = 0; op < _nop; ++op)
  {
    // Initialize variables
    _vals[op] = &coupledValue("v", op);

    // Initialize D_elastic_tensor material
    std::string material_name = "D_elastic_tensor";
    std::stringstream out;
    out << op;
    material_name.append(out.str());
    _D_elastic_tensor[op] = &declareProperty<ElasticityTensorR4>(material_name);
  }
}

void TensorPolycrystalGrTrackerElasticMaterial::computeQpElasticityTensor()
{
  _Cijkl.zero();
  Real sum_h = 0.0;

  const std::vector<std::pair<unsigned int, unsigned int> > & active_ops = _grain_tracker.getElementalValues(_current_elem->id());

  unsigned int n_active_ops= active_ops.size();
  if (n_active_ops < 1 && _t_step > 0 )
    mooseError("No active order parameters");

  // Calculate elasticity tensor
  for (unsigned int op = 0; op<n_active_ops; ++op)
  {
    unsigned int grn_index = active_ops[op].first - 1;
    unsigned int op_index = active_ops[op].second;
    Real h = (1.0 + std::sin(libMesh::pi * ((*_vals[op_index])[_qp] - 0.5)))/2.0;
    _Cijkl += _C_rotated[grn_index]*h;
    sum_h += h;
  }

  Real tol = 1.0e-10;
  if (sum_h < tol)
    sum_h = tol;

  _Cijkl /= sum_h;

   // Fill in the matrix stiffness material property
  _elasticity_tensor[_qp] = _Cijkl;
  _Jacobian_mult[_qp] = _Cijkl;

  for (unsigned int op = 0; op < _nop; ++op)
    (*_D_elastic_tensor[op])[_qp].zero();

  // Calculate elasticity tensor derivative: Cderiv = dhdopi/sum_h * (Cop - _Cijkl)
  for (unsigned int op = 0; op < n_active_ops; ++op)
  {
    unsigned int grn_index = active_ops[op].first - 1;
    unsigned int op_index = active_ops[op].second;
    Real dhdopi = libMesh::pi*std::cos(libMesh::pi*((*_vals[op_index])[_qp] - 0.5))/2.0;
    ElasticityTensorR4 C_deriv(_C_rotated[grn_index]);
    C_deriv -= _Cijkl;
    C_deriv *= dhdopi/sum_h;

    // Convert from XPa to eV/(xm)^3, where X is pressure scale and x is length scale;
    C_deriv *= _JtoeV*(_length_scale*_length_scale*_length_scale)*_pressure_scale;

    // Fill in material property
    (*_D_elastic_tensor[op_index])[_qp] = C_deriv;
  }
}
