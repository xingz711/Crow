#include "CHRadSink.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<CHRadSink>()
{
  InputParameters params = validParams<CHBulk>();
  params.addCoupledVar("v", "Array of coupled variables");
  return params;
}

CHRadSink::CHRadSink(const InputParameters & parameters) :
    CHBulk(parameters),
    _u_old(valueOld()),
    _C_i(getMaterialPropertyByName<Real>("C_i")),
    _m_i(getMaterialPropertyByName<Real>("m_i")),
    _b_i(getMaterialPropertyByName<Real>("b_i")),
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (unsigned int i = 0; i < _n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

RealGradient
CHRadSink::computeGradDFDCons(PFFunctionType type)
{
  const Real tol = 1.0e-5;
  RealGradient value = 0.0;

  Real f_i = 0.0;
  RealGradient grad_f_i = 0.0;
  for (unsigned int i=0; i<_n; ++i)
    for (unsigned int j=i+1; j<_n; ++j)
    {
      f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      grad_f_i += -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      grad_f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];
    }

  Real inside_g_i = 100.0 * _u[_qp] + std::exp(-_b_i[_qp]/_m_i[_qp]);

  Real g_i = _m_i[_qp] * poly2Log(inside_g_i, tol, 0) + _m_i[_qp];
  Real dgidc = 100.0 * _m_i[_qp] * poly2Log(inside_g_i, tol, 1);
  Real d2gidc2 = 100.0*100.0 * _m_i[_qp] * poly2Log(inside_g_i, tol, 2);

  RealGradient grad_g_i = _grad_u[_qp]*dgidc;
  RealGradient grad_dgidc = _grad_u[_qp]*d2gidc2;

  switch (type)
  {
    case Residual:
      value = 2.0*(-grad_g_i*f_i + (_C_i[_qp] - g_i)*grad_f_i - _grad_u[_qp]*dgidc*f_i - _u[_qp]*grad_dgidc*f_i - _u[_qp]*dgidc*grad_f_i);

      if (_u_old[_qp] < 1.0e-4)
        value *= 0.0;

      return  value;

    case Jacobian:
      Real d3gidc3 = 100.0*100.0*100.0 * _m_i[_qp] * poly2Log(inside_g_i, tol, 3);

      Real dg_i = _phi[_j][_qp]*dgidc;
      RealGradient dgrad_g_i = (_grad_phi[_j][_qp]*dgidc + _phi[_j][_qp] * grad_dgidc);

      Real ddgidc = _phi[_j][_qp] * d2gidc2;
      RealGradient dgrad_dgidc = (_grad_phi[_j][_qp] * d2gidc2 + _phi[_j][_qp] * _grad_u[_qp] * d3gidc3);

      value = -2.0 * (dgrad_g_i * f_i + dg_i * grad_f_i + _grad_phi[_j][_qp] * dgidc * f_i + _grad_u[_qp] * ddgidc * f_i);
      value += -2.0 * (_phi[_j][_qp] * grad_dgidc * f_i + _u[_qp]*dgrad_dgidc * f_i + _phi[_j][_qp]*dgidc * grad_f_i + _u[_qp] * ddgidc * grad_f_i);

      if (_u_old[_qp] < 1.0e-4)
        value *= 0.0;

      return value;
  }

  mooseError("Unsupported type passed in.");
}
