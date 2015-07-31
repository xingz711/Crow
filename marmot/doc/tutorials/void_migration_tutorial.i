[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 5
  ny = 5
  nz = 0
  xmin = 0
  xmax = 1.0
  ymin = 0
  ymax = 1.0
  elem_type = QUAD4

  uniform_refine = 1
[]

[Variables]

  [./cv]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 0.5
      y1 = 0.5
      radius = 0.1
      invalue = 1.0
      outvalue = 0.005
      int_width = 0.12
    [../]
  [../]

  [./cg]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 0.5
      y1 = 0.5
      radius = 0.1
      invalue = 0.00
      outvalue = 0.001
      int_width = 0.12
    [../]
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
    grad_mob_name = grad_M_v
    kappa_name = kappa_v
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
    grad_mob_name = grad_M_g
    kappa_name = kappa_g
  [../]
[]

[BCs]
active = 'Periodic'
  [./Periodic]
    [./left_right]
      primary = 0
      secondary = 2
      translation = '0 1.0 0'
    [../]

    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-1.0 0 0'
    [../]
  [../]
[]

[Materials]

  [./vacgas]
    type = UO2VacGas
    block = 0
    cv = cv
    cg = cg
    temp = 2000 #K
    int_width = 0.01
    length_scale = 1.0e-8
    time_scale = 1.0e-6
    Efgas = 3.0
  [../]
[]

[Preconditioning]
 active = ' '

  [./SMP]
   type = SMP
   off_diag_row = 'cv cg'
   off_diag_column = 'cg cv'
  [../]

  [./FDP]
   type = FDP
   full = true
  [../]

[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options = '-pc_factor_shift_nonzero'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20
  l_tol = 1.0e-4

  nl_max_its = 20
  nl_rel_tol = 1.0e-11
  nl_abs_tol = 1.0e-10

  start_time = 0.0
  num_steps = 2
  dt = 0.1

  [./Adaptivity]
    initial_adaptivity = 1
    error_estimator = LaplacianErrorEstimator
    refine_fraction = 0.8
   coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  output_linear = true
  output_initial = true
  interval = 2
  perf_log = true
  file_base = void_output

  [./OverSampling]
    exodus = true
    interval = 2
    refinements = 3
    output_initial = true
  [../]
[]


