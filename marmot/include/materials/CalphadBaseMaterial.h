#ifndef CALPHADBASEMATERIAL_H
#define CALPHADBASEMATERIAL_H

#include "DerivativeParsedMaterialHelper.h"
#include "ExpressionBuilder.h"

// Forward Declarations
class CalphadBaseMaterial;

template<>
InputParameters validParams<CalphadBaseMaterial>();

/**
 * Derivative free energy material base class for a single phase materials as function of
 * concentration \f$ c \f$ and Temperature \f$ T \f$ using the CALPHAD formalism.
 */
class CalphadBaseMaterial : public DerivativeParsedMaterialHelper,
                                   public ExpressionBuilder
{
public:
  CalphadBaseMaterial(const InputParameters & parameters);

  void finalize();

protected:
  std::vector<Real> _ep_lo;
  std::vector<Real> _ep_hi;

  std::vector<Real> _mix;

  /// Tolerance used for plog
  Real _plog_tol;

  /// Scaling factor
  Real _scaling_factor;

  /// Variables
  EBTerm _c, _T;

  /// Temperature expansion
  EBFunction _GTemperature;

  /// Ideal solution
  EBFunction _GIdeal;

  /// Excess free energy
  EBFunction _GEx;

  /// Reference free energy
  EBFunction _GRef;

  /// gas constant
  const Real _R;
};

#endif //CALPHADBASEMATERIAL_H
