# 
# Example problem showing how to use the DerivativeParsedMaterial with CHParsed.
# The free energy is identical to that from CHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmin = -5
  xmax = 5
  ymin = -5
  ymax = 5
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
    block = 0
    [./InitialCondition]
      value = 0.0
      variable = e
      type = ConstantIC
      block = 0
    [../]
  [../]
[]

[ICs]
  [./ci_IC]
    variable = ci
    value = 0.003
    type = ConstantIC
    block = 0
  [../]
  [./IC_cv]
    # int_width = 0.2
    variable = cv
    value = 0.113
    type = ConstantIC
    block = 0
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledImplicitEuler
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
    mob_name = D
  [../]
  [./ci_dot]
    type = CoupledImplicitEuler
    variable = wi
    v = ci
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    kappa_name = kappa_c
    w = wi
    args = 'cv e'
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = D
  [../]
  [./AC_res]
    type = ACParsed
    variable = e
    f_name = F
    args = 'cv ci'
    block = 0
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
  [./CH_noise]
    type = LangevinNoiseVoid
    variable = cv
    eta = e
    amplitude = 0.1
  [../]
  [./CHi_noise]
    type = LangevinNoiseVoid
    variable = ci
    eta = e
    amplitude = 0.1
  [../]
  [./Recomb_cv]
    type = VacancyRecombinationTermKernel
    variable = cv
    ci = ci
    Rs = 10
    eta = e
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = 'cv ci e'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(e-1)^2*(Ef*cv + Ef*ci + kbT*(cv*log(cv)+ci*log(ci)+(1-cv-ci)*log(1-cv-ci)))+e^2*((cv-1)^2+ci^2)'
    args = 'cv ci e'
    constant_names = 'Ef   kbT'
    constant_expressions = '1.0  0.11'
    tol_values = '1e-12 1e-12'
    tol_names = 'cv ci '
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = cv
  [../]
  [./Mobility_ci]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = ci
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '1.0 1.0'
  [../]
[]

[Postprocessors]
  [./Int_ci]
    type = ElementIntegralVariablePostprocessor
    variable = ci
    block = 0
  [../]
  [./Int_cv]
    type = ElementAverageTimeDerivative
    variable = cv
    block = 0
  [../]
  [./Int_e]
    type = ElementIntegralVariablePostprocessor
    variable = e
    block = 0
  [../]
  [./No. of Voids]
    type = NodalFloodCount
    variable = e
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
  solve_type = PJFNK
  end_time = 20
  dt = 0.005
  l_max_its = 35
  l_tol = 1.0e-6
  nl_max_its = 50
  nl_rel_tol = 1.0e-6
  nl_abs_tol = 1.0e-6
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

