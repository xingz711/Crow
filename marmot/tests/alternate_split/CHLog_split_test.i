[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmax = 25
  ymax = 25
  zmax = 25
  elem_type = QUAD4
  uniform_refine = 1
[]

[Variables]
  [./cv]
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 12.5
      y1 = 12.5
      radius = 4.0
      invalue = 1.0
      outvalue = 0.1
      int_width = 3.0
    [../]
  [../]
  [./w]
  [../]
[]

[Kernels]
  [./ie_c]
    type = TimeDerivative
    variable = cv
  [../]
  [./CHSolid]
    type = CHPhys
    variable = cv
    mob_name = Ms_v
  [../]
  [./CHInterface]
    type = CHInterfaceSplit
    variable = cv
    mob_name = M_v
    kappa_name = kappa_v
    w = w
  [../]
  [./w]
    type = Reaction
    variable = w
  [../]
  [./w_def]
    type = CHSplitVar
    variable = w
    c = cv
  [../]
[]

[BCs]
  [./Periodic]
    [./left_right]
      primary = 0
      secondary = 2
      translation = '0 25 0'
    [../]
    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-25 0 0'
    [../]
  [../]
[]

[Materials]
  [./GenIrrad]
    type = CTempIrrad
    block = 0
    cv = cv
    T = 1150 # K
    int_width = 20.0 # nm
    free_energy_form = 0
    D0vac = 2.21e4
    Emvac = 1.0
    Efvac = 1.0
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

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options = '-pc_factor_shift_nonzero '
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'
  #petsc_options = '-snes_mf_operator '
  #petsc_options_iname = '-pc_type '
  #petsc_options_value = lu
  l_max_its = 10
  l_tol = 1e-4
  nl_max_its = 10
  nl_rel_tol = 1e-11
  start_time = 0.0
  num_steps = 12
  dt = 0.1
  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = phys
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

