// Coupled Stress Divergence Kernel
// author: Stephanie Pitts (sapitts@wsu.edu)
// Developed for CDDFSPlasticStrainGradMaterial
//
// This is a first cut of this implementation, updates and bug fixes to come.

#include "CoupleStressDivTensors.h"

#include "Material.h"
#include "ElasticityTensorR4.h"
#include "RankTwoTensor.h"

template<>
InputParameters validParams<CoupleStressDivTensors>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  params.addCoupledVar("temp", "The temperature");
  params.addParam<std::string>("appended_property_name", "", "Name appended to material properties to make them unique");
  // params.set<bool>("use_displaced_mesh") = true;
  // Using the displaced mesh will be set in the solid mechanics action input now.
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}


CoupleStressDivTensors::CoupleStressDivTensors(const InputParameters & parameters) :
    Kernel(parameters),
    _couple_stress_x(getMaterialProperty<RankTwoTensor>("couple_stress_x" + getParam<std::string>("appended_property_name"))),
    _couple_stress_y(getMaterialProperty<RankTwoTensor>("couple_stress_y" + getParam<std::string>("appended_property_name"))),
    _couple_stress_z(getMaterialProperty<RankTwoTensor>("couple_stress_z" + getParam<std::string>("appended_property_name"))),
    _Jacobian_mult(getMaterialProperty<ElasticityTensorR4>("Jacobian_mult" + getParam<std::string>("appended_property_name"))),
    // _d_couple_stress_dt(getMaterialProperty<std::vector<RankTwoTensor> >("d_couple_stress_dt"+ getParam<std::string>("appended_property_name"))),
    _component(getParam<unsigned int>("component")),
    _xdisp_coupled(isCoupled("disp_x")),
    _ydisp_coupled(isCoupled("disp_y")),
    _zdisp_coupled(isCoupled("disp_z")),
    // _temp_coupled(isCoupled("temp")),
    _xdisp_var(_xdisp_coupled ? coupled("disp_x") : 0),
    _ydisp_var(_ydisp_coupled ? coupled("disp_y") : 0),
    _zdisp_var(_zdisp_coupled ? coupled("disp_z") : 0),
    // _temp_var(_temp_coupled ? coupled("temp") : 0)
    _second_test(secondTest()),
    _second_phi(secondPhi())
{
}

Real
CoupleStressDivTensors::computeQpResidual()
{
  Real residual_x = 0.0, residual_y = 0.0, residual_z = 0;
  RankTwoTensor cs_x, cs_y, cs_z, test_fcn;

  // residual_x = _couple_stress_x[_qp].doubleContraction( _second_test[_i][_qp] );
  // residual_y = _couple_stress_y[_qp].doubleContraction( _second_test[_i][_qp] );
  // residual_z = _couple_stress_z[_qp].doubleContraction( _second_test[_i][_qp]);

  return residual_x + residual_y + residual_z;
}

Real
CoupleStressDivTensors::computeQpJacobian()
{
  // RankTwoTensor second_test, second_phi;
  // Real the_sum = 0.0;
  // Real placeholder_A_ijklmn = 1.0e6;

  // Inside the diagonal computation, _component dictates both i and l
  /*
  for (unsigned int j = 0; j < 3; ++j)
    for (unsigned int k = 0; k < 3; ++k)
      for (unsigned int m = 0; m < 3; ++m)
        for (unsigned int n = 0; n < 3; ++n)
        {
          the_sum += placeholder_A_ijklmn * _second_phi[_j][_qp](m,n) * _second_test[_i][_qp](j,k);
        }
  */
  // return the_sum;

  // return _Jacobian_mult[_qp].momentJacobian( _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp] );
  return 0.0;
}

Real
CoupleStressDivTensors::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  /*
  unsigned int coupled_component = 0;
  bool active =false;
  Real the_sum = 0.0, placeholder_A_ijklmn = 1.0e6;
  //  RankTwoTensor second_test, second_phi;
  if ( _xdisp_coupled && jvar == _xdisp_var )
  {
    coupled_component = 0;
    active = true;
  }
  else if ( _ydisp_coupled && jvar == _ydisp_var )
  {
    coupled_component = 1;
    active = true;
  }
  else if ( _zdisp_coupled && jvar == _zdisp_var )
  {
    coupled_component = 2;
    active = true;
  }

  if ( active )
  {
  // Inside the diagonal computation, _component dictates both i and coupled_component controls l
    for (unsigned int j = 0; j < 3; ++j)
      for (unsigned int k = 0; k < 3; ++k)
        for (unsigned int m = 0; m < 3; ++m)
          for (unsigned int n = 0; n < 3; ++n)
          {
            the_sum += placeholder_A_ijklmn * _second_phi[_j][_qp](m,n) * _second_test[_i][_qp](j,k);
          }

    return the_sum;
  }
  */
  /* First pass will not include additional variables
   * if ( _temp_coupled && jvar == _temp_var )
    {
      // return _d_stress_dT[_qp].rowDot(_component, _grad_test[_i][_qp]) * _phi[_j][_qp];
      return 0.0;
    }
  */

  return 0;
}
