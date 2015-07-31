[Mesh]
  file = void_gb_poly.e
[]

[Variables]
  [./u]
    block = 0
  [../]
  [./v]
    block = 0
  [../]
  [./c]
    block = 0
  [../]
  [./b]
    block = 0
  [../]
[]

[AuxVariables]
  [./resid_x]
    block = 0
  [../]
  [./resid_y]
    block = 0
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./dvoid]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./gcproptens]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./gcpropcomp]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./bnd]
    order = FIRST
    family = LAGRANGE
  [../]
  [./conc]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Functions]
  [./tfunc]
    type = ParsedFunction
    value = 15.0*t
  [../]
  [./bndfunc]
    type = SolutionFunction
    solution = soln
    from_variable = bnds
  [../]
  [./concfunc]
    type = SolutionFunction
    solution = soln
    from_variable = c
  [../]
[]

[Kernels]
  [./acbulk]
    type = ACFracBulkRate
    variable = c
    block = 0
    l = 1.0
    beta = b
    visco =1e-3
    gc_tens = 0.001
    gc_comp = 1000
  [../]
  [./solid_x]
    type = StressDivergenceTensors
    variable = u
    disp_x = u
    disp_y = v
    component = 0
    block = 0
    save_in = resid_x
  [../]
  [./solid_y]
    type = StressDivergenceTensors
    variable = v
    disp_y = v
    disp_x = u
    component = 1
    block = 0
    save_in = resid_y
  [../]
  [./dcdt]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./acintvar]
    type = ACFracIntVar
    variable = b
    block = 0
    c = c
  [../]
  [./acintcoupled]
    type = ACFracCoupledInterface
    variable = b
    c = c
    block = 0
  [../]
[]

[AuxKernels]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    execute_on = timestep_end
    block = 0
  [../]
  [./dvoid]
    type = MaterialRealAux
    variable = dvoid
    property = d_void
    execute_on = timestep_end
    block = 0
  [../]
  [./gcproptens]
    type = MaterialRealAux
    variable = gcproptens
    property = gc_prop_tens
    execute_on = timestep_end
    block = 0
  [../]
  [./gcpropcomp]
    type = MaterialRealAux
    variable = gcpropcomp
    property = gc_prop_comp
    execute_on = timestep_end
    block = 0
  [../]
  [./bnd]
    type = FunctionAux
    variable = bnd
    function = bndfunc
  [../]
  [./conc]
    type = FunctionAux
    variable = conc
    function = concfunc
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionPresetBC
    variable = v
    boundary = top
    function = tfunc
  [../]
  [./yfix]
    type = PresetBC
    variable = v
    boundary = bottom
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = u
    boundary = left
    value = 0
  [../]
[]

[Materials]
  [./acbulkmatsolic]
    type = FractureACBulkMatRateSolIC
    block = 0
    bnd = bnd
    conc = conc
    gc_tens_gb = 0.001
    gc_tens_mat = 1000
    gc_comp_gb = 1000
    gc_comp_mat = 1000
    gb_bnd = 0.9
  [../]
  [./acintmat]
    type = FractureACInterfaceMatRate
    block = 0
    l = 1.0
  [../]
  [./elastic]
    type = LinearElasticPFDamageSolIC
    block = 0
    c = c
    kvoid = 1e-8
    disp_y = v
    disp_x = u
    C_ijkl = '280.0 120.0 120.0 280.0 120.0 280.0 80.0 80.0 80.0'
    fill_method = symmetric9
    formulation = 0
    conc = conc
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    pc_side = left
    off_diag_row    = 'u v b c'
    off_diag_column = 'v u c b'
  [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-pc_type    -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31                  preonly       lu           64'

  dt = 1e-4
  l_max_its = 30
  nl_max_its = 20
  nl_rel_tol = 1e-10

  num_steps = 1
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

[UserObjects]
  [./soln]
    type = SolutionUserObject
    system_variables = 'c bnds'
    mesh = void_gb_poly.e
    timestep = 2
  [../]
[]
