[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmin= -0.5
  ymin= -0.5
  xmax = 0.5
  ymax = 0.5
  zmax = 0
  elem_type = QUAD4
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
  [./porosity]
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
[]

[Functions]
  [./tfunc]
    type = ParsedFunction
    value = t
  [../]
  [./porosity_func]
    type = ParsedFunction
    value = 'exp(-pow(x*x+y*y,0.5))'
  [../]
[]

[Kernels]
  [./acbulk]
    type = ACFracBulkRate
    variable = c
    block = 0
    l = .02
    beta = b
    visco =1.0
    gc_tens = 1e-3
    gc_comp = 1000
  [../]
  [./solid_x]
    type = StressDivergenceTensorsPFFrac
    variable = u
    disp_x = u
    disp_y = v
    component = 0
    block = 0
    save_in = resid_x
    c=c
  [../]
  [./solid_y]
    type = StressDivergenceTensorsPFFrac
    variable = v
    disp_y = v
    disp_x = u
    component = 1
    block = 0
    save_in = resid_y
    c=c
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
  [./porosity]
    type = FunctionAux
    variable = porosity
    function = porosity_func
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
  [./acbulkmat]
    type = FractureACBulkMatRate
    block = 0
  [../]
  [./acintmat]
    type = FractureACInterfaceMatRate
    block = 0
    l = 0.02
  [../]
  [./elastic]
    type = LinearElasticPFDamagePorosity
    block = 0
    c = c
    kvoid = 1e-8
    disp_y = v
    disp_x = u
    C_ijkl = '280.0 120.0 120.0 280.0 120.0 280.0 80.0 80.0 80.0'
    fill_method = symmetric9
    formulation = 0
    porosity = porosity
  [../]
[]

[Postprocessors]
  [./resid_x]
    type = NodalSum
    variable = resid_x
    boundary = top
  [../]
  [./resid_y]
    type = NodalSum
    variable = resid_y
    boundary = top
  [../]
[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    off_diag_row='u u v v b c c c'
    off_diag_column='v c u c c b u v'
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
  nl_max_its = 20
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
