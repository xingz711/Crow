#ifndef SPLITCHPHYS_H
#define SPLITCHPHYS_H

#include "SplitCHCRes.h"

//Forward Declarations
class SplitCHPhys;

template<>
InputParameters validParams<SplitCHPhys>();

/**
 * \todo this class has no test coverage
 */
class SplitCHPhys : public SplitCHCRes
{
public:
  SplitCHPhys(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

private:
  const MaterialProperty<Real> & _kT;

  /// Formation energy material property name
  std::string _Ef_name;
  /// Formation energy material property
  const MaterialProperty<Real> & _Efv;
};

#endif //SPLITCHPHYS_H
