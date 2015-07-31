#ifndef RANKTWOEIGENVALS_H
#define RANKTWOEIGENVALS_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class RankTwoEigenvals;

/**
 * RankTwoEigenvals is designed to take the data in the RankTwoTensor material
 * property, for example stress or strain, and output the eigenvalue for the
 * selected index, gives principal stresses or strains...
 */

template<>
InputParameters validParams<RankTwoEigenvals>();

class RankTwoEigenvals : public AuxKernel
{
public:
  RankTwoEigenvals(const InputParameters & parameters);
  virtual ~RankTwoEigenvals() {}

protected:
  virtual Real computeValue();

private:

  const MaterialProperty<RankTwoTensor> & _tensor;
  const unsigned int _i;
  std::vector<Real> _vector;

};

#endif //RANKTWOEIGENVALS_H
