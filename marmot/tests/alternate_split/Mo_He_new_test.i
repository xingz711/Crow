[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 9
  ny = 9
  xmax = 250
  ymax = 250
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 0.75
  radius = 60.0
  int_width = 40.0
  nbub = 1
  theta = 65
  polynomial_order = 8
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
  [./gr0]
  [../]
  [./gr1]
  [../]
[]

[ICs]
  [./c_IC]
    type = BubblesBicrystalIC
    c_outvalue = 0.01
    nbub = 1
    phase = 2
    variable = c
  [../]
  [./gr0_IC]
    type = BubblesBicrystalIC
    nbub = 1
    phase = 0
    variable = gr0
  [../]
  [./gr1_IC]
    type = BubblesBicrystalIC
    nbub = 1
    phase = 1
    variable = gr1
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
[]

[Kernels]
  [./time]
    type = TimeDerivative
    variable = c
  [../]
  [./c_res]
    type = CHParsed
    variable = c
    f_name = F
  [../]
  [./c_res_GB]
    type = CHGBPoly
    variable = c
  [../]
  [./w_res]
    type = CHSplitVar
    variable = w
    c = c
  [../]
  [./w]
    type = Reaction
    variable = w
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
  [./c_res_interface]
    type = CHInterfaceSplit
    variable = c
    w = w
  [../]
[]

[Functions]
  [./Temp_grad]
    type = ParsedFunction
    value = 1000.0+0.1*x
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[BCs]
[]

[Materials]
  [./Moly_GB]
    type = GBEvolution
    block = 0
    time_scale = 1
    GBmob0 = 3.986e-6
    T = 500
    wGB = 40
    Q = 1.0307
    GBenergy = 2.4
  [../]
  [./He_in_Mo]
    type = PFParamsPolyFreeEnergy
    block = 0
    Em = .94498
    c = c
    T = 500
    Ef = 2.6
    time_scale = 1.0
    surface_energy = 3.0
    D0 = 3.1376e-07
    int_width = 40
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 3
    T = 500
  [../]
[]

[Postprocessors]
  [./totalc]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_row = 'c w '
    off_diag_column = 'w c'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 10
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-8

  start_time = 0.0
  num_steps = 2
  dt = 30.0

  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = mo_he
  output_initial = true
  exodus = true
  print_perf_log = true
[]
