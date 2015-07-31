#include "RankTwoEigenvals.h"

template<>
InputParameters validParams<RankTwoEigenvals>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<std::string>("rank_two_tensor", "The rank two material tensor name");
  params.addRequiredRangeCheckedParam<unsigned int>("index_i", "index_i >= 0 & index_i <= 2", "The index i for the vector to output (0, 1, 2)");

  return params;
}

RankTwoEigenvals::RankTwoEigenvals(const InputParameters & parameters) :
    AuxKernel(parameters),
    _tensor(getMaterialProperty<RankTwoTensor>(getParam<std::string>("rank_two_tensor"))),
    _i(getParam<unsigned int>("index_i"))

{
}

Real
RankTwoEigenvals::computeValue()
{
  _tensor[_qp].symmetricEigenvalues(_vector);

  return _vector[_i];
}
