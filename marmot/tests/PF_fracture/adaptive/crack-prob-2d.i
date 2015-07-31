[Mesh]
  type = FileMesh
  file = crack-mesh.e
[]

[Variables]
  [./disp_x]
    block = 1
  [../]
  [./disp_y]
    block = 1
  [../]
  [./c]
    block = 1
  [../]
  [./b]
    block = 1
    #scaling = 1e3
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
  [./strain_yy]
    order = CONSTANT
    family = MONOMIAL
    block = 1
  [../]
  [./G0_pos]
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
    l = .02
    beta = b
    visco = 0.05
    gc_tens = 2.7e-3
    gc_comp = 1000
  [../]
  [./solid_x]
    type = StressDivergenceTensors
    variable = disp_x
    disp_x = disp_x
    component = 0
    block = 1
    save_in = resid_x
    c = c
  [../]
  [./solid_y]
    type = StressDivergenceTensors
    variable = disp_y
    disp_y = disp_y
    component = 1
    block = 1
    save_in = resid_y
    c = c
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
    index_j = 2
    index_i = 2
    execute_on = timestep_end
    block = 1
  [../]
  [./strain_yy]
    type = RankTwoAux
    variable = strain_yy
    rank_two_tensor = elastic_strain
    index_j = 2
    index_i = 2
    execute_on = timestep_end
    block = 1
  [../]
  [./G0_pos]
    type = MaterialRealAux
    variable = G0_pos
    property = G0_pos
    execute_on = timestep_end
    block = 1
  [../]
[]

[BCs]
  [./cbcdrichlet]
    type = PresetBC
    variable = c
    boundary = 4
    value = 1
  [../]
  [./ydisp]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 3
    function = tfunc
  [../]
  [./yfix]
    type = PresetBC
    variable = disp_y
    boundary = 2
    value = 0
  [../]
  [./xfix]
    type = PresetBC
    variable = disp_x
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
    l = 0.02
  [../]
  [./elastic]
    type = LinearElasticPFDamage
    block = 1
    c = c
    kvoid = 1e-8
    disp_y = disp_y
    disp_x = disp_x
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
    full=true
  [../]
  [./FD]
    type = FDP
    full = true
    pc_side = left
  [../]
[]

[Executioner]
  type = Transient
  dt = 1e-4
  l_max_its = 50
  nl_max_its = 20
  petsc_options = ksp_monitor
  solve_type = PJFNK
  #petsc_options_iname = -pc_type
  #petsc_options_value = hypre
  #petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -mat_mffd_type'
  #petsc_options_value = 'hypre boomeramg 101 ds'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  #nl_abs_tol = 1e-4
  #nl_rel_step_tol = 1e-4
  end_time = 1
  nl_abs_step_tol = 1e-4
  dtmax = 100.0
  l_tol = 1e-4
  nl_rel_tol = 1e-4
  #ss_check_tol = 1e-4
  dtmin = 1e-8
  num_steps = 1000
  [./Adaptivity]
    initial_adaptivity = 0
    refine_fraction = .8
    coarsen_fraction = 0.04
    max_h_level = 1
    weight_names = 'disp_x disp_y c b'
    weight_values = '1 1 1 0.0'
  [../]
[]

[Outputs]
  output_initial = true
  exodus = true
  csv = true
  gnuplot = true
  print_perf_log = true
[]
