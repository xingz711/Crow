#include "PFEigenStrainMaterial.h"

template<>
InputParameters validParams<PFEigenStrainMaterial>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  params.addRequiredCoupledVar("v", "order parameters");
  params.addParam<Real>("e_c", "lattice missmatch co-eff");
  params.addParam<Real>("e_v", "lattice missmatch co-eff");
  return params;
}

PFEigenStrainMaterial::PFEigenStrainMaterial(const std::string & name,
                                                 InputParameters parameters) :
    EigenStrainBaseMaterial(name, parameters),
    _v(coupledValue("v")),
    _e_c(getParam<Real>("e_c")),
    _e_v(getParam<Real>("e_c"))
    
{
}

void
PFEigenStrainMaterial::computeEigenStrain()
{
  RankTwoTensor _e_cI;
  RankTwoTensor _e_vI;
  _e_cI.addIa(_e_c);
  _e_vI.addIa(_e_v);
  _eigenstrain[_qp] = _e_cI*_c[_qp] + _e_vI*_v[_qp];

}
