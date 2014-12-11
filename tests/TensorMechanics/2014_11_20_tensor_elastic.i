# This output simulations the shrinkage of a circular grain imbedded in a larger grain
# Simulation is 2-D
# Mesh adaptivity and time step adaptivity are used
# An AuxVariable is used to calculate the grain boundary locations
# Postprocessors are used to record time step and the area of the shrinking grain

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  elem_type = QUAD4
[]

[Variables]
  [./disp_x]
    block = 0
  [../]
  [./Disp_y]
  [../]
[]

[BCs]
  [./Restraint_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0
  [../]
  [./restraint_y]
    type = DirichletBC
    variable = Disp_y
    boundary = bottom
    value = 0
  [../]
  [./load]
    type = NeumannBC
    variable = Disp_y
    boundary = top
    value = 10
  [../]
[]

[Materials]
  [./mat]
    type = LinearElasticMaterial
    block = 0
    initial_stress = '0 0 0 0 0 0 0 0 0 0'
    fill_method = general_isotropic
    disp_y = Disp_y
    disp_x = disp_x
    C_ijkl = '1e6 0.3 3e4'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = PJFNK
[]

