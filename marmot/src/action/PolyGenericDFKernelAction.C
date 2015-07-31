#include "PolyGenericDFKernelAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<PolyGenericDFKernelAction>()
{
  InputParameters params = validParams<Action>();

  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<std::vector<Real> >("DF","Driving force vector in J/m^3");
  params.addParam<Real>("length_scale",1.0e-9,"Length scale of the problem in meters");
  params.addParam<VariableName>("c","NONE","The concentration variable, if used");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");

  return params;
}

PolyGenericDFKernelAction::PolyGenericDFKernelAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _c(getParam<VariableName>("c"))
{
}

void
PolyGenericDFKernelAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolyGenericDFKernelAction Object\n";
  Moose::err << "var name base:" << _var_name_base;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable name
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    // Set name of kernel
    std::string kernel_type = "ACGrGrGenericDF";

    // Create input parameters
    InputParameters poly_params = _factory.getValidParams(kernel_type);
    poly_params.set<NonlinearVariableName>("variable") = var_name;

    if (_c != "NONE")
      poly_params.set<std::vector<VariableName> >("c").push_back(_c);


    poly_params.set<std::vector<Real> >("DF") = getParam<std::vector<Real> >("DF");
    poly_params.set<Real>("length_scale") = getParam<Real>("length_scale");
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    // Name kernel
    std::string kernel_name = "ACGenericDF_";
    kernel_name.append(var_name);

    // Add kernel
    _problem->addKernel(kernel_type, kernel_name, poly_params);
  }
}
