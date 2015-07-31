#include "StressDivergenceTensorsPFFrac.h"

#include "Material.h"
#include "ElasticityTensorR4.h"
#include "RankTwoTensor.h"

template<>
InputParameters validParams<StressDivergenceTensorsPFFrac>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<unsigned int>("component", "An integer corresponding to the direction the variable this kernel acts in. (0 for x, 1 for y, 2 for z)");
  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  params.addCoupledVar("temp", "The temperature");
  params.addCoupledVar("c", "Fracture Parameter");
  params.addParam<std::string>("base_name", "Material property base name");
  // params.set<bool>("use_displaced_mesh") = true;
  // Using the displaced mesh will be set in the solid mechanics action input now.
  params.set<bool>("use_displaced_mesh") = false;
  return params;
}


StressDivergenceTensorsPFFrac::StressDivergenceTensorsPFFrac(const InputParameters & parameters) :
    DerivativeMaterialInterface<Kernel>(parameters),
    _c_name(_var.name()),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : "" ),

    _stress(getMaterialProperty<RankTwoTensor>(_base_name + "stress")),
    _d_stress_dc(getMaterialPropertyDerivative<RankTwoTensor>(_base_name + "stress", _c_name)),
    // _d_stress_dT(getMaterialProperty<RankTwoTensor>("stress", _temp_name)),

    _Jacobian_mult(getMaterialProperty<ElasticityTensorR4>(_base_name + "Jacobian_mult")),

    _component(getParam<unsigned int>("component")),
    _xdisp_coupled(isCoupled("disp_x")),
    _ydisp_coupled(isCoupled("disp_y")),
    _zdisp_coupled(isCoupled("disp_z")),
    _temp_coupled(isCoupled("temp")),
    _c_coupled(isCoupled("c")),
    _xdisp_var(_xdisp_coupled ? coupled("disp_x") : 0),
    _ydisp_var(_ydisp_coupled ? coupled("disp_y") : 0),
    _zdisp_var(_zdisp_coupled ? coupled("disp_z") : 0),
    _temp_var(_temp_coupled ? coupled("temp") : 0),
    _c_var(_c_coupled ? coupled("c") : 0)
{
}

Real
StressDivergenceTensorsPFFrac::computeQpResidual()
{
  return _stress[_qp].row(_component)*_grad_test[_i][_qp];
}

Real
StressDivergenceTensorsPFFrac::computeQpJacobian()
{
  return _Jacobian_mult[_qp].elasticJacobian( _component, _component, _grad_test[_i][_qp], _grad_phi[_j][_qp] );
  // return 0.0;
}

Real
StressDivergenceTensorsPFFrac::computeQpOffDiagJacobian(unsigned int jvar)
{
  unsigned int coupled_component = 0;
  bool active =false;

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
    return _Jacobian_mult[_qp].elasticJacobian( _component, coupled_component,
                                          _grad_test[_i][_qp], _grad_phi[_j][_qp] );
    // return 0.0;
  }

  if ( _temp_coupled && jvar == _temp_var )
  {
    // return _d_stress_dT[_qp].rowDot(_component, _grad_test[_i][_qp]) * _phi[_j][_qp];
    return 0.0;
  }

  // This block is pointless
  if (_c_coupled && jvar == _c_var)
  {
    Real val=0.0;

    for (unsigned int k = 0;k < 3; ++k)
      val += _d_stress_dc[_qp](_component,k) * _grad_test[_i][_qp](k);

    val *= _phi[_j][_qp];

    // printf("dstress_dc=%f %f %f\n",_d_stress_dc[_qp](0,0),_d_stress_dc[_qp](1,1),_d_stress_dc[_qp](2,2));
    // printf("val=%f\n",val);
  }

  return 0.0;
}
