#include "FractureACBulkMatRateSolIC.h"

template<>
InputParameters validParams<FractureACBulkMatRateSolIC>()
{
  InputParameters params = validParams<FractureACBulkMatRate>();
  params.addCoupledVar("bnd", 0.0, "Grain Boundary Order Parameter");
  params.addCoupledVar("conc", 0.0, "Concentration Order Parameter");
  params.addRequiredParam<Real>("gc_tens_gb","GB fracture energy in tension");
  params.addParam<Real>("gc_comp_gb",1000.0,"GB fracture energy in compression");
  params.addRequiredParam<Real>("gc_tens_mat","Material fracture energy in tension");
  params.addParam<Real>("gc_comp_mat",5.0,"Material fracture energy in compression");
  params.addParam<Real>("gb_bnd",0.8,"Cutoff GB bound value");

  return params;
}

FractureACBulkMatRateSolIC::FractureACBulkMatRateSolIC(const InputParameters & parameters) :
    FractureACBulkMatRate(parameters),
    _bnd(coupledValue("bnd")),
    _conc(coupledValue("conc")),
    _gc_tens_gb(getParam<Real>("gc_tens_gb")),
    _gc_comp_gb(getParam<Real>("gc_comp_gb")),
    _gc_tens_mat(getParam<Real>("gc_tens_mat")),
    _gc_comp_mat(getParam<Real>("gc_comp_mat")),
    _gb_bnd(getParam<Real>("gb_bnd"))
{
}

void
FractureACBulkMatRateSolIC::computeQpProperties()
{
  Real c = _conc[_qp];
  Real bnd= _bnd[_qp];

  if (c>0.01)
    c=1.0;
  else
    c=0.0;

  Real gbint = 0.0;
  if (c>0.01)
    gbint = 1.0;
  else
    gbint = bnd;

  Real x = 0.0;
  if (gbint > _gb_bnd)
    x = (gbint - _gb_bnd) / (1.0 -_gb_bnd);

  _gc_prop_tens[_qp] = (1.0 - x) * _gc_tens_gb + x * _gc_tens_mat;
  _gc_prop_comp[_qp] = (1.0 - x) * _gc_comp_gb + x * _gc_comp_mat;

  _L[_qp] = _L0;
}
