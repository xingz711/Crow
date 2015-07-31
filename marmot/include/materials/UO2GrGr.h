#ifndef UO2GrGr_H
#define UO2GrGr_H

#include "GBEvolution.h"

//Forward Declarations
class UO2GrGr;

template<>
InputParameters validParams<UO2GrGr>();

class UO2GrGr : public GBEvolution
{
public:
  UO2GrGr(const InputParameters & parameters);

protected:
  //virtual void computeProperties();

private:

};

#endif //UO2GrGr_H
