#ifndef MARMOTUNITAPP_H
#define MARMOTUNITAPP_H

#include "MarmotApp.h"

class MarmotUnitApp;

template<>
InputParameters validParams<MarmotUnitApp>();

class MarmotUnitApp : public MarmotApp
{
public:
  MarmotUnitApp(InputParameters parameters);
  virtual ~MarmotUnitApp();
};

#endif //MARMOTUNITAPP_H
