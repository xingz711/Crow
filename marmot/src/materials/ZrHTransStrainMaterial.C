#include "ZrHTransStrainMaterial.h"

template<>
InputParameters validParams<ZrHTransStrainMaterial>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  params.addRequiredCoupledVar("eta1", "Order parameter 1");
  params.addRequiredCoupledVar("eta2", "Order parameter 2");
  params.addRequiredCoupledVar("eta3", "Order parameter 3");
  return params;
}

ZrHTransStrainMaterial::ZrHTransStrainMaterial(const InputParameters & parameters) :
    EigenStrainBaseMaterial(parameters),
    _eta1(coupledValue("eta1")),
    _eta2(coupledValue("eta2")),
    _eta3(coupledValue("eta3")),
    _trans1(declareProperty<RankTwoTensor>("trans1")),
    _trans2(declareProperty<RankTwoTensor>("trans2")),
    _trans3(declareProperty<RankTwoTensor>("trans3"))
{
  // _trans1_input = '0.00551 0.0564 0.057 0 0 0'
  // _trans2_input = '0.043675 0.018232 0.057 0 0 0.022035'
  // _trans3_input = '0.043675 0.018232 0.057 0 0 -0.022035'
}

void
ZrHTransStrainMaterial::computeEigenStrain()
{
  _trans1[_qp] = RankTwoTensor(0.00551, 0.0564, 0.057, 0, 0, 0);
  _trans2[_qp] = RankTwoTensor(0.043675, 0.018232, 0.057, 0, 0, 0.022035);
  _trans3[_qp] = RankTwoTensor(0.043675, 0.018232, 0.057, 0, 0, -0.022035);

  _eigenstrain[_qp] = _trans1[_qp] * _eta1[_qp] * _eta1[_qp] + _trans2[_qp] * _eta2[_qp] * _eta2[_qp] + _trans3[_qp] * _eta3[_qp] * _eta3[_qp];
}
