[GlobalParams]
  var_name_base = gr
  op_num = 2.0
  block = 0
  #en_ratio = 1
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 80
  ny = 40
  nz = 40
  xmax = 400.0
  ymax = 200.0
  zmax = 200.0
  elem_type = HEX8
[]

[Variables]
  [./c]
    #scaling = 10
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kappa_c]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./kappa_op]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./M]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./L]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./A]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./B]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1'
  [../]
  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = D
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./PolycrystalSinteringKernel]
    c = c
    consider_rigidbodymotion = false
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1'
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op'
    interfacial_vars = 'c  gr0 gr1'
  [../]
  [./kappa_c]
    type = MaterialRealAux
    variable = kappa_c
    property = kappa_c
  [../]
  [./kappa_op]
    type = MaterialRealAux
    variable = kappa_op
    property = kappa_op
  [../]
  [./M]
    type = MaterialRealAux
    variable = M
    property = D
  [../]
  [./L]
    type = MaterialRealAux
    variable = L
    property = L
  [../]
  [./A]
    type = MaterialRealAux
    variable = A
    property = A
  [../]
  [./B]
    type = MaterialRealAux
    variable = B
    property = B
  [../]
[]

[Materials]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    #A = A
    #B = B
    #f_name = S
    derivative_order = 2
    #outputs = console
  [../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = '  A         B       kappa_op    kappa_c'
    prop_values = '19.9728   2.1221   636.6340   870.6906'
    #prop_names = '  A    B  '
    #prop_values = '16.0 1.0 '
  [../]
  [./mob]
    type = SinteringDiffusion
    block = '0'
    T = 1200.0
    int_width = 20
    #Em = 2.0
    GB_energy = 6.86
    surface_energy = 9.33
    GBmob0 = 3.986e-6
    #Q = 1.0
    Qv = 2.0
    Qgb = 4.143
    Qs = 3.14
    Qgbm = 0.94
    Dgb0 = 4.0e-4
    Dsurf0 = 1.41e-5
    #Ds0 = 1.41e-5
    Dvap0 = 4.0e-6
    Dvol0 = 4.0e-6
    #D0 = 4.0e-6
    c = c
    v = 'gr0 gr1'
    Vm = 1.5829e-29
    #omega = 1.58e-29
    #bulkindex = 1.0
    #surfindex = 1.0
    #gbindex = 1.0
    length_scale = 1e-09
    time_scale = 1e-4
    outputs = exodus
  [../]
[]

[Postprocessors]
  #[./mat_D]
  #  type = ElementIntegralMaterialProperty
  #  mat_prop = M_c
  #[../]
  [./elem_c]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
  [./elem_bnds]
    type = ElementIntegralVariablePostprocessor
    variable = bnds
  [../]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
  [./free_en]
    type = ElementIntegralMaterialProperty
    mat_prop = F
  [../]
  [./dofs]
    type = NumDOFs
  [../]
  [./tstep]
    type = TimestepSize
  [../]
  [./int_area]
    type = InterfaceAreaPostprocessor
    variable = c
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
    #off_diag_column = 'c w c   c   gr0 gr1 gr0 gr1'
    #off_diag_row    = 'w c gr0 gr1 c   c  gr1  gr0'
  [../]
#[./FDP]
#  type = FDP
#  full = true
#  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  #petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  #petsc_options_value = 'lu       superlu_dist'

  line_search = none
  l_max_its = 20
  nl_max_its = 20
  nl_abs_tol = 1e-10
  nl_rel_tol = 1e-08
  l_tol = 1e-03
  end_time = 500
  #dt = 0.01
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
    growth_factor = 1.2
  [../]
  #[./Adaptivity]
  #  refine_fraction = 0.7
  #  coarsen_fraction = 0.1
  #  max_h_level = 2
  #  initial_adaptivity = 1
  #[../]
[]

[Adaptivity]
  marker = errorfrac
  max_h_level = 2
  [./Indicators]
    [./error]
      type = GradientJumpIndicator
      variable = bnds
    [../]
  [../]
  [./Markers]
    #[./bound_adapt]
    #  type = ValueThresholdMarker
    #  third_state = DO_NOTHING
    #  coarsen = 1.0
    #  refine = 0.99
    #  variable = bnds
    #  invert = true
    #[../]
    [./errorfrac]
      type = ErrorFractionMarker
      coarsen = 0.5
      indicator = error
      refine = 0.8
    [../]
  [../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
  gnuplot = true
  print_perf_log = true
  #interval = 10
  file_base = 2016_12_12_2p_sintdiff_3d
  [./console]
    type = Console
    perf_log = true
  [../]
[]

[ICs]
  [./ic_gr1]
    type = SmoothCircleIC
    int_width = 20.0
    x1 = 250.0
    y1 = 100.0
    z1 = 100.0
    radius = 74.0
    outvalue = 0.0
    variable = gr1
    invalue = 1.0
    3D_spheres = true
  [../]
  [./multip]
    type = SpecifiedSmoothCircleIC
    x_positions = '100.0 250.0'
    int_width = 20.0
    z_positions = '100.0 100.0'
    y_positions = '100.0 100.0 '
    radii = '74 74'
    3D_spheres = true
    outvalue = 0.001
    variable = c
    invalue = 0.999
    block = 0
  [../]
  [./ic_gr0]
    type = SmoothCircleIC
    int_width = 20.0
    x1 = 100.0
    y1 = 100.0
    z1 = 100.0
    radius = 74
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    3D_spheres = true
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
