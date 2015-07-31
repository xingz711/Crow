[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 10
  ny = 2
  nz = 10
  xmax = 250
  ymax = 50
  zmax = 250
  elem_type = HEX8
  uniform_refine = 2
[]

[GlobalParams]
  int_width = 20
  amplitude = 50
  sub_height = 125
  ncycl = 1
  dom_dim = 3D
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SinusoidalSurfaceIC
      var_type = conserved
      variable = c
    [../]
  [../]
  [./w]
  [../]
  [./gr0]
    [./InitialCondition]
      type = SinusoidalSurfaceIC
      var_type = nonconserved
    [../]
  [../]
[]

[Executioner]
  type = Steady
[]

[Problem]
  solve = false
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]
