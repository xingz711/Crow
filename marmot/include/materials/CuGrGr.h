#ifndef CUGRGR_H
#define CUGRGR_H

#include "GBEvolution.h"

//Forward Declarations
class CuGrGr;

template<>
InputParameters validParams<CuGrGr>();

/**
 * GBEvolution class with preset parameters from Schoenfelder1997.
 */
class CuGrGr : public GBEvolution
{
public:
  CuGrGr(const InputParameters & parameters);
};

#endif //CUGRGR_H
