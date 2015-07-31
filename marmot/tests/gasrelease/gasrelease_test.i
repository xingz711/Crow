#
# Solve the release in a single spherical grain
#

[Mesh]
  dim = 1
  type = GeneratedMesh
  nx = 60
  xmin = 0.0
  xmax = 10.0
[]

[Problem]
  coord_type = 'RSPHERICAL'
  block = '0'
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./m]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./cres_time]
    type = TimeDerivative
    variable = c
  [../]

  [./cres_diff]
    type = MatDiffusion
    variable = c
    prop_name = D
  [../]

  [./cres_production]
    type = FissionGasSource
    variable = c
    a = beta
    b = 1.0
  [../]

  [./cres_trapping]
    type = FissionGasLinear
    variable = c
    a = g
    b = 1.0
  [../]

  [./cres_resolution]
    type = FissionGasCoupledLinear
    variable = c
    v = m
    a = b
    b = -1.0
  [../]

  [./mres_time]
    type = TimeDerivative
    variable = m
  [../]

  [./mres_trapping]
    type = FissionGasCoupledLinear
    variable = m
    v = c
    a = g
    b = -1.0
  [../]

  [./mres_resolution]
    type = FissionGasLinear
    variable = m
    a = b
    b = 1.0
  [../]
[]

[BCs]
  [./surface]
    type = DirichletBC
    variable = c
    value = 0.0
    boundary = right
  [../]
[]

[Materials]
  [./constant]
    type = GenericConstantMaterial
    block = 0

    # beta (at/um^3/s)
    # D (um^2/s)
    # b (1/s)
    # g (1/s)
    prop_names  = 'b    g    D     beta'
    prop_values = '3e-4 3e-4 1e-8  3'
  [../]
[]

[Executioner]
  type = Transient
  perf_log = true

  solve_type = PJFNK

  no_fe_reinit = true

  l_max_its = 70
  l_tol = 1e-6

  nl_max_its = 30
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9

  num_steps = 6

  dt = 0.1
[]

[Outputs]
  file_base = out
  output_initial = true
  console = true
  exodus = true
[]

