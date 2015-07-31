#include "MaxShear.h"

template<>
InputParameters validParams<MaxShear>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<MaterialPropertyName>("rank_two_tensor", "The rank two material tensor name");

  return params;
}

MaxShear::MaxShear(const InputParameters & parameters) :
    AuxKernel(parameters),
    _tensor(getMaterialProperty<RankTwoTensor>("rank_two_tensor"))
{
}

Real
MaxShear::computeValue()
{
  _tensor[_qp].symmetricEigenvalues(_vector);
  Real tau1 = (_vector[1] - _vector[2]) / 2.0;
  Real tau2 = (_vector[0] - _vector[2]) / 2.0;
  Real tau3 = (_vector[0] - _vector[1]) / 2.0;

  // return the maximum tau value
  Real reals[] = {tau3, -tau3, tau2, -tau2, tau1, -tau1};
  return *std::max_element(reals, reals+6);
}
