[Mesh]
  type = FileMesh
  file = crack-mesh.e
[]

[Variables]
  [./u]
    block = 1
  [../]
  [./v]
    block = 1
  [../]
  [./c]
    block = 1
  [../]
  [./b]
    block = 1
  [../]
[]

[AuxVariables]
  [./resid_x]
    block = 1
  [../]
  [./resid_y]
    block = 1
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
    block = 1
  [../]
[]

[Functions]
  [./tfunc]
    type = ParsedFunction
    value = t
  [../]
[]

[Kernels]
  [./acbulk]
    type = ACFracBulkRate
    variable = c
    block = 1
    l = .08
    beta = b
    visco =1.0
    gc_tens = 2.7e-3
    gc_comp = 1000
  [../]
  [./solid_x]
    type = StressDivergenceTensors
    variable = u
    disp_x = u
    disp_y = v
    component = 0
    block = 1
    save_in = resid_x
  [../]
  [./solid_y]
    type = StressDivergenceTensors
    variable = v
    disp_x = u
    disp_y = v
    component = 1
    block = 1
    save_in = resid_y
  [../]
  [./dcdt]
    type = TimeDerivative
    variable = c
    block = 1
  [../]
  [./acintvar]
    type = ACFracIntVar
    variable = b
    block = 1
    c = c
  [../]
  [./acintcoupled]
    type = ACFracCoupledInterface
    variable = b
    c = c
    block = 1
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
    block = 1
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionPresetBC
    variable = v
    boundary = 3
    function = tfunc
  [../]
  [./yfix]
    type = PresetBC
    variable = v
    boundary = 2
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = u
    boundary = 1
    value = 0
  [../]
[]

[Materials]
  [./acbulkmat]
    type = FractureACBulkMatRate
    block = 1
  [../]
  [./acintmat]
    type = FractureACInterfaceMatRate
    block = 1
    l = 0.08
  [../]
  [./elastic]
    type = LinearElasticPFDamage
    block = 1
    c = c
    kvoid = 1e-8
    disp_y = v
    disp_x = u
    C_ijkl = '280.0 120.0 120.0 280.0 120.0 280.0 80.0 80.0 80.0'
    fill_method = symmetric9
    formulation = 0
  [../]
[]

[Postprocessors]
  [./resid_x]
    type = NodalSum
    variable = resid_x
    boundary = 3
  [../]
  [./resid_y]
    type = NodalSum
    variable = resid_y
    boundary = 3
  [../]
[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    pc_side = left
    off_diag_row='u v b c c c'
   off_diag_column='v u c b u v'
  [../]
  [./FD]
    type = FDP
    full = false
    pc_side = left
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-4
  l_max_its = 30
  nl_max_its = 15
  petsc_options = ksp_monitor
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 1
  dtmax = 100.0
  dtmin = 1e-6
  num_steps = 1
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  csv = true
  gnuplot = true
  print_linear_residuals = true
[]

