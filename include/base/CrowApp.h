#ifndef CROWAPP_H
#define CROWAPP_H

#include "MooseApp.h"

class CrowApp;

template<>
InputParameters validParams<CrowApp>();

class CrowApp : public MooseApp
{
public:
  CrowApp(const InputParameters & parameters);
  CrowApp(const std::string & deprecated_name, InputParameters parameters); // DEPRECATED CONSTRUCTOR
  virtual ~CrowApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CROWAPP_H */
