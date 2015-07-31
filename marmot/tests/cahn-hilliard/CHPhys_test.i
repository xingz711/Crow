[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 4
  ny = 4
  nz = 0
  xmax = 25
  ymax = 25
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 1
[]

[Variables]
  [./cv]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 12.5
      y1 = 12.5
      radius = 6.0
      invalue = 1.0
      outvalue = 0.1
      int_width = 4.0
    [../]
  [../]
[]

[Kernels]
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
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./GenIrrad]
    type = CTempIrrad
    block = 0
    cv = cv
    T = 1150 # K
    int_width = 60 # nm
    free_energy_form = 0
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 15
  l_tol = 1.0e-4
  nl_max_its = 40
  nl_rel_tol = 1e-10
  start_time = 0.0
  num_steps = 2
  dt = 1.0
  [./Adaptivity]
    initial_adaptivity = 1
    error_estimator = LaplacianErrorEstimator
    refine_fraction = 0.8
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  # [./OverSampling]
  # exodus = true
  # refinements = 3
  # output_initial = true
  # [../]
  file_base = chphys
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]

