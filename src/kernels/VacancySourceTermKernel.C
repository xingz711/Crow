#include "VacancySourceTermKernel.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<VacancySourceTermKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("c", "Phase Field Variable");
  params.addCoupledVar("R1", "Random no. generation");
  params.addCoupledVar("R2", "Random no. generation");
  params.addParam<Real>("Pcasc", 0.5 ,"Probability of cascade occurance");
  params.addParam<Real>("Vg", 0.5 ,"Maximum increase in vacancy concentration");
  return params;
}

VacancySourceTermKernel::VacancySourceTermKernel(const std::string & name, InputParameters parameters) :
    Kernel(name, parameters),
    _c(coupledValue("c")),
    _R1(coupledValue("R1")),
    _R2(coupledValue("R2")),
    _Pcasc(getParam<Real>("Pcasc")),
    _Vg(getParam<Real>("Vg"))
    
{
}

Real
VacancySourceTermKernel::computeQpResidual()
{
  Real c = _c[_qp];
  Real R1 = _R1[_qp];
  
  if ( c <= 0.8 && R1<_Pcasc)
    return - _R2[_qp]*_Vg*_test[_i][_qp];
      
  return 0.0; 
}

//Real
//VacancySourceTermKernel::computeQpJacobian()
//{
 //return 0;
//}
//

