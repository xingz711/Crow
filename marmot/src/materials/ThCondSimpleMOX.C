#include "ThCondSimpleMOX.h"

template<>
InputParameters validParams<ThCondSimpleMOX>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<Real>("length_scale", 1.0e-6, "Lengthscale of model");
  params.addParam<Real>("Gas_conductivity", 0.0321, "Thermal conductivity in the pore in W/m-K for MOX fuel at 1000K");
  params.addParam<Real>("GB_conductivity", 2.0, "conductivity in the GB for Paul & Karthik's model");
  params.addParam<Real>("Bulk_conductivity", 2.46, "Bulk conductivity of MOX fuel at 1000K (for Paul & Karthik's model)");
  params.addParam<Real>("PPT_conductivity", 25.1, "Conductivity of the MOX metallic preciptates at 1000K");
  MooseEnum cube_options("B D");
  params.addRequiredParam<MooseEnum>("cube",cube_options,"Which cube microstructure are you using?");

  return params;
}

ThCondSimpleMOX::ThCondSimpleMOX(const InputParameters & parameters) :
    Material(parameters),
    _length_scale(getParam<Real>("length_scale")),
    _Gas_conductivity(getParam<Real>("Gas_conductivity")),
    _GB_conductivity(getParam<Real>("GB_conductivity")),
    _Bulk_conductivity(getParam<Real>("Bulk_conductivity")),
    _PPT_conductivity(getParam<Real>("PPT_conductivity")),
    _thermal_conductivity(declareProperty<Real>("thermal_conductivity")),
    _cube(getParam<MooseEnum>("cube"))
{
  // Assign grain order parameters
  _n = coupledComponents("v");
  _vals.resize(_n);

  for (unsigned int i = 0; i < _n; ++i)
    _vals[i] = &coupledValue("v", i);
}

/*void
ThCondSimpleMOX::initializeProperties()
{
}*/

void
ThCondSimpleMOX::computeQpProperties()
{
  Real Gas_cond = _Gas_conductivity;
  Real GB_cond = _GB_conductivity;
  Real Bulk_cond = _Bulk_conductivity;
  Real PPT_cond = _PPT_conductivity;
  Real local_cond = Bulk_cond; // The output thermal conductivity

  // Define the bnds parameter
  Real bnds = 0.0;
  for (unsigned int i = 0; i < _n; ++i)
    bnds += (*_vals[i])[_qp] * (*_vals[i])[_qp];

  // ------------------------------------------------------------------------
  // ------------ Define the local conductivity  -------
  // ------------------------------------------------------------------------

  switch (_cube)
  {
  case 0: // Cube B
    if ((*_vals[0])[_qp] > 0.9) // Act only on PPTs stored as order parameter 0
      local_cond = PPT_cond;
    else if (bnds < 0.85 && (*_vals[0])[_qp] < 0.05)
      local_cond = GB_cond;
    break;

  case 1: // Cube D
    if ((*_vals[0])[_qp] > 0.9)
      local_cond = PPT_cond;
    else  if ((*_vals[1])[_qp] > 0.9)  // Act only on voids stored as order parameter 1
      local_cond = Gas_cond;
    else if (bnds < 0.85 && (*_vals[1])[_qp] > 0.25)
      local_cond = Gas_cond;
    else if (bnds < 0.85 && (*_vals[0])[_qp] < 0.05)
      local_cond = GB_cond;
    break;
  }

    // ------------------------------------------------------------------------
    // ------------ Rescale the conductivity ----------------------------------
    // ------------------------------------------------------------------------

  _thermal_conductivity[_qp] = local_cond*_length_scale; // Thermal conductivity in W/(length_scale-K)
}
