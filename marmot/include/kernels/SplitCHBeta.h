#ifndef SPLITCHBETA_H
#define SPLITCHBETA_H

#include "SplitCHBase.h"

//Forward Declarations
class SplitCHBeta;

template<>
InputParameters validParams<SplitCHBeta>();

class SplitCHBeta : public SplitCHBase
{
public:
  SplitCHBeta(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type) ;
  Real _Beta;
};

#endif //SPLITCHBETA_H
