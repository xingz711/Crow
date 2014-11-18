#ifndef DERIVATIVEPARSEDFASTMATERIAL_H
#define DERIVATIVEPARSEDFASTMATERIAL_H

#include "DerivativeParsedMaterialHelper.h"

// Forward Declarations
class DerivativeParsedFastMaterial;

template<>
InputParameters validParams<DerivativeParsedMaterial>();

/**
 * DerivativeBaseMaterial child class to evaluate a parsed function for the
 * free energy and automatically provide all derivatives.
 * This requires the autodiff patch (https://github.com/libMesh/libmesh/pull/238)
 * to Function Parser in libmesh.
 */
class DerivativeParsedFastMaterial : public DerivativeParsedFastMaterialHelper
{
public:
  DerivativeParsedFastMaterial(const std::string & name,
                           InputParameters parameters);

protected:
  virtual unsigned int expectedNumArgs();
};

#endif // DERIVATIVEPARSEDMATERIAL_H
