[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  xmin = -10
  xmax = 10
  ymin = -10
  ymax = 10
[]

[Variables]
  [./cv]
  [../]
  [./wv]
  [../]
  #[./ci]
  #[../]
  #[./wi]
  #[../]
[]

[AuxVariables]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./e]
  [../]
  [./ci]
  [../]
[]

[ICs]
  [./IC_c]
    # int_width = 0.2
    int_width = 1.0
    x1 = 0.0
    y1 = 0.0
    radius = 5.0
    outvalue = 0.122
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
  [../]
  [./IC_e]
    # int_width = 0.2
    int_width = 1.0
    x1 = 0.0
    y1 = 0.0
    radius = 5.0
    outvalue = 0.0
    variable = e
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./IC_ci]
    # int_width = 0.2
    int_width = 1.0
    x1 = 0.0
    y1 = 0.0
    radius = 5.0
    outvalue = 6.9e-4
    variable = ci
    invalue = 1e-6
    type = SmoothCircleIC
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = wv
    v = cv
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_c
    w = wv
    args = 'ci'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = 'ci'
  [../]
  #[./ci_dot]
  #  type = CoupledTimeDerivative
  #  variable = wi
  #  v = ci
  #[../]
  #[./ci_res]
  #  type = SplitCHParsed
  #  variable = ci
  #  f_name = F
  #  args = 'cv'
  #  kappa_name = kappa_c
  #  w = wi
  #[../]
  #[./wi_res]
  #  type = SplitCHWRes
  #  variable = wi
  #  mob_name = Mi
  #  args = 'cv'
  #[../]
  #[./cv_recomb]
  #  type = VacancyRecombination
  #  variable = cv
  #  ci = ci
  #  Rs = 10.0
  #  eta = e
  #[../]
  #[./ci_recomb]
  #  type = VacancyRecombination
  #  variable = ci
  #  ci = cv
  #  Rs = 10.0
  #  eta = e
  #[../]
[]

[AuxKernels]
  [./total_en]
    type = TotalFreeEnergy
    variable = total_en
    interfacial_vars = 'cv'
    kappa_names = 'kappa_c'
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    #function = '(Ef*cv+Ei*ci+kbT*(cv*log(cv)+ci*log(ci)+(1.0-cv)*log(1.0-cv)+(1.0-ci)*log(1.0-ci)))'
    function = '(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1.0-cv)))'
    args = 'cv'
    tol_names = 'cv'
    tol_values = '0.001'
    constant_names = 'Ef kbT'
    constant_expressions = '1.0 0.11'
    derivative_order = 2
    outputs = exodus
  [../]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'kappa_c Mv Mi L kappa_op'
    prop_values = '0.5 1.0 1.0 1.0 1.0'
  [../]
[]

[Postprocessors]
  [./ElementInt_cv]
    type = ElementIntegralVariablePostprocessor
    variable = cv
  [../]
  [./total_en]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_column = 'cv wv'
    off_diag_row =    'wv cv'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  end_time = 1.0
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 0.005
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  #nl_abs_tol = 1.0e-6
  #[./Adaptivity]
  #  refine_fraction = 0.7
  #  coarsen_fraction = 0.1
  #  max_h_level = 2
  #  initial_adaptivity = 1
  #[../]
  #[./TimeStepper]
  #  type = IterationAdaptiveDT
  #  dt = 0.01
  #[../]
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
  gnuplot = true
  #interval = 100
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]
