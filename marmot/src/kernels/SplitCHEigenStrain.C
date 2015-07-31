#include "SplitCHEigenStrain.h"

template<>
InputParameters validParams<SplitCHEigenStrain>()
{
  InputParameters params = validParams<SplitCHBase>();
  params.addRequiredParam<Real>("epsilon0", "Eigen strain value");
  return params;
}

SplitCHEigenStrain::SplitCHEigenStrain(const InputParameters & parameters) :
    SplitCHBase(parameters),
    _epsilon0(getParam<Real> ("epsilon0")),
    _stress(getMaterialPropertyByName<RankTwoTensor> ("stress"))
{
}

Real
SplitCHEigenStrain::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
    case Jacobian:
      return 0.0; // return Jacobian value

    case Residual:
    {
      RankTwoTensor eigenstrain;
      eigenstrain.addIa(-_epsilon0);

      return eigenstrain.doubleContraction(_stress[_qp]); // return Residual value
    }
  }

  mooseError("Invalid type passed in");
}
