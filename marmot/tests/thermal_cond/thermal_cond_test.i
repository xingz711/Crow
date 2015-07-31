[Mesh]
  type = FileMesh
  file = bicrystal_gen_0000_mesh.xda
[]

[GlobalParams]
  mesh = bicrystal_gen_0000_mesh.xda
  es = bicrystal_gen_0000.xda
[]

[Variables]
  [./Tx]
    initial_condition = 800.0
    scaling = 1.0e10
  [../]
[]

[Functions]
  [./gr0_func]
    type = SolutionFunction
    solution = PF_soln
    from_variable = gr0
  [../]
  [./gr1_func]
    type = SolutionFunction
    solution = PF_soln
    from_variable = gr1
  [../]
  [./c_func]
    type = SolutionFunction
    solution = PF_soln
    from_variable = c
  [../]
[]

[UserObjects]
  [./PF_soln]
    type = SolutionUserObject
    execute_on = initial
    system_variables = 'gr0 gr1 c'
  [../]
[]

[AuxVariables]
  [./th_cond]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./auxgr0]
  [../]
  [./auxgr1]
  [../]
  [./auxc]
  [../]
[]

[ICs]
  [./gr0_IC]
    function = gr0_func
    variable = auxgr0
    type = FunctionIC
  [../]
  [./gr1_IC]
    function = gr1_func
    variable = auxgr1
    type = FunctionIC
  [../]
  [./c_IC]
    function = c_func
    variable = auxc
    type = FunctionIC
  [../]
[]

[Kernels]
  [./heat_cond_x]
    type = HeatConduction
    variable = Tx
  [../]
[]

[AuxKernels]
  [./th_cond]
    type = MaterialRealAux
    variable = th_cond
    execute_on = timestep_end
    property = thermal_conductivity
  [../]
[]

[BCs]
  [./Periodic]
    [./Tx_ydir]
      auto_direction = y
      variable = Tx
    [../]
  [../]
  [./Const_Tx]
    type = PresetBC
    variable = Tx
    boundary = left
    value = 800.0
  [../]
  [./Const_Fluxx]
    # value = 0.0
    type = NeumannBC
    variable = Tx
    boundary = right
    value = -0.5e-11
  [../]
[]

[Materials]
  [./Thermal]
    type = MicroThermal
    block = 0
    Gas_conductivity = 0.00565 # Xe at 300K
    length_scale = 1e-09
    GB_conductivity = 1.0
    eta = auxc
    Bulk_conductivity = 4.75 # UO2 at 800K
    var_name_base = auxgr
    op_num = 2
  [../]
[]

[Postprocessors]
  [./Tx_right]
    type = SideAverageValue
    boundary = right
    variable = Tx
  [../]
  [./th_cond_x]
    type = ThermalCond
    variable = Tx
    boundary = right
    flux = 0.5e-11
    dx = 1500.0
    T_hot = 800
    length_scale = 1e-9
  [../]
  [./Rprime_x]
    type = EffectiveKapitzaResistance
    dx = 750.0 # Grain size
    ko = 4.75
    eff_cond = th_cond_x
    length_scale = 1.0e-9
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = -pc_type
  petsc_options_value = ' lu'
  l_max_its = 30
  l_tol = 1.0e-5
  nl_max_its = 15
  nl_rel_tol = 1.0e-10
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]

