[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 1
  nz = 0
  xmin = 0
  xmax = 5
  ymin = 0
  ymax = 1
  elem_type = QUAD4
[]

[BCs]
  [./all]
    type = DirichletBC
    variable = u
    boundary = 'left right'
    value = 1.0
  [../]
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./udiff]
    type = Diffusion
    variable = u
  [../]
  [./dudt]
    type = TimeDerivative
    variable = u
  [../]
[]

[Postprocessors]
  [./dbox]
    type = OneBubbleGBC
    Ly = 2.0
    Lz = 2.0
    radius_bub = 1.0
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  dt = 0.5
  num_steps = 1
[]

[Outputs]
  output_initial = false
  csv = true
  print_linear_residuals = true
  print_perf_log = true
[]

