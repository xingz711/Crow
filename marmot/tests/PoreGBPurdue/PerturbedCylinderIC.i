[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 5
  ny = 5
  nz = 10
  xmax = 200
  ymax = 200
  zmax = 400
  elem_type = HEX8
  uniform_refine = 2
[]

[GlobalParams]
  int_width = 20
  radius = 40
  amplitude = 10
  ncycl = 1
  x1 = 100
  y1 = 100
  cyl_bottom = 50
  cyl_top = 350
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PerturbedCylinderIC
      var_type = conserved
      variable = c
    [../]
  [../]
  [./w]
  [../]
  [./gr0]
    [./InitialCondition]
      type = PerturbedCylinderIC
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
