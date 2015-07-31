#ifndef MARMOTAPP_H
#define MARMOTAPP_H

#include "MooseApp.h"

class MarmotApp;

template<>
InputParameters validParams<MarmotApp>();

/**
 * \mainpage Source code documentation
 *
 * \section Overview
 * MARMOT is a phase field code based of the [MOOSE Framework](http://mooseframework.org).
 * * Classes
 *  * [Index of Classes](classes.html)
 *  * [Annotated list of Classes](annotated.html)
 *  * [List of all class members](functions.html)
 * * [List of global symbols](globals.html)
 * * [List of files](files.html)
 * * [TODO list](todo.html)
 *
 * MARMOT is a multiphyiscs simulation tool focused on the mesoscale. It predicts the coevolution of microstructure and properties by coupling the phase field model with solid mechanics and heat conduction. Though MARMOT has a significant physics implemented in the code, it has also been developed to allow for the easy implementation of new models.
 *
 * MARMOT is implemented using the Multiphysics Object-Oriented Simulation Environment (MOOSE) framework from INL. Thus, the partial differential equations (PDEs) defining the coupled mesoscale physics are solved with the finite element method (FEM). Because of its connection to MOOSE, MARMOT is massively parallel (it can run on one processor but also on thousands of processors) and has access to mesh and time step adaptivity.
 *
 * MARMOT has two parts, generic models that can be used as the building blocks of a new mesoscale modeling tool and the application-specific models developed at INL that are primarily focused on nuclear applications. The generic models are located within the MOOSE modules library and come with the MOOSE framework. The application-specific models are in the MARMOT directory and require special permission for access. However, such access is easily obtained by contacting Michael Tonks at INL.
 */
class MarmotApp : public MooseApp
{
public:
  MarmotApp(InputParameters parameters);

  virtual void runInputFile();

  static void registerApps();
  static void registerObjects(Factory & factory);

protected:
  void printHeader();
};

#endif //MARMOTAPP_H
