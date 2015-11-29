[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 80
  xmin = -50
  xmax = 50
  ymin = -50
  ymax = 50
  elem_type = QUAD4
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
  [./gr0]
  [../]
  [./gr1]
  [../]
[]

[ICs]
  [./IC_Cv_1]
    variable = cv
    type = ConstantIC
    value = 0.12
  [../]
  [./IC_e]
    variable = e
    type = ConstantIC
    value = 0
  [../]
  [./IC_gr0]
    y2 = 50
    x1 = -50
    y1 = -50
    inside = 1.0
    x2 = 0
    variable = gr0
    type = BoundingBoxIC
    block = 0
    outside = 0
  [../]
  [./IC_gr1]
    y2 = 50
    x1 = 0
    y1 = -50
    inside = 1.0
    x2 = 50
    variable = gr1
    type = BoundingBoxIC
    block = 0
    outside = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
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
    args = e
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = e
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
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
  [../]
  [./gr]
    type = GBVacancyAnnihilation
    variable = cv
    v = 'gr0 gr1'
    ceq = 6.9e-4
    Svgb = 3
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'Mv kappa_c kappa_op L'
    prop_values = '2.0 1.0 1.0 1.0'
  [../]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1.0-cv)))'
    args = 'cv'
    f_name = fi
    tol_names = 'cv'
    tol_values = '1e-5'
    constant_names = 'Ef kbT'
    constant_expressions = '0.8 0.11'
    derivative_order = 2
    outputs = exodus
  [../]
  [./Vacancy_en]
    type = DerivativeParsedMaterial
    block = 0
    function = '(cv-1.0)*(cv-1.0)'
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
    f_name = Fc
  [../]
  [./probability]
    # This is a made up toy nucleation rate it should be replaced by
    # classical nucleation theory in a real simulation.
    type = ParsedMaterial
    block = 0
    f_name = P
    args = 'cv'
    function = '5e-3*cv'
    outputs = exodus
  [../]
  [./nucleation]
    # The nucleation material is configured to insert nuclei into the free energy
    # that force the concentration to go to 0.95, and holds this enforcement for 500
    # time units.
    type = PolyDiscreteNucleation
    block = 0
    f_name = Fn
    op_names = 'e cv'
    op_values = '0.90 0.9'
    penalty = 5
    penalty_mode = MIN
    map = map
    outputs = exodus
    v = 'gr0 gr1'
    derivative_order = 2
  [../]
  [./free_energy]
    # add the chemical and nucleation free energy contributions together
    type = DerivativeSumMaterial
    derivative_order = 2
    block = 0
    args = 'cv e'
    sum_materials = 'Fc Fn'
  [../]
[]

[UserObjects]
  [./inserter]
    # The inserter runs at the end of each time step to add nucleation events
    # that happend during the timestep (if it converged) to the list of nuclei
    type = DiscreteNucleationInserter
    hold_time = 20
    probability = P
  [../]
  [./map]
    # The map UO runs at the beginning of a timestep and generates a per-element/qp
    # map of nucleus locations. The map is only regenerated if the mesh changed or
    # the list of nuclei was modified.
    # The map converts the nucleation points into finite area objects with a given radius.
    type = DiscreteNucleationMap
    radius = 2.0
    int_width = 0.1
    periodic = cv
    inserter = inserter
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
  end_time = 20.0
  l_max_its = 20
  nl_max_its = 20
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  dt = 0.01
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
[]

[Outputs]
  # print_linear_residuals = true
  # output_on = 'timestep_end initial'
  exodus = true
  csv = true
  gnuplot = true
  [./console]
    # output_on = 'timestep_end failed nonlinear linear'
    type = Console
    perf_log = true
  [../]
[]
