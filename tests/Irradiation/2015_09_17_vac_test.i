[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
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
  [./ci]
  [../]
  [./wi]
  [../]
  #[./e]
  #[../]
[]

[AuxVariables]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./e]
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[ICs]
  [./IC_c]
    int_width = 1.0
    x1 = 0.0
    y1 = 0.0
    radius = 5.0
    outvalue = 0.2
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
  [../]
  [./IC_e]
    int_width = 1.0
    x1 = 0
    y1 = 0
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
    outvalue = 7e-4
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
    args = 'ci e'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = 'ci e'
  [../]
  #[./AC_bulk]
  #  type = ACParsed
  #  variable = e
  #  f_name = F
  #  args = 'cv ci'
  #[../]
  #[./AC_int]
  #  type = ACInterface
  #  variable = e
  #[../]
  #[./e_dot]
  #  type = TimeDerivative
  #  variable = e
  #[../]
  [./ci_dot]
    type = CoupledTimeDerivative
    variable = wi
    v = ci
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    args = 'cv e'
    kappa_name = kappa_c
    w = wi
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = Mi
    args = 'cv e'
  [../]
  [./cv_recomb]
    type = VacancyRecombination
    variable = cv
    ci = ci
    Rs = 10.0
    Rbulk = 1.0
    eta = e
  [../]
  [./ci_recomb]
    type = VacancyRecombination
    variable = ci
    ci = cv
    Rs = 10.0
    Rbulk = 1.0
    eta = e
  [../]
  [../]
[]

[Materials]
  #[./Energy_matrix]
  #  type = DerivativeParsedMaterial
  #  block = 0
  #  function = '(Ef*cv+Ei*ci+kbT*(cv*log(cv)+ci*log(ci)+(1.0-ci)*log(1.0-ci)+(1.0-cv)*log(1.0-cv)))'
  #  args = 'cv ci'
  #  f_name = fi
  #  tol_names = 'cv ci'
  #  tol_values = '1e-6 1e-6'
  #  constant_names = 'Ef kbT Ei'
  #  constant_expressions = '1.0 0.11 1.0'
  #  derivative_order = 2
  #[../]
  [./Vacancy_en]
    type = DerivativeParsedMaterial
    block = 0
    function = (cv-1.0)*(cv-1.0)+ci*ci
    #f_name = fv
    args = 'cv ci'
    derivative_order = 2
  [../]
  #[./switching_function]
  #  type = SwitchingFunctionMaterial
  #  block = 0
  #  eta = e
  #[../]
  #[./barrier_function]
  #  type = BarrierFunctionMaterial
  #  block = 0
  #  eta = e
  #[../]
  #[./free_en]
  #  type = DerivativeTwoPhaseMaterial
  #  block = 0
  #  args = 'cv ci'
  #  derivative_order = 2
  #  eta = e
  #  fa_name = fi
  #  fb_name = fv
  #[../]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'kappa_c Mv Mi L kappa_op'
    prop_values = '0.5 1.0 1.0 1.0 1.0'
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
  num_steps = 2
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 1.0
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
[]

[Outputs]
  exodus = true
  execute_on = 'INITIAL TIMESTEP_END'
  output_on = 'initial timestep_end'
  [./console]
    type = Console
    perf_log = true
    execute_on = 'FAILED INITIAL LINEAR NONLINEAR TIMESTEP_BEGIN TIMESTEP_END'
  [../]
[]
