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

#ifndef MATHUTILSTEST_H
#define MATHUTILSTEST_H

//CPPUnit includes
#include "cppunit/extensions/HelperMacros.h"

class MathUtilsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( MathUtilsTest );

  CPPUNIT_TEST( poly1Log );
  CPPUNIT_TEST( poly2Log );
  CPPUNIT_TEST( poly3Log );
  CPPUNIT_TEST( poly4Log );
  CPPUNIT_TEST( TaylorLog );
  CPPUNIT_TEST( round );

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void poly1Log();
  void poly2Log();
  void poly3Log();
  void poly4Log();
  void TaylorLog();
  void round();

private:
};

#endif //MATHUTILSTEST_H
