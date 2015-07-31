[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 8
  ny = 8
  nz = 0
  xmax = 25
  ymax = 25
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 12.5
      y1 = 12.5
      radius = 6.0
      invalue = 1.0
      outvalue = -0.8
      int_width = 4.0
    [../]
  [../]
[]

[Kernels]
  [./ie_c]
    type = TimeDerivative
    variable = c
  [../]
  [./CHSolid]
    type = CHMath
    variable = c
    mob_name = M
  [../]
  [./CHInterface]
    type = CHInterface
    variable = c
    kappa_name = kappa_c
    mob_name = M
    grad_mob_name = grad_M
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
    kappa = 1.0
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 20
  l_tol = 1.0e-5
  nl_max_its = 10
  nl_rel_tol = 1.0e-10
  start_time = 0.0
  num_steps = 3
  dt = 2.0
  [./Adaptivity]
    initial_adaptivity = 1
    error_estimator = LaplacianErrorEstimator
    refine_fraction = .7
    coarsen_fraction = .1
    max_h_level = 1
  [../]
[]

[Outputs]
  file_base = circle
  print_perf_log = true
  [./exodus_oversample]
    type = Exodus
    refinements = 2
    output_initial = true
    file_base = circle_oversample
  [../]
[]
