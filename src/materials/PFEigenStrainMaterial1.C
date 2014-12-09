#include "PFEigenStrainMaterial1.h"
#include "AddV.h"
template<>
InputParameters validParams<PFEigenStrainMaterial1>()
{
  InputParameters params = validParams<EigenStrainBaseMaterial>();
  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredCoupledVar("v", "order parameters");
  params.addRequiredParam<unsigned int>("op_num", "number of grains");
  params.addRequiredParam<std::string>("var_name_base", "base for variable names");
  params.addParam<Real>("e_c", "lattice mismatch co-eff");
  params.addParam<Real>("e_v", "lattice mismatch co-eff");
  return params;
}

PFEigenStrainMaterial1::PFEigenStrainMaterial1(const std::string & name,
                                                 InputParameters parameters) :
    EigenStrainBaseMaterial(name, parameters),
    _c(coupledValue("c")),
    _e_c(getParam<Real>("e_c")),
    _e_v(getParam<Real>("e_c"))
    
{
  _ncrys = coupledComponents("v");
  _vals.resize(_ncrys);

  for (unsigned int i=0; i < _ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
}

void
PFEigenStrainMaterial1::computeEigenStrain()
{
  RankTwoTensor _e_cI;
  RankTwoTensor _e_vI;
  _e_cI.addIa(_e_c);
  _e_vI.addIa(_e_v);
  Real sumetaj = 0.0;
  for (unsigned int i = 0; i < _ncrys; ++i)
    sumetaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; 
  
  _eigenstrain[_qp] = _e_cI*_c[_qp] + _e_vI*sumetaj;

}
