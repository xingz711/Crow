#ifndef STRESSDIVERGENCETENSORSPFFRAC_H
#define STRESSDIVERGENCETENSORSPFFRAC_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class StressDivergenceTensorsPFFrac;
class ElasticityTensorR4;
class RankTwoTensor;

template<>
InputParameters validParams<StressDivergenceTensorsPFFrac>();

/**
 * StressDivergenceTensors mostly copies from StressDivergence.  There are small changes to use
 * RankFourTensor and RankTwoTensors instead of SymmElasticityTensors and SymmTensors.  This is done
 * to allow for more mathematical transparancy.
 * \tod is inheritance a better option than copy+paste?
 */
class StressDivergenceTensorsPFFrac : public DerivativeMaterialInterface<Kernel>
{
public:
  StressDivergenceTensorsPFFrac(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  VariableName _c_name;
  std::string _base_name;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _d_stress_dc;
  // const MaterialProperty<RankTwoTensor> & _d_stress_dT;

  const MaterialProperty<ElasticityTensorR4> & _Jacobian_mult;

  const unsigned int _component;

  const bool _xdisp_coupled;
  const bool _ydisp_coupled;
  const bool _zdisp_coupled;
  const bool _temp_coupled;
  const bool _c_coupled;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
  const unsigned int _temp_var;
  const unsigned int _c_var;
};

#endif //STRESSDIVERGENCETENSORSPFFRAC_H
