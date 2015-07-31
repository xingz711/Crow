#include "LinearElasticPFDamagePorosity.h"

template<>
InputParameters validParams<LinearElasticPFDamagePorosity>()
{
  InputParameters params = validParams<LinearElasticPFDamageSolIC>();
  params.addCoupledVar("porosity", "Porosity variable");
  return params;
}

LinearElasticPFDamagePorosity::LinearElasticPFDamagePorosity(const InputParameters & parameters) :
    LinearElasticPFDamageSolIC(parameters),
    _has_porosity(isCoupled("porosity")),
    _porosity(_has_porosity ? coupledValue("porosity") : _zero)
{
}

void LinearElasticPFDamagePorosity::initProperty()
{
  if (_conc[_qp] > _c_void_tol)
    _d_void[_qp] = _kvoid;
  else
    _d_void[_qp] = 1.0 - _porosity[_qp] + _kvoid;
}
