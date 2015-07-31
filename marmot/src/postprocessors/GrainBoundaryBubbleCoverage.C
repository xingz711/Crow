#include "GrainBoundaryBubbleCoverage.h"

template<>
InputParameters validParams<GrainBoundaryBubbleCoverage>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<Real>("Lx", "Domain x-dimension");
  params.addRequiredParam<Real>("Ly", "Domain y-dimension");
  params.addRequiredParam<Real>("GBwidth", "The GB width");
  params.addRequiredParam<PostprocessorName>("porosity_name", "Name of postprocessor that calculates the porosity");
  params.addRequiredParam<PostprocessorName>("numbub_name", "Name of postprocessor that calculates the number of bubbles");
  params.addRequiredParam<PostprocessorName>("GBarea_name", "Name of postprocessor that calculates the GB area (excluding GB covered by bubble)");

  return params;
}

GrainBoundaryBubbleCoverage::GrainBoundaryBubbleCoverage(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _Lx(getParam<Real>("Lx")),
    _Ly(getParam<Real>("Ly")),
    _GBwidth(getParam<Real>("GBwidth")),
    _porosity(getPostprocessorValue("porosity_name")),
    _numbub(getPostprocessorValue("numbub_name")),
    _GBarea(getPostprocessorValue("GBarea_name"))
{
}

Real
GrainBoundaryBubbleCoverage::getValue()
{
  // Calculate the average bubble diameter
  Real domain_area = _Lx * _Ly;
  Real total_bubble_area = _porosity * domain_area;

  Real average_bubble_area = total_bubble_area / _numbub;
  if (_numbub == 0)
    average_bubble_area = 0.0;

  Real average_bubble_diameter = 2.0 * std::sqrt(average_bubble_area / libMesh::pi);

  // Calculate total GB area covered by bubbles
  Real bub_GB_area = average_bubble_diameter*_numbub*_GBwidth;

  // Calculate GB coverage
  return bub_GB_area / (bub_GB_area + _GBarea);
}
