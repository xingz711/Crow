[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 15
  ny = 15
  nz = 0
  xmin = 0
  xmax = 30
  ymin = 0
  ymax = 30
  zmin = 0
  zmax = 0
  elem_type = QUAD4

   #uniform_refine = 3
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
[]

[Variables]

  [./PolycrystalVariables]
    order = THIRD
    family = HERMITE
  [../]

  [./cg]
    order = THIRD
    family = HERMITE
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

  [./cg]
    type = RndSmoothCircleIC
    variable = cg
    invalue = 1.0
    variation_invalue = 0.0
    outvalue = 0.1
    variation_outvalue = 0.05
    radius = 5.0
    x1 = 15.0
    y1 = 15.0
    int_width = 5.0
  [../]
[]

[AuxVariables]
active = ''

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
    type = TimeDerivative
    variable = cg
  [../]

  [./cg_bulk]
    type = CHVacGB
    variable = cg
    mob_name = Dg
  [../]

  [./cg_interface]
    type = CHInterface
    variable = cg
    kappa_name = kappa
    mob_name = Dg
    grad_mob_name = grad_Dg
  [../]

[]

[AuxKernels]
active = ''

  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[BCs]

  [./penaltycg]
    type = CHRadPenaltyBC
    variable = cg
    boundary = '0 1 2 3 '
  [../]

  [./penaltygr0]
    type = CHRadPenaltyBC
    variable = gr0
    boundary = '0 1 2 3 '
  [../]

  [./penaltygr1]
    type = CHRadPenaltyBC
    variable = gr1
    boundary = '0 1 2 3 '
  [../]

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
active = 'dt'

  [./dt]
    type = TimestepSize
  [../]

  [./gr_area1]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]

  [./gr_area2]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
  [../]

[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20

  nl_max_its = 30
  nl_abs_tol = 3e-9

  start_time = 0.0
  num_steps = 1
  dt = 1.0e-1

 # [./TimeStepper]
 #   type = FunctionDT
 #   time_t = '0 .001'
 #   time_dt = '0.001 0.005'
 # [../]

[]

[Outputs]
  file_base = out
  interval = 1
  exodus = true
  output_initial = true
  print_perf_log = true
[]



