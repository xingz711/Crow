#include "FiniteStrainElasticPFDamage.h"

#include "libmesh/petsc_macro.h"
#include <petscsys.h>
#include <petscblaslapack.h>

#if !defined(LIBMESH_HAVE_PETSC)
  extern "C" void FORTRAN_CALL(dsyev) ( ... );
#endif

template<>
InputParameters validParams<FiniteStrainElasticPFDamage>()
{
  InputParameters params = validParams<FiniteStrainMaterial>();
  params.addRequiredCoupledVar("c","Concentration");
  params.addParam<FunctionName>("function", "Bilinear function describing property distribution");
  params.addParam<Real>("kvoid",1e-6,"Stiffness of cracked matrix");
  params.addParam<FileName>("void_prop_file_name","", "Name of the file containing d values");
  return params;
}

FiniteStrainElasticPFDamage::FiniteStrainElasticPFDamage(const InputParameters & parameters) :
    DerivativeMaterialInterface<FiniteStrainMaterial>(parameters),
    _c(coupledValue("c")),
    _c_name(getVar("c", 0)->name()),

    _delasticity_tensor_dc(declarePropertyDerivative<ElasticityTensorR4>(_elasticity_tensor_name, _c_name)),
    _d2elasticity_tensor_dc2(declarePropertyDerivative<ElasticityTensorR4>(_elasticity_tensor_name, _c_name, _c_name)),

    _dstress_dc(declarePropertyDerivative<RankTwoTensor>(_base_name + "stress", _c_name)),

    _G0_pos(declareProperty<Real>("G0_pos")),
    _G0_pos_old(declarePropertyOld<Real>("G0_pos")),
    _G0_neg(declareProperty<Real>("G0_neg")),
    _stress0(declareProperty<RankTwoTensor>("stress0")),
    _stress0_old(declarePropertyOld<RankTwoTensor>("stress0")),
    _d_void(declareProperty<Real>("d_void")),
    _d_void_old(declarePropertyOld<Real>("d_void")),
    _has_function(isParamValid("function")),
    _function( _has_function ? &getFunction("function") : NULL ),
    _kvoid(getParam<Real>("kvoid")),
    _void_prop_file_name(getParam<FileName>("void_prop_file_name"))
{
  if (_has_function && !_function)
  {
    Moose::err << "Unable to find function in LinearIsoElasticPFAnisoDamVoid." << std::endl;
    libmesh_error();
  }
}

void FiniteStrainElasticPFDamage::initQpStatefulProperties()
{
  read_prop();

  if (_has_function)
    _d_void[_qp] = _function->value(_t, _q_point[_qp])+1e-8;
  else
    _d_void[_qp]=1.0;

  _G0_pos[_qp]=0.0;
  _stress0[_qp].zero();
}

void FiniteStrainElasticPFDamage::computeQpStress()
{
  _stress[_qp].zero();
  update_var(_strain_increment[_qp]);
}

void FiniteStrainElasticPFDamage::computeQpElasticityTensor()
{
}

void
FiniteStrainElasticPFDamage::read_prop()
{
  std::ifstream fileprop;
  Real xmin, xmax, ymin, ymax;
  int istart = 0;

  Real xgp, ygp;

  xgp=_q_point[_qp](0);
  ygp=_q_point[_qp](1);


  _d_void[_qp]=1.0;

  if (_void_prop_file_name.length()!=0)
  {
    fileprop.open(_void_prop_file_name.c_str());
    if (!fileprop)
      mooseError("Can't open property file ");
    else
    {
      while (!fileprop.eof())
      {
        fileprop >> xmin;
        fileprop >> xmax;
        fileprop >> ymin;
        fileprop >> ymax;

        if (istart == 0)
        {
          if ((xgp < xmin || xgp > xmax) && (ygp < ymin || ygp > ymax))
          {
            _d_void[_qp]=1.0;
            fileprop.close();
            return;
          }

          istart=1;
        }
        else
        {
          if ((xgp > xmin && xgp < xmax) && (ygp > ymin && ygp < ymax))
          {
            _d_void[_qp]=1e-8;
            fileprop.close();
            return;
          }
        }
      }

      fileprop.close();
    }
  }
}

void FiniteStrainElasticPFDamage::update_var(RankTwoTensor &strain_inc)
{
  RankTwoTensor etens1, etens2, etens3;
  Real cmat[3][3], w[3], work[20];
  int info;
  int nd = 3;
  int lwork = 20;
  RankTwoTensor stress0pos, stress0neg, dstress0;
  Real evec1[3], evec2[3], evec3[3];

  _elasticity_tensor[_qp] = _Cijkl;
  _elasticity_tensor[_qp] *= _d_void[_qp];

  dstress0 = _elasticity_tensor[_qp] * strain_inc;

  _stress0[_qp] = _stress0_old[_qp] + dstress0;
  _stress0[_qp] = _rotation_increment[_qp] * _stress0[_qp] * _rotation_increment[_qp].transpose();


  for (unsigned int i = 0; i < 3; ++i)
    for (unsigned int j = 0; j < 3; ++j)
      cmat[i][j] = _stress0[_qp](i,j);

#if !defined(LIBMESH_HAVE_PETSC)
  FORTRAN_CALL(dsyev)("V", "U", &nd, &cmat[0][0], &nd, w, work, &lwork, &info);
#else
  LAPACKsyev_("V", "U", &nd, &cmat[0][0], &nd, w, work, &lwork, &info);
#endif


  for (unsigned int i = 0; i < 3; ++i)
  {
    evec1[i] = cmat[0][i];
    evec2[i] = cmat[1][i];
    evec3[i] = cmat[2][i];
  }

  for (unsigned int i = 0; i < 3; ++i)
    for (unsigned int j = 0; j < 3; ++j)
    {
      etens1(i,j) = evec1[i] * evec1[j];
      etens2(i,j) = evec2[i] * evec2[j];
      etens3(i,j) = evec3[i] * evec3[j];
    }

  stress0pos = etens1 * (std::abs(w[0])+w[0])/2.0 + etens2 * (std::abs(w[1])+w[1])/2.0 + etens3 * (std::abs(w[2])+w[2])/2.0;
  stress0neg = etens1 * (std::abs(w[0])-w[0])/2.0 + etens2 * (std::abs(w[1])-w[1])/2.0 + etens3 * (std::abs(w[2])-w[2])/2.0;

  Real c = _c[_qp];
  Real xfac = std::pow(1.0 - c, 2.0) + _kvoid;

  _stress[_qp] = stress0pos * xfac - stress0neg;

  _G0_pos[_qp] = _G0_pos_old[_qp] + stress0pos.doubleContraction(strain_inc);
  _G0_neg[_qp] = 0.0; // Attention!!!!!!

  _Jacobian_mult[_qp] = _elasticity_tensor[_qp];
  _dstress_dc[_qp] *= 0.0;
}
