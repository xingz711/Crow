[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  xmax = 30.0
  ymax = 30.0
  elem_type = QUAD4
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
[]

[ICs]
  [./InitialCondition]
    type = CrossIC
    x1 = 0.0
    x2 = 30.0
    y1 = 0.0
    y2 = 30.0
    variable = c
  [../]
[]

[Kernels]
  [./cdot]
    type = TimeDerivative
    variable = c
  [../]
  [./BulkFreeEnergy]
    type = CHMath
    variable = c
  [../]
  [./InterfaceEnergy]
    type = CHInterfaceSplit
    variable = c
    w = w
    kappa_name = kappa_c
  [../]
  [./w_res]
    type = CHSplitVar
    variable = w
    c = c
  [../]
  [./w_term]
    type = Reaction
    variable = w
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./constant]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 2.0
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'NEWTON'
  petsc_options_iname = -pc_type
  petsc_options_value = lu

  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-9

  dt = 10.0
  num_steps = 3
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
