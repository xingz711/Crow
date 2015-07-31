/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "MathUtilsTest.h"

//Moose includes
#include "MathUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MathUtilsTest );

void
MathUtilsTest::setUp()
{
}

void
MathUtilsTest::tearDown()
{
}

void
MathUtilsTest::poly1Log()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, tol;
    int deriv;
    Real expect;
  } samples[nsamples] =
  {
    { 0.2, 0.001, 0, -1.60944 },
    { 0.2, 0.001, 1, 5.0 },
    { 0.2, 0.001, 2, -25.0 },
    { 0.2, 0.001, 3, 250.0 },
    { 0.001, 0.002, 0, -6.71461 },
    { 0.001, 0.002, 1, 500.0 },
    { 0.001, 0.002, 2, 0.0 },
    { 0.001, 0.002, 3, 0.0 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::poly1Log(samples[i].x, samples[i].tol, samples[i].deriv), samples[i].expect, 0.00001);
}

void
MathUtilsTest::poly2Log()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, tol;
    int deriv;
    Real expect;
  } samples[nsamples] =
  {
    { 0.2, 0.001, 0, -1.60944 },
    { 0.2, 0.001, 1, 5.0 },
    { 0.2, 0.001, 2, -25.0 },
    { 0.2, 0.001, 3, 250.0 },
    { 0.001, 0.002, 0, -6.83961 },
    { 0.001, 0.002, 1, 750.0 },
    { 0.001, 0.002, 2, -250000 },
    { 0.001, 0.002, 3, 0.0 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::poly2Log(samples[i].x, samples[i].tol, samples[i].deriv), samples[i].expect, 0.00001);
}

void
MathUtilsTest::poly3Log()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, tol;
    int deriv;
    Real expect;
  } samples[nsamples] =
  {
    { 0.2, 0.001, 0, -1.60944 },
    { 0.2, 0.001, 1, 5.0 },
    { 0.2, 0.001, 2, -25.0 },
    { 0.2, 0.001, 3, 250.0 },
    { 0.001, 0.002, 0, -6.88127 },
    { 0.001, 0.002, 1, 875.0 },
    { 0.001, 0.002, 2, -500000 },
    { 0.001, 0.002, 3, 2.5e+08 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::poly3Log(samples[i].x, samples[i].tol, samples[i].deriv), samples[i].expect, 0.00001);
}

void
MathUtilsTest::poly4Log()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, tol;
    int deriv;
    Real expect;
  } samples[nsamples] =
  {
    { 0.2, 0.001, 0, -1.60944 },
    { 0.2, 0.001, 1, 5.0 },
    { 0.2, 0.001, 2, -25.0 },
    { 0.2, 0.001, 3, 250.0 },
    { 0.001, 0.002, 0, -6.90575 },
    { 0.001, 0.002, 1, 984.375 },
    { 0.001, 0.002, 2, -890625 },
    { 0.001, 0.002, 3, 1.3125e+09 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::poly4Log(samples[i].x, samples[i].tol, samples[i].deriv), samples[i].expect, 0.00001);
}

void
MathUtilsTest::TaylorLog()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, expect;
  } samples[nsamples] =
  {
    { 0.0, -2.5119769 },
    { 0.01, -2.4384881 },
    { 0.1, -1.9049717 },
    { 1.0, 0.0 },
    { 2.5, 0.92910611 },
    { 17.9, 2.8807267 },
    { 123.1, 3.6048257 },
    { 32846.7, 3.7558348 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::TaylorLog(samples[i].x), samples[i].expect, 0.0000001);
}

void
MathUtilsTest::round()
{
  const unsigned int nsamples = 8;

  struct Samples {
    Real x, expect;
  } samples[nsamples] =
  {
    { 0.2, 0 },
    { 0.5, 1 },
    { 0.7, 1 },
    { 1.2, 1 },
    { 10.7, 11 },
    { -0.1, -0 },
    { -0.7, -1 },
    { -10.5, -11 }
  };

  for (unsigned int i = 0; i < nsamples; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL( MathUtils::round(samples[i].x), samples[i].expect, 0.000001);
  // Moose::out << samples[i].x << ' ' << MathUtils::round(samples[i].x) << "\n";
}
