[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 5
  nz = 0
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
  elem_type = QUAD4
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = 'left'
    value = 0.1
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = 'right'
    value = 0.9
  [../]
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
  [./v]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./udiff]
    type = Diffusion
    variable = u
  [../]
  [./valgebra]
    type = AlgebraDebug
    variable = v
    v = u
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  output_initial = false
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

