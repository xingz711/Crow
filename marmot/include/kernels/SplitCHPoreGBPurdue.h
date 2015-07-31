#ifndef SPLITCHPOREGBPURDUE_H
#define SPLITCHPOREGBPURDUE_H

#include "SplitCHCRes.h"

//Forward Declarations
class SplitCHPoreGBPurdue;

template<>
InputParameters validParams<SplitCHPoreGBPurdue>();

/**
 * CHPoreGBPurdue computes the dFdc term for the Pore/GB interaction model based on Ahmed2013
 */
class SplitCHPoreGBPurdue : public SplitCHCRes
{
public:
  SplitCHPoreGBPurdue(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  /// Vector containing all of the order parameter variables
  std::vector<VariableValue *> _vals;

  /// Vector containing variable IDs
  std::vector<unsigned int> _vals_var;

  /// Required material properties for the model
  const MaterialProperty<Real> & _B;
  const MaterialProperty<Real> & _C;

  /// Number of grain order parameters used in the model
  unsigned int _ncrys;
};

#endif //SPLITCHPOREGBPURDUE_H
