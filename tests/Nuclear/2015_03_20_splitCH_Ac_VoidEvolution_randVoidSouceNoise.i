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
  [./e]
  [../]
[]

[ICs]
  active = 'const_c const_e'
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
  [./const_c]
    variable = cv
    value = 0.113
    type = ConstantIC
    block = 0
  [../]
  [./const_e]
    variable = e
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
[]

[Kernels]
  active = 'AC_int w_res c_dot e_dot c_res CH_noise AC_bulk'
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
    kappa_name = kappa_c
    w = wv
    args = e
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = D
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
    amplitude = 0.5
    block = 0
    eta = e
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
    function = '(e^2-1)^2*(Ef*cv+ kbT*(cv*log(cv)+(1-cv)*log(1-cv))) - A*(cv-cv0)^2*(e^4-3*e^3+2*e)+B*(cv-1)^2*e^2'
    args = 'cv e'
    constant_names = 'Ef   kbT A cv0 B'
    constant_expressions = '9.09  0.11  9.09 1.13e-4 9.09'
    tol_values = 0.001
    tol_names = cv
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
    prop_names = 'L kappa_op kappa_c D'
    prop_values = '1.0 1.0 0.5 1.0 '
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
  file_base = Randvoidsource
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

