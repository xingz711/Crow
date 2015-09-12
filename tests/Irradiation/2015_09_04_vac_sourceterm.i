[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 64
  ny = 64
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
  #[./rnd_cv]
  #  variable = cv
  #  type = RandomIC
  #  max = 0.999
  #  min = 0.001
  #[../]
  #[./rnd_e]
  #  variable = e
  #  type = RandomIC
  #  max = 1.0
  #  min = 0.0
  #[../]
  [./const_cv]
    variable = cv
    type = ConstantIC
    value = 0.113
  [../]
  [./rnd_e]
    variable = e
    type = ConstantIC
    value = 0.0
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
  [./cv_source]
    type = LangevinNoiseVoid
    Pcasc = 0.9
    eta = e
    amplitude = 1.0
    variable = cv
  [../]
  [./e_source]
    type = LangevinNoiseVoid
    Pcasc = 0.9
    eta = e
    amplitude = 1.0
    variable = e
  [../]
[]

[AuxKernels]
  [./total_en]
    type = TotalFreeEnergy
    variable = total_en
    interfacial_vars = 'cv e'
    kappa_names = 'kappa_c kappa_op'
    block = 0
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    #function = '(Ef*cv+Ei*ci+kbT*(cv*log(cv)+ci*log(ci)+(1.0-cv-ci)*log(1-cv-ci)))'
    function = '(e-1.0)^2*(e+1.0)^2*(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1-cv)))-A*(cv-ceq)^2*e*(e+2.0)*(e-1.0)^2+B*(cv-1.0)^2*e^2'
    args = 'cv e'
    #f_name = fi
    tol_names = 'cv'
    tol_values = '0.001'
    constant_names = 'Ef kbT A B ceq'
    constant_expressions = '1.0 0.11 1.0 1.0 1.13e-4'
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
    off_diag_column = 'cv wv e cv'
    off_diag_row = 'wv cv cv e'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  end_time = 20.0
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 0.01
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
  gnuplot = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]
