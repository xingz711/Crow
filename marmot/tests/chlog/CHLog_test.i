[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 7
  ny = 7
  nz = 0
  xmin = 0
  xmax = 25
  ymin = 0
  ymax = 25
  zmin = 0
  zmax = 25
  elem_type = QUAD4

  uniform_refine = 1
[]

[Variables]
  active = 'cv'

  [./cv]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 12.5
      y1 = 12.5
      radius = 4.0
      invalue = 1.0
      outvalue = 0.1
      int_width = 3.0
    [../]
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
      translation = '0 25 0'
    [../]

    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-25 0 0'
    [../]
  [../]
[]

[Materials]

  [./GenIrrad]
    type = CTempIrrad
    block = 0
    cv = cv
    T = 1150 #K
    int_width = 20.0 #nm
    free_energy_form = 0
    D0vac = 2.21e4
    Emvac = 1.0
    Efvac = 1.0
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -mat_mffd_type'
  petsc_options_value = 'hypre boomeramg 101 ds'

  l_max_its = 10
  l_tol = 1e-3

  nl_max_its = 10
  nl_rel_tol = 1e-11

  start_time = 0.0
  num_steps = 2
  dt = 0.1

  [./Adaptivity]
    initial_adaptivity = 2
    error_estimator = LaplacianErrorEstimator
    refine_fraction = 0.7
   coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = out
  output_initial = true
  interval = 1
  print_perf_log = true
  [./OverSampling]
    type = Exodus
    refinements = 2
    output_initial = true
    interval = 1
    file_base = out_oversample
  [../]
[]
