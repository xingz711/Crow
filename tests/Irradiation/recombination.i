[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
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
[]

[AuxVariables]
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
    int_width = 0.2
    x1 = 0.0
    y1 = 0.0
    radius = 5.0
    outvalue = 0.1
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
  [../]
  [./IC_e]
    int_width = 0.2
    x1 = 0
    y1 = 0
    radius = 5.0
    outvalue = 0.0
    variable = e
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./IC_ci]
    int_width = 0.2
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
    args = 'ci'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = 'ci'
  [../]
  [./ci_dot]
    type = CoupledTimeDerivative
    variable = wi
    v = ci
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    args = 'cv'
    kappa_name = kappa_c
    w = wi
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = Mi
    args = 'cv'
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
  [./Vacancy_en]
    type = DerivativeParsedMaterial
    block = 0
    function = (cv-1.0)*(cv-1.0)+ci*ci
    args = 'cv ci'
    derivative_order = 2
  [../]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'kappa_c Mv Mi '
    prop_values = '1.0 0.5 0.5 '
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
  #petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  #petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 1.0
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  [./console]
    type = Console
    perf_log = true
  [../]
[]
