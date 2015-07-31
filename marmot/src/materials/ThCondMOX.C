#include "ThCondMOX.h"

template<>
InputParameters validParams<ThCondMOX>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  params.addCoupledVar("eta", 0.0, "Order Parameter");
  params.addParam<Real>("length_scale", 1.0e-6, "Lengthscale of model");
  params.addParam<Real>("Gas_conductivity", 0.0321, "Thermal conductivity in the pore in W/m-K for MOX fuel at 1000K");
  params.addParam<Real>("GB_conductivity", 2.0, "conductivity in the GB for Paul & Karthik's model");
  params.addParam<Real>("Bulk_conductivity", 2.46, "Bulk conductivity of MOX fuel at 1000K (for Paul & Karthik's model)");
  params.addParam<Real>("PPT_conductivity", 25.1, "Conductivity of the MOX metallic preciptates at 1000K");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

ThCondMOX::ThCondMOX(const InputParameters & parameters) :
    Material(parameters),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _grn(declareProperty<unsigned int>("grn")),
    _phase(declareProperty<unsigned int>("phase")),
    _op_num(getParam<unsigned int>("op_num")),
    _has_v(isCoupled("v")),
    _length_scale(getParam<Real>("length_scale")),
    _Gas_conductivity(getParam<Real>("Gas_conductivity")),
    _GB_conductivity(getParam<Real>("GB_conductivity")),
    _Bulk_conductivity(getParam<Real>("Bulk_conductivity")),
    _PPT_conductivity(getParam<Real>("PPT_conductivity")),
    _eta(coupledValue("eta")),
    _thermal_conductivity(declareProperty<Real>("thermal_conductivity"))
{
  // Assign grain order parameters
  if (_has_v)
  {
    _n = coupledComponents("v");
    _vals.resize(_n);

    for (unsigned int i = 0; i < _n; ++i)
      _vals[i] = &coupledValue("v", i);
  }
  else
    _n = 0;
}

void
ThCondMOX::initializeProperties()
{
  // Initialize material properties from EBSDReader user object
  Point p0 = _current_elem->centroid();
  const EBSDReader::EBSDPointData & d = _ebsd_reader.getData(p0);
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _grn[_qp] = d.grain;
    _phase[_qp] = d.phase;
  }
}

void
ThCondMOX::computeProperties()
{
  Real Gas_cond = _Gas_conductivity;
  Real GB_cond = _GB_conductivity;
  Real Bulk_cond = _Bulk_conductivity;
  Real PPT_cond = _PPT_conductivity;
  Real local_cond = 0.0; // The output thermal conductivity

  // Find grain index (maxind) of largest eta value at each quadrature point
  Real maxvalue = -std::numeric_limits<Real>::max();
  unsigned int maxind = std::numeric_limits<unsigned int>::min();

  for (unsigned int i = 0; i < _op_num; ++i)
  {
    if ((*_vals[i])[_qp] > maxvalue)
    {
      maxvalue = (*_vals[i])[_qp];
      maxind = i;
    }
  }
  _grn[_qp] = maxind;
  _phase[_qp] = _ebsd_reader.getAvgData(_grn[_qp]).phase;


  // GB-defining Parameters

  Real bnds = 1.0;
  if (_has_v)
  {
    bnds = 0.0;
    for (unsigned int i = 0; i < _n; ++i)
      bnds += (*_vals[i])[_qp] * (*_vals[i])[_qp];
  }

  // Define the local conductivity

  if (_phase[_qp] == 0)
    local_cond = Gas_cond;

  else if (_phase[_qp] == 2)
    local_cond = PPT_cond;

  else if (_phase[_qp] == 1)
  {
    if (bnds < 0.90)
      local_cond = GB_cond;
    else
      local_cond = Bulk_cond;
  }

  // Rescale the conductivity
  _thermal_conductivity[_qp] = local_cond * _length_scale; // Thermal conductivity in W/(10nm-K)
}
