#include "VacancyRecombinationTermKernel.h"

//#include "Material.h"

template<>
InputParameters validParams<VacancyRecombinationTermKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("eta", "coupled order parameter for void");
  params.addCoupledVar("ci", "Interstitial Concentration field");
  params.addParam<Real>("Rbulk",0.0, "Bulk recombination term");
  params.addParam<Real>("Rs", "Efficiency of void nucleation/annihilation");
  return params;
}

VacancyRecombinationTermKernel::VacancyRecombinationTermKernel(const std::string & name, InputParameters parameters) :
    Kernel(name,parameters),
    _eta(coupledValue("eta")),
    _ci(coupledValue("ci")),
    _Rbulk(getParam<Real>("Rbulk")),
    _Rs(getParam<Real>("Rs"))
{
}

Real
VacancyRecombinationTermKernel::computeQpResidual()
{
  Real Rr = _Rbulk + _eta[_qp]*_Rs;
    
  return Rr*_ci[_qp]*_u[_qp]*_test[_i][_qp];

}

Real
VacancyRecombinationTermKernel::computeQpJacobian()
{
  Real Rr = _Rbulk + _eta[_qp]*_Rs;

  return  Rr*_ci[_qp]*_phi[_j][_qp]*_test[_i][_qp];  
}
