#include "ReconMaterial.h"

template<>
InputParameters validParams<ReconMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject to get values from");
  params.addRequiredParam<UserObjectName>("GrainTracker_object", "The GrainTracker UserObject to get values from");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

ReconMaterial::ReconMaterial(const InputParameters & parameters) :
    Material(parameters),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _grain_tracker(getUserObject<GrainTracker>("GrainTracker_object")),
    _op_num(getParam<unsigned int>("op_num")),
    _grain_num(getParam<unsigned int>("grain_num")),
    _grn(declareProperty<unsigned int>("grn")),
    _grn_old(declarePropertyOld<unsigned int>("grn")),
    _op(declareProperty<unsigned int>("op")),
    _op_old(declarePropertyOld<unsigned int>("op"))
{
  // Create vector of order paramter values at each quadrature point
  _vals.resize(_op_num);
  for (unsigned int i = 0; i < _op_num; ++i)
    _vals[i] = &coupledValue("v", i);
}

void
ReconMaterial::initQpStatefulProperties()
{
  Point p0 = _current_elem->centroid();
  const EBSDReader::EBSDPointData & d = _ebsd_reader.getData(p0);
  _grn[_qp] = d.grain;
  _op[_qp] = d.op;
}

void
ReconMaterial::computeProperties()
{
  // Update evolved grain and order parameter index values from GrainTracker
  const std::vector<std::pair<unsigned int, unsigned int> > & active_ops = _grain_tracker.getElementalValues(_current_elem->id());

  unsigned int n_active_ops= active_ops.size();
  if (n_active_ops < 1 && _t_step > 0 )
    mooseError("No active order parameters");

  // TODO: Need to figure out which grain/op to use for this material property.
  //       Several may be active but we'll just pick the first one here
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _grn[_qp] = active_ops[0].first - 1;
    _op[_qp] = active_ops[0].second;
  }
}
