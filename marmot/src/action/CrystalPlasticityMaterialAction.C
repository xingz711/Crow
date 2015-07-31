#include "CrystalPlasticityMaterialAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<CrystalPlasticityMaterialAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<Real>("c11", "Material modulus C11");
  params.addRequiredParam<Real>("c12", "Material modulus C12");
  params.addRequiredParam<Real>("c44", "Material modulus C44");
  params.addRequiredParam<Real>("youngs_modulus", "Material constant one");
  params.addRequiredParam<Real>("poissons_ratio", "Material constant two");
  params.addParam<Real>("g0", 60.8, "Initial flow stress");
  params.addParam<Real>("h0", 541.5, "Hardening parameter");
  params.addParam<Real>("q0", 1.0, "Latenheat parameter");
  params.addParam<Real>("a0", 0.001, "Reference strain rate");
  params.addParam<Real>("tau0",109.8, "Initial shear strength");
  params.addRequiredParam<FileName>("euler_angle_file_name", "Name of the file containing the euler angles");
  params.addRequiredParam<FileName>("slip_plane_file_name", "Name of the file containing the slip plane");
  params.addRequiredParam<Real>("nsp", "number of slip planes");
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  return params;
}

CrystalPlasticityMaterialAction::CrystalPlasticityMaterialAction(const InputParameters & params) :
    Action(params),
    _c11(getParam<Real>("c11")),
    _c12(getParam<Real>("c12")),
    _c44(getParam<Real>("c44")),
    _youngs_modulus(getParam<Real>("youngs_modulus")),
    _poissons_ratio(getParam<Real>("poissons_ratio")),
    _g0(getParam<Real>("g0")),
    _h0(getParam<Real>("h0")),
    _q0(getParam<Real>("q0")),
    _a0(getParam<Real>("a0")),
    _tau0(getParam<Real>("tau0")),
    _euler_angle_file_name(getParam<FileName>("euler_angle_file_name")),
    _slip_plane_file_name(getParam<FileName>("slip_plane_file_name")),
    _nss(getParam<unsigned int>("nsp")),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{
  std::ifstream myfile;
  myfile.open(_euler_angle_file_name.c_str());
  _angle.resize(3 * _op_num);

  // Reading euler angles from inputfile
  for (unsigned int i = 0; i < _op_num; ++i)
    myfile >> _angle[0+(i*3)]  >> _angle[1+(i*3)]  >> _angle[2+(i*3)];
}

void
CrystalPlasticityMaterialAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the CrystalPlasticityMaterialAction Object\n";
  Moose::err << "var name base:" << _var_name_base;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    InputParameters poly_params = _factory.getValidParams("AnisotropicCrystalPlasticity");
    poly_params.set<Real>("youngs_modulus") = _youngs_modulus;
    poly_params.set<Real>("poissons_ratio") = _poissons_ratio;
    poly_params.set<Real>("c11") = _c11;
    poly_params.set<Real>("c12") = _c12;
    poly_params.set<Real>("c44") = _c44;
    poly_params.set<Real>("g0") = _g0;
    poly_params.set<Real>("h0") = _h0;
    poly_params.set<Real>("q0") = _q0;
    poly_params.set<Real>("a0") = _a0;
    poly_params.set<Real>("tau0") = _tau0;
    poly_params.set<FileName>("slip_plane_file_name") = _slip_plane_file_name;
    poly_params.set<unsigned int>("nsp") = _nss;
    poly_params.set<Real>("angle1") = _angle[0+(op*3)];
    poly_params.set<Real>("angle2") = _angle[1+(op*3)];
    poly_params.set<Real>("angle3") = _angle[2+(op*3)];

    poly_params.set<std::string>("increment_calculation") = "eigen";
    std::vector<unsigned int> block(1);
    block[0] = op + 1;
    poly_params.set<std::vector<unsigned int> >("block") = block;
    std::vector<std::string> disp(1);
    disp[0] = "disp_x";
    poly_params.set<std::vector<std::string> >("disp_x") =  disp;
    disp[0] = "disp_y";
    poly_params.set<std::vector<std::string> >("disp_y") =  disp;
    disp[0] = "disp_z";
    poly_params.set<std::vector<std::string> >("disp_z") =  disp;

    Moose::err << poly_params;

    std::string material_name = "CrystalPlasticity_block_";
    material_name.append(var_name);

    _problem->addMaterial("AnisotrophicCrystalPlasticity", material_name, poly_params);
  }
}
