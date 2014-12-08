#include "PFEigenStrainMaterial.h"

template<>
InputParameters validParams<PFEigenStrainMaterial>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  //params.addRequiredCoupledVar("v", "order parameters");
  params.addParam<Real>("e_c", "lattice missmatch co-eff");
  return params;
}

PFEigenStrainMaterial::PFEigenStrainMaterial(const std::string & name,
                                                 InputParameters parameters) :
    EigenStrainBaseMaterial(name, parameters),
    _e_c(getParam<Real>("e_c"))
    
{
}

void
PFEigenStrainMaterial::computeEigenStrain()
{
  RankTwoTensor _e_cI;
  _e_cI.addIa(_e_c);
  _eigenstrain[_qp] = _e_cI*_c[_qp];

}
