#include "LinearElasticPFDamageSolIC.h"

template<>
InputParameters validParams<LinearElasticPFDamageSolIC>()
{
  InputParameters params = validParams<LinearElasticPFDamage>();
  params.addCoupledVar("conc", "Concentration Variable");
  params.addParam<Real>("conc_void_tol", 0.01, "Tolerance for conc beyond which material is considered as void");
  params.addParam<Real>("void_stiffness", 1e-8, "Stiffness of voided material");
  return params;
}

LinearElasticPFDamageSolIC::LinearElasticPFDamageSolIC(const InputParameters & parameters) :
  LinearElasticPFDamage(parameters),
  _has_conc(isCoupled("conc")),
  _conc(_has_conc ? coupledValue("conc") : _zero),
  _c_void_tol(getParam<Real>("conc_void_tol")),
  _kvoid(getParam<Real>("void_stiffness"))
{
}

void LinearElasticPFDamageSolIC::computeQpStress()
{
  initProperty();
  _stress[_qp].zero();
  update_var(_elastic_strain[_qp]);
}

void LinearElasticPFDamageSolIC::initProperty()
{
  if (_has_conc && _conc[_qp] > _c_void_tol)
    _d_void[_qp] = _kvoid;
  else
    _d_void[_qp] = 1.0;
}
