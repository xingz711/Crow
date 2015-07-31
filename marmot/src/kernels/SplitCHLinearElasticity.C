#include "SplitCHLinearElasticity.h"

template<>
InputParameters validParams<SplitCHLinearElasticity>()
{
  InputParameters params = validParams<SplitCHBase>();
  params.addParam<std::string>("base_name", "Material property base name");
  return params;
}

SplitCHLinearElasticity::SplitCHLinearElasticity(const InputParameters & parameters) :
    DerivativeMaterialInterface<SplitCHBase>(parameters),

    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : "" ),
    _elastic_strain_name(_base_name + "elastic_strain"),
    _elasticity_tensor_name(_base_name + "elasticity_tensor"),

    _stress(getMaterialProperty<RankTwoTensor>(_base_name + "stress")),

    _elastic_strain(getMaterialProperty<RankTwoTensor> (_elastic_strain_name)),
    _delastic_strain_dc(getMaterialPropertyDerivative<RankTwoTensor>(_elastic_strain_name, _var.name())),
    _d2elastic_strain_dc2(getMaterialPropertyDerivative<RankTwoTensor>(_elastic_strain_name, _var.name(), _var.name())),

    _elasticity_tensor(getMaterialProperty<ElasticityTensorR4>(_elasticity_tensor_name)),
    _delasticity_tensor_dc(getMaterialPropertyDerivative<ElasticityTensorR4>(_elasticity_tensor_name, _var.name())),
    _d2elasticity_tensor_dc2(getMaterialPropertyDerivative<ElasticityTensorR4> (_elasticity_tensor_name, _var.name(), _var.name()))
{
}

Real // Example of what the virtual function should look like
SplitCHLinearElasticity::computeDEDC(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
    {
      Real value =   0.5 * (_delasticity_tensor_dc[_qp] * _elastic_strain[_qp]).doubleContraction(_elastic_strain[_qp])
                   + _delastic_strain_dc[_qp].doubleContraction(_stress[_qp]);

      // _console << "residual = " << value << std::endl;
      return value;
    }

    case Jacobian:
    {
      Real value =   0.5 * (_d2elasticity_tensor_dc2[_qp]*_elastic_strain[_qp]).doubleContraction(_elastic_strain[_qp])
                   + 2.0*  (_delasticity_tensor_dc[_qp]*_elastic_strain[_qp]).doubleContraction(_delastic_strain_dc[_qp])
                   + (_elasticity_tensor[_qp]*_delastic_strain_dc[_qp]).doubleContraction(_delastic_strain_dc[_qp])
                   + _d2elastic_strain_dc2[_qp].doubleContraction(_stress[_qp]);

      // _console << "jacobian = " << value << std::endl;
      return value * _phi[_j][_qp];
    }
  }

  mooseError("Invalid type passed in");
}
