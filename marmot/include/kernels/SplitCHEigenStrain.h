#ifndef SPLITCHEIGENSTRAIN_H
#define SPLITCHEIGENSTRAIN_H

#include "SplitCHBase.h"
#include "RankTwoTensor.h"

//Forward Declarations
class SplitCHEigenStrain;

template<>
InputParameters validParams<SplitCHEigenStrain>();

/**
 * The couple, SplitCHEigenStrain and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
 */
class SplitCHEigenStrain : public SplitCHBase
{
public:
  SplitCHEigenStrain(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type) ;

private:
  Real _epsilon0;
  const MaterialProperty<RankTwoTensor> & _stress;
};

#endif //SPLITCHEIGENSTRAIN_H
