#ifndef RECONMATERIAL_H
#define RECONMATERIAL_H

#include "Material.h"
#include "EBSDReader.h"
#include "GrainTracker.h"

// Forward Declarationsy
class ReconMaterial;
class GrainTracker;

template<>
InputParameters validParams<ReconMaterial>();

class ReconMaterial : public Material
{
public:
  ReconMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties();
  virtual void computeProperties();

 private:
  const EBSDReader & _ebsd_reader;
  const GrainTracker & _grain_tracker;

  unsigned int _op_num;
  unsigned int _grain_num;

  std::vector<VariableValue *> _vals;

  MaterialProperty<unsigned int> & _grn;
  MaterialProperty<unsigned int> & _grn_old;
  MaterialProperty<unsigned int> & _op;
  MaterialProperty<unsigned int> & _op_old;
};

#endif //RECONMATERIAL_H
