#include "PolyCrystPlasticDFKernelAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<PolyCrystPlasticDFKernelAction>()
{
  InputParameters params = validParams<Action>();

  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<NonlinearVariableName>("disp_x_name", "The x-displacement");
  params.addRequiredParam<NonlinearVariableName>("disp_y_name", "The y-displacement");
  params.addParam<NonlinearVariableName>("disp_z_name", "The z-displacement");
  params.addParam<bool>("Tensor_form", false,"Indicates if the tensor form is used insteady of the symmetric tensor form");
  params.addParam<bool>("Elastic_driving_force", true, "Consider elastic driving force or not");
  params.addParam<bool>("Plastic_driving_force", true, "Consider plastic driving force or not");
  params.addParam<std::vector<Real> >("applied_strain_vector", "Applied strain: e11, e22, e33, e23, e13, e12");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  return params;
}

PolyCrystPlasticDFKernelAction::PolyCrystPlasticDFKernelAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _Tensor_form(getParam<bool>("Tensor_form")),
    _elastic_driving_force(getParam<bool>("Elastic_driving_force")),
    _plastic_driving_force(getParam<bool>("Plastic_driving_force"))
{
}

void
PolyCrystPlasticDFKernelAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolyCrystPlasticDFKernelAction Object\n";
  Moose::err << "var name base:" << _var_name_base;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable name
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    // Create potential derivative name
    std::string elastic_name = "elastic_driving_force";
    std::string plastic_name = "plastic_driving_force";
    elastic_name.append(out.str());
    plastic_name.append(out.str());
    std::string kernel_type;

    kernel_type = "ACGrGrTensorCrystPlasticDF";
    InputParameters poly_params = _factory.getValidParams(kernel_type);
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::string>("elastic_name") = elastic_name;
    poly_params.set<std::string>("plastic_name") = plastic_name;
    poly_params.set<bool>("elastic_force") = _elastic_driving_force;
    poly_params.set<bool>("plastic_force") = _plastic_driving_force;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");
    poly_params.set<std::vector<NonlinearVariableName> >("disp_x").push_back(getParam<NonlinearVariableName>("disp_x_name"));
    poly_params.set<std::vector<NonlinearVariableName> >("disp_y").push_back(getParam<NonlinearVariableName>("disp_y_name"));
    if (isParamValid("disp_z_name"))
      poly_params.set<std::vector<NonlinearVariableName> >("disp_z").push_back(getParam<NonlinearVariableName>("disp_z_name"));

    std::string kernel_name = "ACCrystPlastic_";
    kernel_name.append(var_name);

    _problem->addKernel(kernel_type, kernel_name, poly_params);
  }
}
