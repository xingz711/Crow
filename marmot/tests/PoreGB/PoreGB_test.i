[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 9
  ny = 9
  nz = 0
  xmin = 0
  xmax = 250
  ymin = 0
  ymax = 250
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1
  radius = 60.0
  int_width = 40.0
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = BubblesBicrystalIC
      c_outvalue = 0.01
      nbub = 1
      phase = 2
      theta = 65
    [../]
  [../]
  [./w]
    scaling = 1.0e4
  [../]
  [./gr0]
    scaling = 1e2
    [./InitialCondition]
      type = BubblesBicrystalIC
      nbub = 1
      phase = 0
      theta = 65
    [../]
  [../]
  [./gr1]
    scaling = 1e2
    [./InitialCondition]
      type = BubblesBicrystalIC
      nbub = 1
      phase = 1
      theta = 50
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./free_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHPoreGB
    variable = c
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    c = c
    mob_name = M_c
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
  [./free_energy]
    type = PoreGBFreeEnergyCalculation
    variable = free_energy
    c = c
    execute_on = timestep_end
  [../]
[]

[BCs]
[]

[Materials]
  [./MoHe]
    type = PoreGBMaterial
    block = 0
    T = 500.0
    int_width = 40.0
    time_scale = 1
    D0 = 3.1376e-07
    Em = 0.94498
    GB_energy = 2.4
    GBmob0 = 3.986e-6
    Q = 1.0307
  [../]
[]

[Postprocessors]
  [./totalc]
    type = ElementIntegralVariablePostprocessor
    variable = c
    execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./PBP]
    type = PBP
    solve_order = 'w c gr0 gr1'
    preconditioner = 'AMG ASM AMG AMG'
    off_diag_row = 'c '
    off_diag_column = 'w '
  [../]
  [./SMP]
    # full = true
    type = SMP
    off_diag_row = 'c w gr0 gr1'
    off_diag_column = 'w c gr0 gr1'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'
  l_max_its = 10
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  start_time = 0.0
  num_steps = 2
  dt = 30.0
  nl_abs_tol = 1e-9
  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]
