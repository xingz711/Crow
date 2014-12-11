[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
[]

[Variables]
  [./Disp_x]
  [../]
  [./Disp_y]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_y = Disp_y
    disp_x = Disp_x
  [../]
[]

[BCs]
  [./fixed_x]
    type = PresetBC
    variable = Disp_x
    boundary = bottom
    value = 0
  [../]
  [./Fixed_y]
    type = PresetBC
    variable = Disp_y
    boundary = bottom
    value = 0
  [../]
  [./Load]
    type = NeumannBC
    variable = Disp_y
    boundary = top
    value = 20
  [../]
[]

[Materials]
  [./mat]
    type = LinearElasticMaterial
    block = 0
    fill_method = symmetric_isotropic
    disp_y = Disp_y
    disp_x = Disp_x
    C_ijkl = '3e6 0.3'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = PJFNK
[]

[Outputs]
  output_initial = true
  exodus = true
  output_final = true
  [./console]
    type = Console
    perf_log = true
    scientific_time = true
    linear_residuals = true
  [../]
[]

