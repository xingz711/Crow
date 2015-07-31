[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 15
  ny = 15
  xmax = 1.0
  ymax = 1.0
  elem_type = QUAD4
[]

[Variables]
  [./cv]
    order = THIRD
    family = HERMITE
  [../]
  [./cg]
    order = THIRD
    family = HERMITE
  [../]
[]

[ICs]
  [./cv_IC]
    type = SmoothCircleIC
    variable = cv
    x1 = 0.5
    y1 = 0.5
    radius = 0.15
    invalue = 1.0
    outvalue = 0.005
    int_width = 0.12
  [../]
  [./cg_IC]
    type = SmoothCircleIC
    x1 = 0.5
    y1 = 0.5
    radius = 0.15
    invalue = 0.00
    outvalue = 0.0001
    int_width = 0.12
    variable = cg
  [../]
[]

[Kernels]
  [./ie_cv]
    type = TimeDerivative
    variable = cv
  [../]
  [./ch_bulk_v]
    type = CHVacwGas
    variable = cv
    cg = cg
    mob_name = Ms_v
  [../]
  [./ch_interf_v]
    type = CHInterface
    variable = cv
    mob_name = M_v
    kappa_name = kappa_v
    args = cg
  [../]
  [./ie_cg]
    type = TimeDerivative
    variable = cg
  [../]
  [./ch_bulk_g]
    type = CHGaswVac
    variable = cg
    cv = cv
    mob_name = Ms_g
  [../]
  [./ch_interf_g]
    type = CHInterface
    variable = cg
    mob_name = M_g
    kappa_name = kappa_g
    args = cv
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
  [./vacgas]
    type = UO2VacGas
    block = 0
    cv = cv
    cg = cg
    temp = 2000 # K
    int_width = 0.01
    length_scale = 1.0e-8
    time_scale = 1.0e-6
    Efgas = 3.0
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'

  l_max_its = 20
  l_tol = 1.0e-4
  nl_max_its = 20
  nl_rel_tol = 1.0e-11
  nl_abs_tol = 1.0e-10

  start_time = 0.0
  num_steps = 2
  dt = 0.05
[]

[Outputs]
  output_initial = true
  interval = 2
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
