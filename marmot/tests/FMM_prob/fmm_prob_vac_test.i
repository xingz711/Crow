[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  nz = 0
  xmin = 0
  xmax = 1.0
  ymin = 0
  ymax = 1.0
  elem_type = QUAD4

  uniform_refine = 3
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
      outvalue = 0.05
      int_width = 0.2
    [../]
  [../]
[]

[AuxVariables]
  [./cg]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.0
  [../]
[]

[Kernels]
  active = 'ie_c CHSolid CHInterface'

  [./ie_c]
    type = TimeDerivative
    variable = cv
  [../]

  [./CHSolid]
    type = CHPhys
    variable = cv
    mob_name = Ms_v
  [../]

  [./CHInterface]
    type = CHInterface
    variable = cv
    mob_name = M_v
    grad_mob_name = grad_M_v
    kappa_name = kappa_v
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
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20
  l_tol = 1.0e-4

  nl_max_its = 10
  nl_rel_tol = 1.0e-11

  start_time = 0.0
  num_steps = 2
  dt = 0.2

  [./Adaptivity]
    initial_adaptivity = 2
    error_estimator = LaplacianErrorEstimator
    refine_fraction = 0.8
   coarsen_fraction = 0.05
    max_h_level = 3
  [../]
[]

[Outputs]
  file_base = fmm_vac
  output_initial = true
  interval = 1
  exodus = false
  print_perf_log = true
  [./OverSampling]
    type = Exodus
    refinements = 1
    output_initial = true
    file_base = fmm_vac_oversample
  [../]
[]
