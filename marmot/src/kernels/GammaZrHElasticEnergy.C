#include "GammaZrHElasticEnergy.h"

template<>
InputParameters validParams<GammaZrHElasticEnergy>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("eta2","Order parameter 2");
  params.addRequiredCoupledVar("eta3","Order parameter 3");
  params.addRequiredCoupledVar("C","Concentration of Hydrogen");
  params.addRequiredParam<std::vector<Real> >("eigen1","transformation strain for variable name it _trans1-3, match # with eta # of u");
  params.addRequiredParam<std::vector<Real> >("eigen2","transformation strain for variable 2 name it _trans1-3");
  params.addRequiredParam<std::vector<Real> >("eigen3","transformation strain for variable 3 name it _trans1-3");
  return params;
}

GammaZrHElasticEnergy::GammaZrHElasticEnergy(const InputParameters & parameters) :
    ACBulk(parameters),
    _eta2(coupledValue("eta2")),
    _eta3(coupledValue("eta3")),
    _C(coupledValue("C")),
    _trans1_input(getParam<std::vector<Real> >("eigen1")),
    _trans2_input(getParam<std::vector<Real> >("eigen2")),
    _trans3_input(getParam<std::vector<Real> >("eigen3")),
    _C_ijkl(getMaterialPropertyByName<ElasticityTensorR4>("elasticity_tensor")),
    _elastic_strain(getMaterialPropertyByName<RankTwoTensor>("elastic_strain"))
{
  _eps1.fillFromInputVector(_trans1_input);
  _eps2.fillFromInputVector(_trans2_input);
  _eps3.fillFromInputVector(_trans3_input);
}

Real
GammaZrHElasticEnergy::computeDFDOP(PFFunctionType type)
{
  Real tens1 = (_C_ijkl[_qp] * _eps1).doubleContraction(_elastic_strain[_qp]);

  Real C_eps11 = (_C_ijkl[_qp] * _eps1).doubleContraction(_eps1);
  Real C_eps12 = (_C_ijkl[_qp] * _eps1).doubleContraction(_eps2);
  Real C_eps13 = (_C_ijkl[_qp] * _eps1).doubleContraction(_eps3);

  switch (type)
  {
    case Residual:
      return ( - tens1 * _u[_qp]
               + (   C_eps11 * _u[_qp] * _u[_qp] * _u[_qp]
                   + C_eps12 * _u[_qp] * _eta2[_qp] * _eta2[_qp]
                   + C_eps13 * _u[_qp] * _eta3[_qp] * _eta3[_qp]
                 )
             ) * 2.0;

    case Jacobian:
      return (( - tens1
                + (   C_eps11 * 3.0 * _u[_qp] * _u[_qp]
                    + C_eps12 * _eta2[_qp] * _eta2[_qp]
                    + C_eps13 * _eta3[_qp] * _eta3[_qp]
                  )
             ) * 2.0) * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}
