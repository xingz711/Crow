[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 15
  xmax = 250
  elem_type = EDGE
[]

[GlobalParams]
  polynomial_order = 4
  length_scale = 1.0e-9
  time_scale = 1.0e-9
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
[]

[ICs]
  [./InitialCondition]
    type = BoundingBoxIC
    x1 = 0.0
    x2 = 50.0
    y1 = 0
    y2 = 0
    inside = 1.0
    outside = 0.1
    variable = c
  [../]
[]

[Kernels]
  [./ie_c]
    type = TimeDerivative
    variable = c
  [../]
  [./CHSolid]
    type = CHParsed
    variable = c
    mob_name = M
    f_name = F
  [../]
  [./CHInterface]
    type = CHInterfaceSplit
    variable = c
    mob_name = M
    kappa_name = kappa
    w = w
  [../]
  [./w]
    type = Reaction
    variable = w
  [../]
  [./w_def]
    type = CHSplitVar
    variable = w
    c = c
  [../]
[]

[Materials]
  [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = 1000 # K
    int_width = 40.0
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 3
    T = 1000
  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
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
  solve_type = NEWTON
  petsc_options_iname = -pc_type
  petsc_options_value = lu
  l_max_its = 20
  l_tol = 1.0e-4
  nl_max_its = 30
  nl_rel_tol = 1.0e-9
  nl_abs_tol = 1.0e-11
  start_time = 0.0
  num_steps = 35
  dt = 40
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.75
    dt = 40
    growth_factor = 1.25
    optimal_iterations = 6
  [../]
[]

[Outputs]
  file_base = cu
  output_initial = true
  exodus = true
  print_perf_log = true
[]
