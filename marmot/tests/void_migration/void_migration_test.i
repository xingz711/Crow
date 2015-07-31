[Mesh]
  file = rect_midplane_Q4_coarse.e
  uniform_refine = 1
[]

[GlobalParams]
  bulkindex = 1.0
  diffindex = 0.0
  polynomial_order = 8
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0e3
  [../]
[]

[ICs]
  [./c_IC]
    type = SmoothCircleIC
    x1 = 500.0
    y1 = 350
    radius = 200
    invalue = 1.0
    outvalue = 0.01
    int_width = 200.0
    variable = c
  [../]
[]

[AuxVariables]
  [./T]
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    type = SplitCHWResGBSurfDiff
    variable = w
    c = c
    mob_name = M
  [../]
  [./w_res_Tgrad]
    type = SplitCHWResTGrad
    variable = w
    c = c
    T = T
    diff_name = D
    Q_name = Qstar
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
[]

[AuxKernels]
  [./Temp]
    type = FunctionAux
    variable = T
    function = 1000.0+0.025*y
  [../]
[]

[Materials]
  [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 1
    c = c
    T = T # K
    int_width = 100.0
    length_scale = 1.0e-9
    time_scale = 1.0e-6
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 1
    c = c
    outputs = exodus
    derivative_order = 2
    T = T
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Postprocessors]
  [./void_pos]
    type = CInterfacePosition
    variable = c
    boundary = 5
    direction_index = 1
    RefVal = 0.95
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 10
  l_tol = 1.0e-4
  nl_max_its = 25
  nl_rel_tol = 1.0e-9
  start_time = 0.0
  num_steps = 4
  dt = 3
  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = out
  output_initial = true
  interval = 4
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
