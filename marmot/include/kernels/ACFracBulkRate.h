/*
Formulation: Miehe et. al., Int. J. Num. Methods Engg., 2010, 83. 1273-1311
Equation 63
*/
#include "KernelValue.h"
#include "RankTwoTensor.h"

#ifndef ACFRACBULKRATE_H
#define ACFRACBULKRATE_H

//Forward Declarations
class ACFracBulkRate;

template<>
InputParameters validParams<ACFracBulkRate>();

class ACFracBulkRate : public KernelValue
{
public:
  ACFracBulkRate(const InputParameters & parameters);

protected:
  enum PFFunctionType
  {
    Jacobian,
    Residual
  };
  virtual Real precomputeQpResidual();
  virtual Real precomputeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  virtual Real computeDFDOP(PFFunctionType type);

  const MaterialProperty<Real> & _L;

  /// Critical energy release rate for fracture due to positive or hydrostatic component of stress to energy
  const MaterialProperty<Real> & _gc_prop_tens;
  /// Critical energy release rate for fracture due to non-positive or deviatoric component of stress to energy
  const MaterialProperty<Real> & _gc_prop_comp;

  /// Contribution from positive or hydrostatic component of stress to energy
  const MaterialProperty<Real> & _G0_pos;
  /// //Contribution from non-positive or deviatoric component of stress to energy
  const MaterialProperty<Real> & _G0_neg;

  const MaterialProperty<RankTwoTensor> & _dG0_pos_dstrain;
  VariableValue & _betaval;
  unsigned int _beta_var;

  const bool _xdisp_coupled;
  const bool _ydisp_coupled;
  const bool _zdisp_coupled;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;

  /// Critical energy release rate for fracture due to positive or hydrostatic component of stress to energy
  Real _gc_tens;

  /// Critical energy release rate for fracture due to non-positive or deviatoric component of stress to energy
  Real _gc_comp;

  /// Characteristic length, controls crack thickness
  Real _l;
  /// Viscosity (10^-3 provides rate independent behavior, slow convergence)
  Real _visco;
};

#endif //ACFRACBULKRATE_H
