[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 128
  ny = 128
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
  [./const_e]
    variable = e
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
  [./cv_ic]
    variable = cv
    value = 0.122
    type = ConstantIC
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
  [./src_cv]
    type = RadiationDefectSource
    variable = cv
  [../]
  [./src_e]
    type = RadiationDefectSource
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
    #function = '(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1-cv)))'
    function = '(e-1.0)^2*(e+1.0)^2*(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1.0-cv)))-A*(cv-ceq)^2*e*(e+2.0)*(e-1.0)^2+B*(cv-1.0)^2*e^2'
    args = 'cv e'
    #f_name = fi
    tol_names = 'cv'
    tol_values = '0.001'
    #constant_names = 'Ef kbT'
    #constant_expressions = '1.0 0.11'
    constant_names = 'Ef kbT A B ceq'
    constant_expressions = '1.0 0.11 1.0 1.0 1.13e-4'
    derivative_order = 2
    outputs = exodus
  [../]
  #[./Vacancy_en]
  #  type = DerivativeParsedMaterial
  #  block = 0
  #  function = (cv-1)^2
  #  f_name = fv
  #  args = 'cv'
  #  derivative_order = 2
  #[../]
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
  #  args = 'cv'
  #  derivative_order = 2
  #  eta = e
  #  fa_name = fi
  #  fb_name = fv
  #  outputs = exodus
  #  output_properties = 'F dF/dc dF/deta d^2F/dc^2 d^2F/dcdeta d^2F/deta^2'
  #[../]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'kappa_c Mv Mi L kappa_op'
    prop_values = '0.5 1.0 1.0 1.0 1.0'
  [../]
  [./NeutronSrc]
    type = RadiationDefectCreation
    block = 0
    Vg = 1e-2
    x1 = -64
    y1 = -64
    x2 = 64
    y2 = 64
    eta = e
    periodic = true
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
  end_time = 50.0
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  dt = 0.01
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  #nl_abs_tol = 1.0e-6
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
[]

[Outputs]
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
  gnuplot = true
  interval = 50
  print_linear_residuals = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]
