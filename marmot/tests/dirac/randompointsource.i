[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmin = 0
  xmax = 100
  ymin = 0
  ymax = 100
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.0
  [../]
[]

[Kernels]
  [./dcdt]
    type = TimeDerivative
    variable = c
  [../]

  [./diffusion]
    type = Diffusion
    variable = c
  [../]
[]

[DiracKernels]
  [./radiation]
    type = RandomPointSource
    num_srcs = 1
    value = 1
    variable = c
    x1 = 0.0
    y1 = 0.0
    x2 = 100.0
    y2 = 100.0
  [../]
[]

[BCs]
  [./dirichlet]
    type = DirichletBC
    variable = c
    boundary = 'left right top bottom'
    value = 0.0
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-10

  start_time = 0.0
  dt = 0.3
  num_steps = 5
[]

[Outputs]
  interval = 1
  exodus = true
  print_perf_log = true
[]
