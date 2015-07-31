[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    block = 0
    prop_names  = 'propA propB'
    prop_values = '1     2'
    outputs = exodus
  [../]
  [./copy]
    type = GenericMaterialMaterial
    block = 0
    prop_new_names = 'copyA copyB'
    prop_old_names = 'propA propB'
    outputs = exodus
  [../]
[]

[Problem]
  type = FEProblem
  solve = false
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  num_steps = 1
[]

[Outputs]
  exodus = true
[]

