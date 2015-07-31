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
  int_width = 40.00
  nbub = 1
  theta = 65
  polynomial_order = 8
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e4
  [../]
  [./gr0]
    scaling = 1e2
  [../]
  [./gr1]
    scaling = 1e2
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
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    type = SplitCHWResGBSurfDiff
    variable = w
    diffindex = .5
    gbindex = 1.0
    bulkindex = .1
    c = c
    mob_name = M
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
  [./c_res_GB]
    # GB contribution to buble
    type = SplitCHGBPoly
    variable = c
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
    derivative_order = 2
    T = 500
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
  [./SMP]
    type = SMP
    off_diag_row = 'c w gr0 gr1'
    off_diag_column = 'w c gr0 gr1'
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 10
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-8

  start_time = 0.0
  num_steps = 2
  dt = 30.0
  nl_abs_tol = 1e-9
[]

[Outputs]
  file_base = mo_he
  output_initial = true
  exodus = true
  print_perf_log = true
[]
