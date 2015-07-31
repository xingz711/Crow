#ifndef CALPHADAB1CD2MATERIAL_H
#define CALPHADAB1CD2MATERIAL_H

#include "CalphadBaseMaterial.h"

// Forward Declarations
class CalphadAB1CD2Material;

template<>
InputParameters validParams<CalphadAB1CD2Material>();

/**
 * Derivative free energy material for a AB1CD2 phase material using the CALPHAD formalism
 * based on HYRAX work by Andrea M. Jokisaari
 */
class CalphadAB1CD2Material : public CalphadBaseMaterial
{
public:
  CalphadAB1CD2Material(const InputParameters & parameters);

protected:
  /// Redlich-Kister Lx polynomial coefficients
  std::vector<Real> _L0;
  std::vector<Real> _L1;

  /// Pure endpoint at low composition in the first phase
  std::vector<Real> _pure_ep1;
};

#endif //CALPHADAB1CD2MATERIAL_H
