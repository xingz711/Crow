#include "FeCrCHBulk.h"

/**
 * This is the newly derived FeCrCHBulk.C
 * Key:
 * Term1 = G0*(T/T0)
 * Term2 = H_a
 * Term3 = H_b*TlogT
 * Term4 = H_c*T^2
 * Term5 = 0.5*H_d*T^3
 * Term6 = H_e*T
 * LastTerm = kB*T*(CHIogCHI + (1-CHI)log(1-CHI))
 */

template<>
InputParameters validParams<FeCrCHBulk>()
{
  InputParameters params = validParams<CHBulk>();
  // params.addParam
  params.addRequiredCoupledVar("Temperature", "The temperature the alloy resides at");
  return params;
}

FeCrCHBulk::FeCrCHBulk(const InputParameters & parameters) :
  CHBulk(parameters),
  _grad_T(coupledGradient("Temperature")),
  _T(coupledValue("Temperature"))
{
}

RealGradient
FeCrCHBulk::computeGradDFDCons(PFFunctionType type)
{
  Real chi = _u[_qp]; // this is the concentration variable: [Cr] (Wt. percentage) in alloy
  Real T = _T[_qp]; // T is the second variable
  Real t = 2 * chi - 1;
  Real p = chi - 1;

  const Real kB = 8.6173324e-5; // Boltzmann constant
  const Real T0 = 410.0; // Switching temperature-->Schwen used this in Mathematica file

  //
  // values of constants taken from Schwen2013 (f,a-->d)
  //

  // first to last element represents f0-->f7 obtained from Schwen2013
  const Real f[] = { 0.3817, -0.1007, -0.0485, -0.1541, -0.1684, -0.0416, -3.8602, -4.1671};

  // first to last element represents a0-->a7 obtained from Schwen2013
  const Real a[] ={0.3856, -0.0973, -0.0467, -0.1945, -0.1856, 0.0044, -3.8366, -4.1231};
  const Real b[] = {0.0003, 4.6956e-5, -4.9588e-5, 1.1333e-5};
  const Real c[] = {3.1446e-9, -2.2031e-8, 3.9597e-8, -3.0902e-8};
  const Real d[] = {-1.7601e-13, 5.5788e-12, -5.9353e-12, 1.2285e-11};

  Real d2g0_dCHI2 = - 2.0 * f[0] + 12*f[1]*chi - 6*f[1] - 48*f[2]*std::pow(chi, 2) + 48*f[2]*chi - 10*f[2] + 160*f[3]*std::pow(chi, 3) - 240*f[3]*std::pow(chi, 2)
                    + 108.0 * f[3]*chi - 14*f[3] - 480*f[4]*std::pow(chi, 4) + 960*f[4]*std::pow(chi, 3) - 672*f[4]*std::pow(chi, 2) + 192*f[4]*chi - 18*f[4]
                    + 1344.0 * f[5]*std::pow(chi, 5) - 3360*f[5]*std::pow(chi, 4) + 3200*f[5]*std::pow(chi, 3) - 1440*f[5]*std::pow(chi, 2) + 300*f[5]*chi
                    - 22.0 * f[5] - 6*f[6]*chi + 2*f[6] + 6*f[7]*chi - 4*f[7];

  Real d2Ha_dCHI2 = - 2*a[0] + 12*a[1]*chi - 6*a[1] - 48*a[2]*std::pow(chi, 2) + 48*a[2]*chi - 10*a[2] + 160*a[3]*std::pow(chi, 3) - 240*a[3]*std::pow(chi, 2)
                    + 108*a[3]*chi - 14*a[3] - 480*a[4]*std::pow(chi, 4) + 960*a[4]*std::pow(chi, 3) - 672*a[4]*std::pow(chi, 2) + 192*a[4]*chi - 18*a[4]
                    + 1344*a[5]*std::pow(chi, 5) - 3360*a[5]*std::pow(chi, 4) + 3200*a[5]*std::pow(chi, 3) - 1440*a[5]*std::pow(chi, 2) + 300*a[5]*chi
                    - 22*a[5] - 6*a[6]*chi + 2*a[6] + 6*a[7]*chi - 4*a[7];

  Real d2Hb_dCHI2 = -6*p*b[3] + 2*b[2]; // Ok
  Real d2Hc_dCHI2 = -6*p*c[3] + 2*c[2]; // Ok
  Real d2Hd_dCHI2 = -6*p*d[3] + 2*d[2]; // Ok
  Real d2He_dCHI2 = (-1.0/T0) * d2Ha_dCHI2 + (T0 * d2Hc_dCHI2) + d2Hb_dCHI2 * std::log(T0) + d2Hd_dCHI2 * 0.5 * T0*T0; //

  Real SecondDiff_last_term = -kB * T/(chi*p);

  Real d2G_dCHI2 = d2g0_dCHI2 * (T/T0) + d2Ha_dCHI2 - d2Hb_dCHI2 * T * std::log(T) - d2Hc_dCHI2 * T*T + d2Hd_dCHI2 * 0.5 * T*T*T + d2He_dCHI2 * T + kB * T * SecondDiff_last_term;
  RealGradient First_term = d2G_dCHI2 * _grad_u[_qp]; // The full form of the first term in the residual

  //
  // Second term
  //

  Real d2_dCHIdTTerm1 =   (2*chi*p*(5*std::pow(t, 4)*f[5] - 4*std::pow(t, 3)*f[4] + 3*std::pow(t, 2)*f[3] - 2*t*f[2] + f[1])
                        + 2*(std::pow((chi*1 - chi), 5)*f[5] - std::pow(t, 4)*f[4] + std::pow(t, 3)*f[3] - std::pow(t, 2)*f[2] + t*f[1] - f[0])
                        + p*(std::pow(t, 5)*f[5] - std::pow(t, 4)*f[4]  + std::pow(t, 3)*f[3] - std::pow(t, 2)*f[2] + t*f[1] - f[0]) + f[6] - f[7])/T0;
  Real d2_dCHIdTTerm2 = 0.0; // Ok
  Real d2_dCHIdTTerm3 = -b[1] + b[2]*(2*chi - 2) - 3*b[3]*std::pow((-chi + 1), 2) + (-b[1] + b[2]*(2*chi - 2) - 3*b[3]*std::pow((-chi + 1), 2))*std::log(T);
  Real d2_dCHIdTTerm4 = 2*T*(-c[1] + c[2]*(2*chi - 2) - 3*c[3]*std::pow((-chi + 1), 2));
  Real d2_dCHIdTTerm5 = 3*std::pow(T, 2)*(-0.5*d[1] + 0.5*d[2]*(2*chi - 2) - 1.5*d[3]*std::pow((-chi + 1), 2));
  Real d2_dCHIdTTerm6 =  (T0*(std::pow(T0, 2)*(-0.5*d[1] + d[2]*p - 1.5*d[3]*std::pow(p, 2)) - T0*(c[1] - 2*c[2]*p + 3*c[3]*std::pow(p, 2))
                        - (b[1] - 2*b[2]*p + 3*b[3]*std::pow(p, 2))*std::log(T0)) - chi*p*(2*a[1] - 4*a[2]*t + 6*a[3]*std::pow(t, 2) - 8*a[4]*std::pow(t, 3)
                        + 10*a[5]*std::pow(t, 4) - a[6] + a[7]) + chi*(a[0] - a[1]*t + a[2]*std::pow(t, 2) - a[3]*std::pow(t, 3) + a[4]*std::pow(t, 4)
                        - a[5]*std::pow(t, 5) + a[6]*chi - a[7]*p) + p*(a[0] - a[1]*t + a[2]*std::pow(t, 2) - a[3]*std::pow(t, 3) + a[4]*std::pow(t, 4)
                        - a[5]*std::pow(t, 5) + a[6]*chi - a[7]*p))/T0;

  Real d2_dCHIdTLastTerm = kB * (std::log(chi) - std::log(-chi + 1));

  Real d2G_dCHIdT= d2_dCHIdTTerm1 + d2_dCHIdTTerm2 - d2_dCHIdTTerm3 - d2_dCHIdTTerm4 + d2_dCHIdTTerm5 + d2_dCHIdTTerm6 + d2_dCHIdTLastTerm; // Need to change this as well

  RealGradient Second_term = d2G_dCHIdT*_grad_T[_qp]; // Second term in the resdidual

  RealGradient grad_dSchwen_dCHI = First_term + Second_term; // Add the first and second term in the residual

  switch (type)
  {
    case Residual: // value u is the concentration field and w is the temperature field
      return grad_dSchwen_dCHI; // This is nabla dflocal_dc

    case Jacobian:
    {
      // As stated the non-isothermal project plan
      // Need to convert into C++syntachi
      Real d3g0_dCHI3 = 6720.0 * std::pow(chi, 4) * f[5] - 1920 * std::pow(chi, 3) * f[4] - 13440*std::pow(chi, 3)*f[5] + 480*std::pow(chi, 2)*f[3] + 2880*std::pow(chi, 2)*f[4]
        + 9600*std::pow(chi, 2)*f[5] - 96*chi*f[2] - 480*chi*f[3] - 1344*chi*f[4] - 2880*chi*f[5] + 12*f[1] + 48*f[2] + 108*f[3] + 192*f[4] + 300*f[5];
      //
      Real d3Ha_dCHI3 = 6720.0 * std::pow(chi, 4)*a[5] - 1920*std::pow(chi, 3)*a[4] - 13440*std::pow(chi, 3)*a[5] + 480*std::pow(chi, 2)*a[3] + 2880*std::pow(chi, 2)*a[4]
        + 9600*std::pow(chi, 2)*a[5] - 96*chi*a[2] - 480*chi*a[3] - 1344*chi*a[4] - 2880*chi*a[5] + 12*a[1] + 48*a[2] + 108*a[3] + 192*a[4] + 300*a[5];

      Real d3Hb_dCHI3 = -6*b[3];
      Real d3Hc_dCHI3 = -6*c[3];
      Real d3Hd_dCHI3 = -6*d[3];
      Real d3He_dCHI3 = (-3.0*std::pow(T0, 3)*d[3] - 6.0*std::pow(T0, 2)*c[3] - 6.0*T0*b[3]*log(T0) - 6720.0*std::pow(chi, 4)*a[5] + 1920.0*std::pow(chi, 3)*a[4]
                       + 13440.0*std::pow(chi, 3)*a[5] - 480.0*std::pow(chi, 2)*a[3] - 2880.0*std::pow(chi, 2)*a[4] - 9600.0*std::pow(chi, 2)*a[5] + 96.0*chi*a[2]
                       + 480.0*chi*a[3] + 1344.0*chi*a[4] + 2880.0*chi*a[5] - 12.0*a[1] - 48.0*a[2] - 108.0*a[3] - 192.0*a[4] - 300.0*a[5])/T0;
      Real d3lastTerm_dCHI3= (kB*T/std::pow(p, 2)) - (kB*T/std::pow(chi, 2));
      Real d3G_dCHI3=d3g0_dCHI3*(T/T0) + d3Ha_dCHI3 - d3Hb_dCHI3*T*std::log(T) - d3Hc_dCHI3*T*T + d3Hd_dCHI3*0.5*T*T*T + d3He_dCHI3*T   +   d3lastTerm_dCHI3;

      Real d3FirstTerm_dCHIdCHIdT=   2*(672*std::pow(chi, 5)*f[5] - 240*std::pow(chi, 4)*f[4] - 1680*std::pow(chi, 4)*f[5] + 80*std::pow(chi, 3)*f[3]
            + 480*std::pow(chi, 3)*f[4] + 1600*std::pow(chi, 3)*f[5] - 24*std::pow(chi, 2)*f[2] - 120*std::pow(chi, 2)*f[3]
            - 336*std::pow(chi, 2)*f[4] - 720*std::pow(chi, 2)*f[5] + 6*chi*f[1] + 24*chi*f[2] + 54*chi*f[3] + 96*chi*f[4] + 150*chi*f[5]
            - f[0] - 3*f[1] - 5*f[2] - 7*f[3] - 9*f[4] - 11*f[5])/T0;

      Real d3SecondTerm_dCHIdCHIdT = 0.0;
      Real d3ThirdTerm_dCHIdCHIdT = -6.0 * p * b[3] - 2*(3*p*b[3] - b[2])*log(T) + 2*b[2];
      Real d3FourthTerm_dCHIdCHIdT = -4.0 * T * (3.0 * p * c[3] - c[2]);
      Real d3FifthTerm_dCHIdCHIdT = std::pow(T, 2)*(-9.0*p*d[3] + 3.0*d[2]);
      Real d3SichithTerm_dCHIdCHIdT = (-3.0*std::pow(T0, 3)*chi*d[3] + 1.0*std::pow(T0, 3)*d[2] + 3.0*std::pow(T0, 3)*d[3] - 6.0*std::pow(T0, 2)*chi*c[3]
          + 2.0*std::pow(T0, 2)*c[2] + 6.0*std::pow(T0, 2)*c[3] - 6.0*T0*chi*b[3]*log(T0) + 2.0*T0*b[2]*log(T0) + 6.0*T0*b[3]*log(T0)
          - 1344.0*std::pow(chi, 5)*a[5] + 480.0*std::pow(chi, 4)*a[4] + 3360.0*std::pow(chi, 4)*a[5] - 160.0*std::pow(chi, 3)*a[3]
          - 960.0*std::pow(chi, 3)*a[4] - 3200.0*std::pow(chi, 3)*a[5] + 48.0*std::pow(chi,  2)*a[2] + 240.0*std::pow(chi, 2)*a[3]
          + 672.0*std::pow(chi, 2)*a[4] + 1440.0*std::pow(chi, 2)*a[5] - 12.0*chi*a[1] - 48.0*chi*a[2] - 108.0*chi*a[3] - 192.0*chi*a[4]
          - 300.0*chi*a[5] + 2.0*a[0] + 6.0*a[1] + 10.0*a[2] + 14.0*a[3] + 18.0*a[4] + 22.0*a[5])/T0;

      // Real d3LastTerm_dCHIdCHIdT = -K_B/(chi*p);
      Real d3G_dChidCHIdT = d3FirstTerm_dCHIdCHIdT + d3SecondTerm_dCHIdCHIdT + d3ThirdTerm_dCHIdCHIdT + d3FourthTerm_dCHIdCHIdT + d3FifthTerm_dCHIdCHIdT + d3SichithTerm_dCHIdCHIdT;

      // Now form the Jacobian (moose compatible i.e. without constants e.t.c.)
      RealGradient Jacobian_bulk = d3G_dCHI3 * _phi[_j][_qp] * _grad_u[_qp] + d2G_dCHI2 * _grad_phi[_j][_qp] + d3G_dChidCHIdT * _phi[_j][_qp] * _grad_T[_qp];

      return Jacobian_bulk; // This is the Jacobian
    }
  }

  mooseError("Invalid type passed in");
}
