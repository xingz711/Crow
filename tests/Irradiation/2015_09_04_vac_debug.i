[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 32
  ny = 32
  xmin = -64
  xmax = 64
  ymin = -64
  ymax = 64
[]

[Variables]
  [./cv]
  [../]
  [./wv]
  [../]
  [./e]
  [../]
[]

[AuxVariables]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  #[./IC_c]
  #  # int_width = 0.2
  #  int_width = 2.0
  #  x1 = 0.0
  #  y1 = 0.0
  #  radius = 11.48
  #  outvalue = 0.122
  #  variable = cv
  #  invalue = 0.999
  #  type = SmoothCircleIC
  #[../]
  #[./IC_e]
  #  int_width = 2.0
  #  x1 = 0
  #  y1 = 0
  #  radius = 11.48
  #  outvalue = 0.0
  #  variable = e
  #  invalue = 1.0
  #  type = SmoothCircleIC
  #[../]
  [./IC_c]
    type = RandomIC
    max = 0.999
    min = 0.001
    variable = cv
  [../]
  [./IC_e]
    type = RandomIC
    max = 1.0
    min = 0.0
    variable = e
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
    args = 'e'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = 'e'
  [../]
  [./AC_bulk]
    type = ACParsed
    variable = e
    f_name = F
    args = 'cv'
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
  [../]
[]

[AuxKernels]
  [./total_en]
    type = TotalFreeEnergy
    variable = total_en
    interfacial_vars = 'cv e'
    kappa_names = 'kappa_c kappa_op'
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    #function = '(Ef*cv+Ei*ci+kbT*(cv*log(cv)+ci*log(ci)+(1.0-cv-ci)*log(1-cv-ci)))'
    function = '(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1-cv)))'
    args = 'cv'
    f_name = fi
    tol_names = 'cv'
    tol_values = '0.001'
    constant_names = 'Ef kbT'
    constant_expressions = '1.0 0.11'
    derivative_order = 2
    outputs = exodus
  [../]
  [./Vacancy_en]
    type = DerivativeParsedMaterial
    block = 0
    function = (cv-1)^2
    f_name = fv
    args = 'cv'
    derivative_order = 2
  [../]
  [./switching_function]
    type = SwitchingFunctionMaterial
    block = 0
    eta = e
  [../]
  [./barrier_function]
    type = BarrierFunctionMaterial
    block = 0
    eta = e
  [../]
  [./free_en]
    type = DerivativeTwoPhaseMaterial
    block = 0
    args = 'cv'
    derivative_order = 2
    eta = e
    fa_name = fi
    fb_name = fv
    outputs = exodus
    output_properties = 'F dF/dc dF/deta d^2F/dc^2 d^2F/dcdeta d^2F/deta^2'
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
  [./Element_int_eta]
    type = ElementIntegralVariablePostprocessor
    variable = e
  [../]
  [./total_en]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_column = 'cv wv e cv'
    off_diag_row = 'wv cv cv e'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  num_steps = 2
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 0.005
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
  gnuplot = true
  interval = 100
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]
