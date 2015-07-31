[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  nz = 0
  xmin = 0
  xmax = 250
  ymin = 0
  ymax = 250
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1
  radius = 30.0
  int_width = 30.0
  nbub = 1
  theta = 75
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = BubblesBicrystalIC
      c_outvalue = 0.01
      phase = 2
    [../]
  [../]
  [./w]
  [../]
  [./gr0]
    scaling = 1e2
    [./InitialCondition]
      type = BubblesBicrystalIC
      phase = 0
    [../]
  [../]
  [./gr1]
    scaling = 1e2
    [./InitialCondition]
      type = BubblesBicrystalIC
      phase = 1
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
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
    type = SplitCHWResMtrxMob
    variable = w
    c = c
    mob_name = M_c
    dmob_name = dMdc
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
  [./PolyGenericDFKernel]
    DF = '2.22222222222222e7 0 0'
    c = c
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
[]

[Materials]
  [./MoHe]
    type = PoreGBMtrxMobMaterial
    block = 0
    T = 500.0
    int_width = 30.0
    time_scale = 1
    D0 = 3.1376e-07
    Em = 0.94498
    GB_energy = 2.4
    GBmob0 = 3.986e-6
    Q = 1.0307
    c = c
    bulkindex = 0
    surfindex = 1.0
    gbindex = 2.0
  [../]
[]

[Preconditioning]
  active = 'SMP'
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
  l_max_its = 30
  l_tol = 1.0e-3
  nl_max_its = 30
  nl_rel_tol = 1.0e-9
  start_time = 0.0
  num_steps = 3
  dt = 0.9
  nl_abs_tol = 1e-9
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]

