#ifndef ACPOREGBPURDUE_H
#define ACPOREGBPURDUE_H

#include "ACBulk.h"

//Forward Declarations
class ACPoreGBPurdue;

template<>
InputParameters validParams<ACPoreGBPurdue>();

/**
 * Computes the evolution for the Pore/GB interaction model based on Ahmed2013
 */
class ACPoreGBPurdue : public ACBulk
{
public:
  ACPoreGBPurdue(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  /// Vector containing all of the order parameter variables
  std::vector<VariableValue *> _vals;

  /// Vector containing variable IDs
  std::vector<unsigned int> _vals_var;

  /// Required material property for the model
  const MaterialProperty<Real> & _C;

  /// Concentration variable
  VariableValue & _c;

  /// ID of concentration variable
  unsigned int _c_var;

  /// Number of grain order parameters used in the model
  unsigned int _ncrys;
};

#endif //ACPOREGBPURDUE_H
