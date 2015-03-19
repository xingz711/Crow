#ifndef CONSERVEDLANGEVINNOISEVOIDSOURCE_H
#define CONSERVEDLANGEVINNOISEVOIDSOURCE_H

#include "LangevinNoise.h"
#include "ConservedNoiseBase.h"

//Forward Declarations
class ConservedLangevinNoiseVoidSource;

template<>
InputParameters validParams<LangevinNoise>();

class ConservedLangevinNoiseVoidSource : public LangevinNoise
{
public:
  ConservedLangevinNoiseVoidSource(const std::string & name, InputParameters parameters);

protected:
  virtual void residualSetup() {};
  virtual Real computeQpResidual();

private:
  const ConservedNoiseInterface & _noise;
  VariableValue & _eta;
};

#endif //CONSERVEDLANGEVINNOISE_H
