// Coupled Stress Divergence Kernel
// author: Stephanie Pitts (sapitts@wsu.edu)
// Developed for CDDFSPlasticStrainGradMaterial
//
// This is a first cut of this implementation, updates and bug fixes to come.

#ifndef COUPLESTRESSDIVTENSORS_H
#define COUPLESTRESSDIVTENSORS_H

#include "Kernel.h"

//Forward Declarations
class CoupleStressDivTensors;
class ElasticityTensorR4;
class RankTwoTensor;

template<>
InputParameters validParams<CoupleStressDivTensors>();

/**
 * CoupleStressDivTensors mostly copies from StressDivergenceTensors.  There are small changes to use
 * the work conjugate of the strain gradient rather than of the strain, including calls to rank-3
 * Jacobian calculation.
 * \tod is inheritance a better option than copy+paste?
 */
class CoupleStressDivTensors : public Kernel
{
public:

  CoupleStressDivTensors(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const MaterialProperty<RankTwoTensor> & _couple_stress_x;
  const MaterialProperty<RankTwoTensor> & _couple_stress_y;
  const MaterialProperty<RankTwoTensor> & _couple_stress_z;
  const MaterialProperty<ElasticityTensorR4> & _Jacobian_mult;
  // const MaterialProperty<std::vector<RankTwoTensor> > & _d_couple_stress_dt;

  const unsigned int _component;

  const bool _xdisp_coupled;
  const bool _ydisp_coupled;
  const bool _zdisp_coupled;
  // const bool _temp_coupled;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
  // const unsigned int _temp_var;

private:
  VariableTestSecond & _second_test;
  VariablePhiSecond & _second_phi;
};

#endif //CoupleStressDivTensors_H
