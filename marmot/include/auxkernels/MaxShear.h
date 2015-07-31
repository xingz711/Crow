#ifndef MAXSHEAR_H
#define MAXSHEAR_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class MaxShear;

/**
 * RankTwoEigenvals is designed to take the data in the RankTwoTensor material
 * property, for example stress or strain, and output the eigenvalue for the
 * selected index, gives principal stresses or strains...
 */

template<>
InputParameters validParams<MaxShear>();

class MaxShear : public AuxKernel
{
public:
  MaxShear(const InputParameters & parameters);
  virtual ~MaxShear() {}

protected:
  virtual Real computeValue();

private:
  const MaterialProperty<RankTwoTensor> & _tensor;
  std::vector<Real> _vector;
};

#endif //MAXSHEAR_H
