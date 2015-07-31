#include "ACGrGrTensorCrystPlasticDF.h"
#include "Material.h"

template<>
InputParameters validParams<ACGrGrTensorCrystPlasticDF>()
{
  InputParameters params = validParams<ACBulk>();
  params.addParam<MaterialPropertyName>("elastic_name", 0.0, "The elastic energy derivative for the specific order parameter");
  params.addParam<MaterialPropertyName>("plastic_name", 0.0, "The plastic potential derivative for the specific order parameter");
  return params;
}

ACGrGrTensorCrystPlasticDF::ACGrGrTensorCrystPlasticDF(const InputParameters & parameters) :
    ACBulk(parameters),
    _elastic_driving_force(getMaterialProperty<Real>("elastic_name")),
    _plastic_driving_force(getMaterialProperty<Real>("plastic_name"))
{
}

Real
ACGrGrTensorCrystPlasticDF::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return _elastic_driving_force[_qp] + _plastic_driving_force[_qp];

    case Jacobian:
      return 0.0;
  }

  mooseError("Invalid type passed in");
}
