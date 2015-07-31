#include "XeVa2Diffusion.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<XeVa2Diffusion>()
{
  InputParameters params = validParams<CHBulk>();
  params.addParam<std::string>("LXeVa_name", "LXeVa", "The name of the Xe-Va regular solution parameter");
  params.addParam<std::string>("LUVa_name", "LUVa", "The name of the U-Va regular solution parameter");
  params.addParam<std::string>("LXeU_name", "LXeU", "The name of the Xe-U regular solution parameter");
  params.addParam<std::string>("LXeVaU_name", "LXeVaU", "The name of the XeVa-U regular solution parameter");
  params.addParam<std::string>("LXeVaVa_name", "LXeVaVa", "The name of the XeVa-Va regular solution parameter");
  params.addParam<std::string>("LXeVaXe_name", "LXeVaXe", "The name of the XeVa-Xe regular solution parameter");
  params.addParam<std::string>("LXeVa2_name", "LXeVa2", "The name of the Xe-VaVa regular solution parameter");
  params.addParam<std::string>("LXeVaVa2_name", "LXeVaVa2", "The name of the XeVa-VaVa regular solution parameter");
  params.addParam<std::string>("LUVa2_name", "LUVa2", "The name of the U-VaVa regular solution parameter");
  params.addParam<std::string>("LVaVa2_name", "LVaVa2", "The name of the Va-VaVa regular solution parameter");
  return params;
}

XeVa2Diffusion::XeVa2Diffusion(const InputParameters & parameters) :
    CHBulk(parameters),
    _LXeVa_name(getParam<std::string>("LXeVa_name")),
    _LXeVa(getMaterialProperty<Real>(_LXeVa_name)),
    _LUVa_name(getParam<std::string>("LUVa_name")),
    _LUVa(getMaterialProperty<Real>(_LUVa_name)),
    _LXeU_name(getParam<std::string>("LXeU_name")),
    _LXeU(getMaterialProperty<Real>(_LXeU_name)),
    _LXeVaU_name(getParam<std::string>("LXeVaU_name")),
    _LXeVaU(getMaterialProperty<Real>(_LXeVaU_name)),
    _LXeVaVa_name(getParam<std::string>("LXeVaVa_name")),
    _LXeVaVa(getMaterialProperty<Real>(_LXeVaVa_name)),
    _LXeVaXe_name(getParam<std::string>("LXeVaXe_name")),
    _LXeVaXe(getMaterialProperty<Real>(_LXeVaXe_name)),
    _LXeVa2_name(getParam<std::string>("LXeVa2_name")),
    _LXeVa2(getMaterialProperty<Real>(_LXeVa2_name)),
    _LXeVaVa2_name(getParam<std::string>("LXeVaVa2_name")),
    _LXeVaVa2(getMaterialProperty<Real>(_LXeVaVa2_name)),
    _LUVa2_name(getParam<std::string>("LUVa2_name")),
    _LUVa2(getMaterialProperty<Real>(_LUVa2_name)),
    _LVaVa2_name(getParam<std::string>("LVaVa2_name")),
    _LVaVa2(getMaterialProperty<Real>(_LVaVa2_name)),
    _kT(getMaterialProperty<Real>("kT")),
    _cu(getMaterialProperty<Real>("cu")),
    _grad_cu(getMaterialProperty<RealGradient>("grad_cu"))
{
}

RealGradient  //Use This an example of the the function should look like
XeVa2Diffusion::computeGradDFDCons(PFFunctionType type)
{

  //XeVaVa diffusion

  switch (type)
  {
    case Residual:
      return _kT[_qp] * (_grad_u[_qp] - 3 * _u[_qp] / (_cu[_qp]) * _grad_cu[_qp]);

    case Jacobian:

      return _kT[_qp] * (
               _grad_phi[_j][_qp]
               - 3.0 * _phi[_j][_qp] / (_cu[_qp]) * _grad_cu[_qp]
               - 3.0 * _u[_qp] / (_cu[_qp]) * (-3.0 * _grad_phi[_j][_qp])
               - 3.0 * _u[_qp] / (_cu[_qp]) / (_cu[_qp]) * (-3.0 * _phi[_j][_qp]) * _grad_cu[_qp]
             );
  }

  mooseError("Invalid type passed in");
}
