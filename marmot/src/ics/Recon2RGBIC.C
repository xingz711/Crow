#include "Recon2RGBIC.h"
#include "Euler2RGB.h"

template<>
InputParameters validParams<Recon2RGBIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<UserObjectName>("ebsd_reader", "The EBSDReader GeneralUserObject");
  params.addParam<unsigned int>("sd", 3, "An integer representing reference sample direction");
  return params;
}

Recon2RGBIC::Recon2RGBIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _ebsd_reader(getUserObject<EBSDReader>("ebsd_reader")),
    _sd(getParam<unsigned int>("sd"))
{
}

Real
Recon2RGBIC::value(const Point &)
{
  const Point p = _current_elem->centroid();
  const EBSDReader::EBSDPointData & d = _ebsd_reader.getData(p);

  return Euler2RGB(_sd, d.phi1, d.phi, d.phi2, d.phase, d.symmetry);
}
