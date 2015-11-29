[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  xmin = 0
  xmax = 60
  ymin = 0
  ymax = 60
  uniform_refine = 2
  elem_type = QUAD4
[]

[Variables]
  [./cv]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      #type = ConstantIC
      #value = 0.122
      type = SmoothCircleIC
      x1 = 30.0
      y1 = 30.0
      radius = 5.0
      outvalue = 0.122
      invalue = 0.999
    [../]
  [../]
  [./wv]
    order = FIRST
    family = LAGRANGE
  [../]
  [./e]
    [./InitialCondition]
      #type = ConstantIC
      #value = 0
      type = SmoothCircleIC
      x1 = 30.0
      y1 = 30.0
      radius = 5.0
      outvalue = 0.0
      invalue = 1.0
    [../]
  [../]
[]

#[AuxVariables]
#  [./gr0]
#    [./InitialCondition]
#      type = ConstantIC
#      value = 1
#    [../]
#  [../]
#[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Kernels]
  [./cv_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_c
    w = wv
  [../]
  [./wv_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = wv
    v = cv
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
    kappa_name = kappa_op
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'Mv kappa_c kappa_op L'
    prop_values = '1.0 1.0 1.0 1.0'
  [../]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(e-1)*(e-1)*(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1.0-cv)))+e*e*(cv-1.0)*(cv-1.0)'
    args = 'cv e'
    constant_names = 'Ef kbT'
    constant_expressions = '0.8 0.11'
    #f_name = Fc
    tol_names = 'cv'
    tol_values = '1e-5'
    derivative_order = 2
    outputs = exodus
  [../]
  #[./Energy_matrix]
  #  type = DerivativeParsedMaterial
  #  block = 0
  #  function = '(Ef*cv+kbT*(cv*log(cv)+(1.0-cv)*log(1.0-cv)))'
  #  args = 'cv'
  #  f_name = fi
  #  tol_names = 'cv'
  #  tol_values = '1e-6'
  #  constant_names = 'Ef kbT'
  #  constant_expressions = '0.8 0.11'
  #  derivative_order = 2
  #  outputs = exodus
  #[../]
  #[./Vacancy_en]
  #  type = DerivativeParsedMaterial
  #  block = 0
  #  function = '(cv-1.0)*(cv-1.0)'
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
  #  f_name = Fc
  #[../]

  #[./probability]
  #  # This is a made up toy nucleation rate it should be replaced by
  #  # classical nucleation theory in a real simulation.
  #  type = ParsedMaterial
  #  block = 0
  #  f_name = P
  #  args = gr0
  #  function = gr0*3e-3
  #  outputs = exodus
  #[../]
  #[./nucleation]
  #  # The nucleation material is configured to insert nuclei into the free energy
  #  # tht force the concentration to go to 0.95, and holds this enforcement for 500
  #  # time units.
  #  type = DiscreteNucleation
  #  block = 0
  #  f_name = Fn
  #  op_names  = 'e cv'
  #  op_values = '0.9 0.9'
  #  penalty = 5
  #  penalty_mode = MIN
  #  map = map
  #  outputs = exodus
  #[../]

  #[./free_energy]
  #  # add the chemical and nucleation free energy contributions together
  #  type = DerivativeSumMaterial
  #  derivative_order = 2
  #  block = 0
  #  args = 'cv e'
  #  sum_materials = 'Fc Fn'
  #[../]
[]

#[UserObjects]
#  [./inserter]
#    # The inserter runs at the end of each time step to add nucleation events
#    # that happend during the timestep (if it converged) to the list of nuclei
#    type = DiscreteNucleationInserter
#    hold_time = 100
#    probability = P
#  [../]
#  [./map]
#    # The map UO runs at the beginning of a timestep and generates a per-element/qp
#    # map of nucleus locations. The map is only regenerated if the mesh changed or
#    # the list of nuclei was modified.
#    # The map converts the nucleation points into finite area objects with a given radius.
#    type = DiscreteNucleationMap
#    radius = 1
#    int_width = 0.1
#    periodic = cv
#    inserter = inserter
#  [../]
#[]

[Preconditioning]
  [./SMP]
    type = SMP
    #full = true
    off_diag_column = 'wv cv e cv'
    off_diag_row =    'cv wv e cv'
  [../]
[]

[Executioner]
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  nl_abs_tol = 1e-10
  dt = 0.01
  end_time = 5
[]

[Outputs]
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
