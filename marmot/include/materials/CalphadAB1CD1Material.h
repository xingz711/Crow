#ifndef CALPHADAB1CD1MATERIAL_H
#define CALPHADAB1CD1MATERIAL_H

#include "CalphadBaseMaterial.h"

// Forward Declarations
class CalphadAB1CD1Material;

template<>
InputParameters validParams<CalphadAB1CD1Material>();

/**
 * Derivative free energy material for a AB1CD1 phase material using the CALPHAD formalism
 * based on HYRAX work by Andrea M. Jokisaari
 */
class CalphadAB1CD1Material : public CalphadBaseMaterial
{
public:
  CalphadAB1CD1Material(const InputParameters & parameters);
};

#endif //CALPHADAB1CD1MATERIAL_H
