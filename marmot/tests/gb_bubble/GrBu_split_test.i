[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  nz = 0
  xmin = 0
  xmax = 30
  ymin = 0
  ymax = 30
  zmin = 0
  zmax = 0
  elem_type = QUAD4

   #uniform_refine = 1
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
[]

[Variables]

  [./PolycrystalVariables]
  [../]

  [./cg]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      invalue = 1.0
      outvalue = 0.025
      radius = 5.0
      x1 = 15.0
      y1 = 15.0
      int_width = 3.0
    [../]
  [../]

  [./w]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0e3
  [../]

[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 15
      y2 = 30
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

  [./PolycrystalBubbleKernel]
    cg = cg
  [../]

  [./cg_dot]
    type = CoupledImplicitEuler
    variable = w
    v = cg
  [../]

  [./c_res]
    type = CHGrBu
    variable = cg
    kappa_name = kappa
    w = w
  [../]

  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = Dg
    c = cg
  [../]

[]

[AuxKernels]

  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
[]

[BCs]

[]

[Materials]
  active = 'GeneralGrBu'

  [./GeneralGrBu]
    type = GeneralGrBu
    block = 0
    Dg = 1.83
    L = 10.0
    Ao = 4.0
    BetaS  = 1.7
    BetaGB = 0.7
    kappSS = 3.4
    kappGB = 1.4
    cg = cg
  [../]
[]

[Postprocessors]

[]

[Preconditioning]
#active = ''
  [./SMP]
   type = SMP
    off_diag_row = 'w cg'
    off_diag_column = 'cg w'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options = '-pc_factor_shift_nonzero'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-10
  start_time = 0.0
  num_steps = 2
  dt = 0.3

[]

[Outputs]
  file_base = GrBu_split
  interval = 1
  exodus = true
  output_initial = true
  print_perf_log = true
[]



