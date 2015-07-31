
#include "PolycrystalBubbleKernelAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<PolycrystalBubbleKernelAction>()
{
  InputParameters params = validParams<Action>();

  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addRequiredParam<VariableName>("cg", "Variable to be coupled as cg");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  return params;
}

PolycrystalBubbleKernelAction::PolycrystalBubbleKernelAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _cg(getParam<VariableName>("cg"))
{
}

void
PolycrystalBubbleKernelAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the PolycrystalBubbleKernelBlock Object\n";
  Moose::err << "var name base:" << _var_name_base << std::endl;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    std::vector<VariableName> v;
    v.resize(_op_num - 1);

    unsigned int ind = 0;

    for (unsigned int j = 0; j<_op_num; j++)
    {

      if (j != op)
      {
        std::string coupled_var_name = _var_name_base;
        std::stringstream out2;
        out2 << j;
        coupled_var_name.append(out2.str());
        v[ind] = coupled_var_name;
        ind++;
      }
    }

    std::vector<VariableName> cg(1);
    cg[0] = _cg;

    InputParameters poly_params = _factory.getValidParams("ACGrBuPoly");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::vector<VariableName> >("v") = v;
    poly_params.set<std::vector<VariableName> >("cg") = cg;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = "ACBulk_";
    kernel_name.append(var_name);

    _problem->addKernel("ACGrBuPoly", kernel_name, poly_params);

    /************/

    poly_params = _factory.getValidParams("ACInterface");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");
    // poly_params.set<std::vector<std::string> >("v") = v;

    poly_params.set<MaterialPropertyName>("kappa_name") = "kappa";


    kernel_name = "ACInt_";
    kernel_name.append(var_name);

    _problem->addKernel("ACInterface", kernel_name, poly_params);

    // *******************

    poly_params = _factory.getValidParams("TimeDerivative");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    kernel_name = "IE_";
    kernel_name.append(var_name);

    _problem->addKernel("TimeDerivative", kernel_name, poly_params);
  }
}
