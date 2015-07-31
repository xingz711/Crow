#ifndef RECON2RGBIC_H
#define RECON2RGBIC_H

#include "InitialCondition.h"
#include "EBSDReader.h"

class Recon2RGBIC;

template<>
InputParameters validParams<Recon2RGBIC>();

class Recon2RGBIC : public InitialCondition
{
public:
  Recon2RGBIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point &);

private:
  const EBSDReader & _ebsd_reader;
  unsigned int _sd;
};

#endif //RECON2RGBIC_H
