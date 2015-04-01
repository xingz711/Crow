# 
# Example problem showing how to use the DerivativeParsedMaterial with CHParsed.
# The free energy is identical to that from CHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 256
  ny = 256
  xmin = -64
  xmax = 64
  ymin = -64
  ymax = 64
[]

[Variables]
  [./cv]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./wv]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./ci]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./wi]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./e]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
[]

[AuxVariables]
  [./gr0]
    block = 0
  [../]
  [./gr1]
    block = 0
  [../]
[]

[ICs]
  active = 'PolycrystalICs const_ci const_e const_cv'
  [./IC_c]
    # int_width = 0.2
    int_width = 5.0
    x1 = 0.0
    y1 = 0.0
    radius = 11.48
    outvalue = 0.113
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
    block = 0
  [../]
  [./IC_e]
    int_width = 5.0
    x1 = 0
    y1 = 0
    radius = 11.48
    outvalue = 0.0
    variable = e
    invalue = 1.0
    type = SmoothCircleIC
    block = 0
  [../]
  [./rnd_R1]
    variable = R1
    type = RandomIC
    block = 0
  [../]
  [./rand_R2]
    variable = R2
    type = RandomIC
    block = 0
  [../]
  [./const_cv]
    variable = cv
    value = 0.07
    type = ConstantIC
    block = 0
  [../]
  [./const_ci]
    variable = ci
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
  [./const_e]
    variable = e
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      var_name_base = gr
      y2 = 64.0
      op_num = 2
      y1 = -64.0
      x2 = 0.0
      x1 = -64.0
    [../]
  [../]
[]

[Kernels]
  active = 'AC_int e_dot cv_dot CH_noise CHi_noise ci_res AC_bulk wi_res VacancyAhhihilation cv_res wv_res InterstitialsAhhihilation ci_dot Recomb_cv recomb_ci'
  [./cv_dot]
    type = CoupledImplicitEuler
    variable = wv
    v = cv
    block = 0
  [../]
  [./cv_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_c
    w = wv
    args = 'e ci'
  [../]
  [./wv_res]
    type = SplitCHWRes
    variable = wv
    mob_name = D
  [../]
  [./ci_dot]
    type = CoupledImplicitEuler
    variable = wi
    v = ci
    block = 0
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    kappa_name = kappa_c
    w = wi
    args = 'e cv'
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = D
  [../]
  [./AC_bulk]
    type = ACParsed
    variable = e
    f_name = F
    args = 'cv ci'
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
    block = 0
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
    block = 0
  [../]
  [./CH_Pv]
    type = VacancySourceTermKernel
    variable = cv
    Vg = 0.25
    eta = e
    block = 0
  [../]
  [./CH_Pv_rand]
    type = RandomVacancySourceTermKernel
    variable = cv
    Vg = 0.1
    eta = e
    block = 0
  [../]
  [./CH_noise]
    type = LangevinNoiseVoid
    variable = cv
    amplitude = 0.1
    block = 0
    eta = e
  [../]
  [./VacancyAhhihilation]
    type = VacancyAnnihilationKernel
    variable = cv
    c = ' e'
    ceq = 1.13e-4
    v = 'gr0  gr1'
    block = 0
  [../]
  [./CHi_noise]
    type = LangevinNoiseVoid
    variable = ci
    amplitude = 0.1
    block = 0
    eta = e
  [../]
  [./InterstitialsAhhihilation]
    type = VacancyAnnihilationKernel
    variable = ci
    c = ' e'
    ceq = 1.13e-4
    v = 'gr0  gr1'
    block = 0
  [../]
  [./Recomb_cv]
    type = VacancyRecombinationTermKernel
    variable = cv
    ci = ci
    Rs = 10
    eta = e
    block = 0
  [../]
  [./recomb_ci]
    type = VacancyRecombinationTermKernel
    variable = ci
    ci = cv
    Rs = 10
    eta = e
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = 'cv e wv'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  active = 'AC_mat Energy_matrix'
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(e-1)^2*(Efv*cv+ Efi*ci+ kbT*(cv*log(cv) + ci*log(ci) + (1-cv-ci)*log(1-cv-ci))) + ((cv-1)^2+ci^2)*e^2'
    args = 'cv ci e'
    constant_names = 'Efv Efi  kbT'
    constant_expressions = '9.09 9.09 0.11'
    tol_values = '0.001 0.001'
    tol_names = 'cv ci '
    third_derivatives = false
  [../]
  [./Mobility]
    type = PFMobility
    block = 0
    mob = '1.0 '
    kappa = 0.5
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op kappa_c D S'
    prop_values = '1.0 1.0 0.5 1.0 1.0'
  [../]
  [./Rand_mat]
    type = RandomVacancySourceTermMaterial
    block = 0
    seed = 1
  [../]
[]

[Postprocessors]
  [./ElementInt_cv]
    type = ElementIntegralVariablePostprocessor
    variable = cv
    block = 0
  [../]
  [./Element_int_eta]
    type = ElementIntegralVariablePostprocessor
    variable = e
  [../]
  [./Element_int_ci]
    type = ElementIntegralVariablePostprocessor
    variable = ci
  [../]
  [./Nodalflood]
    type = NodalFloodCount
    variable = cv
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
  end_time = 20
  dt = 0.005
  l_max_its = 35
  l_tol = 1.0e-6
  nl_rel_tol = 1.0e-6
  nl_abs_tol = 1.0e-6
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  tecplot = true
  file_base = RandvoidsourceAnnihilation
  csv = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

