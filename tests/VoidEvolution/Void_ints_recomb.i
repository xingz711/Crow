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
  active = 'ci e wi wv cv'
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
  [./gr0]
    block = 0
  [../]
  [./gr1]
    block = 0
  [../]
[]

[AuxVariables]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  active = 'const_c const_e ci_ic'
  [./IC_c]
    # int_width = 0.2
    int_width = 5.0
    x1 = 0.0
    y1 = 0.0
    radius = 11.48
    outvalue = 0.122
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
  [./const_c]
    variable = cv
    value = 6.9e-4
    type = ConstantIC
    block = 0
  [../]
  [./const_e]
    variable = e
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
  [./IC_gr0]
    y2 = 64
    x1 = -64
    y1 = -64
    inside = 1.0000
    grain_side = left
    x2 = 0
    variable = gr0
    int_width = 2
    type = BicrystalIC
    block = 0
  [../]
  [./IC_gr1]
    y2 = 64
    x1 = 0
    y1 = -64
    inside = 1.0
    grain_side = right
    x2 = 64
    variable = gr1
    int_width = 2
    type = BicrystalIC
    block = 0
  [../]
  [./ci_ic]
    variable = ci
    value = 6.9e-4
    type = ConstantIC
    block = 0
  [../]
[]

[Kernels]
  active = 'AC_int w_res c_dot ints_noise e_dot c_res CH_noise ci_res AC_bulk wi_res cv_recomb CH_Pv_rand ci_dot'
  [./c_dot]
    type = CoupledImplicitEuler
    variable = wv
    v = cv
    block = 0
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_cv
    w = wv
    args = 'ci e'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = M_cv
    args = 'ci e'
  [../]
  [./AC_bulk]
    type = ACParsed
    variable = e
    f_name = F
    args = cv
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
  [./CH_Pv_rand]
    type = VacancyRecombinationTermKernel
    variable = ci
    eta = e
    block = 0
    ci = cv
    Rs = 10e9
    Rbulk = 1e9
  [../]
  [./CH_noise]
    type = ConservedLangevinNoise
    variable = cv
    amplitude = 0.25
    block = 0
    noise = noise
  [../]
  [./VacancyAhhihilation]
    type = VacancyAnnihilationKernel
    variable = cv
    ceq = 1.13e-4
    v = 'gr0  gr1'
    block = 0
  [../]
  [./ci_dot]
    type = CoupledImplicitEuler
    variable = wi
    block = 0
    v = ci
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    args = 'cv e'
    kappa_name = kappa_ci
    w = wi
    block = 0
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = M_ci
    block = 0
    args = 'cv e'
  [../]
  [./cv_recomb]
    type = VacancyRecombinationTermKernel
    variable = cv
    ci = ci
    Rs = 1oe9
    Rbulk = 1e9
    eta = e
    block = 0
  [../]
  [./ints_noise]
    type = ConservedLangevinNoise
    variable = ci
    noise = noise
    amplitude = 0.2
    block = 0
  [../]
[]

[AuxKernels]
  [./total_en]
    type = TotalFreeEnergy
    variable = total_en
    interfacial_vars = 'cv ci e'
    kappa_names = 'kappa_cv kappa_ci kappa_op'
    block = 0
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
  active = 'mob_ci AC_mat Vacancy_en switching_function barrier_function Mob free_en Energy_matrix'
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(Ef*cv+Ei*ci+ kbT*(cv*log(cv)+ci*log(ci)+(1-cv-ci)*log(1-cv-ci)))'
    args = 'cv ci e'
    constant_names = 'Ef Ei kbT '
    constant_expressions = '0.8  1.2 0.11 '
    tol_values = '1e-6 1e-6'
    tol_names = 'cv ci '
    derivative_order = 2
    f_name = fi
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
    prop_names = 'L kappa_op mask '
    prop_values = '1.0 1.0 1.0'
  [../]
  [./Rand_mat]
    type = RandomVacancySourceTermMaterial
    block = 0
    seed = 1
  [../]
  [./Mob]
    type = Mobility
    block = 0
    Em = 0.8
    c = cv
    Ef = 0.8
    surface_energy = 0.7
    D0 = 9.1e-6
    int_width = 5.0
    T = 1275
  [../]
  [./Vacancy_en]
    type = DerivativeParsedMaterial
    block = 0
    function = (cv-1)^2+ci^2
    f_name = fv
    args = 'cv ci e'
    derivative_order = 2
  [../]
  [./free_en]
    type = DerivativeTwoPhaseMaterial
    block = 0
    args = 'cv ci'
    derivative_order = 2
    eta = e
    fa_name = fi
    fb_name = fv
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
  [./mob_ci]
    type = Mobility
    block = 0
    Em = 0.8
    c = ci
    Ef = 0.8
    surface_energy = 0.7
    D0 = 9.1e-6
    int_width = 5.0
    T = 1275
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
  [./total_en]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
    block = 0
  [../]
[]

[UserObjects]
  [./noise]
    type = ConservedMaskedUniformNoise
    mask = mask
    block = 0
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
  end_time = 100.0
  dt = 0.5
  l_max_its = 35
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_tol = 1.0e-6
  nl_rel_tol = 1.0e-6
  nl_abs_tol = 1.0e-6
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

