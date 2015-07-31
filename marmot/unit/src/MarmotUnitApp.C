#include "MarmotUnitApp.h"
#include "Moose.h"

template<>
InputParameters validParams<MarmotUnitApp>()
{
  InputParameters params = validParams<MarmotApp>();
  return params;
}

MarmotUnitApp::MarmotUnitApp(InputParameters parameters) :
    MarmotApp(parameters)
{
}

MarmotUnitApp::~MarmotUnitApp()
{
}
