// /****************************************************************/
// /*               DO NOT MODIFY THIS HEADER                      */
// /* MOOSE - Multiphysics Object Oriented Simulation Environment  */
// /*                                                              */
// /*           (c) 2010 Battelle Energy Alliance, LLC             */
// /*                   ALL RIGHTS RESERVED                        */
// /*                                                              */
// /*          Prepared by Battelle Energy Alliance, LLC           */
// /*            Under Contract No. DE-AC07-05ID14517              */
// /*            With the U. S. Department of Energy               */
// /*                                                              */
// /*            See COPYRIGHT for full restrictions               */
// /****************************************************************/
//
// #ifndef ConservationCheck_H
// #define ConservationCheck_H
//
// #include "ElementIntegralVariablePostprocessor.h"
// #include "VectorPostprocessorInterface.h"
//
// //Forward Declarations
// class ConservationCheck;
// // class FeatureVolumeVectorPostprocessor;
// class GrainTrackerInterface;
// class GrainForceAndTorqueInterface;
//
// template<>
// InputParameters validParams<ConservationCheck>();
//
// /**
//  * This postprocessor computes a volume integral of the specified variable.
//  *
//  * Note that specializations of this integral are possible by deriving from this
//  * class and overriding computeQpIntegral().
//  */
// class ConservationCheck :
//   public ElementIntegralVariablePostprocessor,
//   public VectorPostprocessorInterface
// {
// public:
//   ConservationCheck(const InputParameters & parameters);
//
// protected:
//   virtual Real computeQpIntegral() override;
//
//   /// getting userobject for calculating grain centers and volumes
//   const GrainTrackerInterface & _grain_tracker;
//
//   /// The grain volumes
//   const VectorPostprocessorValue & _grain_volumes;
//
//   /// getting userobject for calculating grain forces and torques
//   const GrainForceAndTorqueInterface & _grain_force_torque;
//   const std::vector<RealGradient> & _grain_forces;
//   const std::vector<RealGradient> & _grain_torques;
//
// private:
//   /// constant value corresponding to grain translation
//   const Real _mt;
//
//   /// constant value corresponding to grain rotation
//   const Real _mr;
//
//   const unsigned int _op_num;
//
//   /// holds the order parameters
//   std::vector<const VariableValue *> _vals;
//
//   RealGradient _velocity_advection;
//   std::vector<unsigned int> _grain_ids;
// };
//
// #endif
