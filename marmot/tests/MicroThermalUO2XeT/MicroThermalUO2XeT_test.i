[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  xmax = 250
  ymax = 250
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 0.75
  radius = 40.0
  int_width = 40.0
  theta = 65
  length_scale = 1.0e-9
  time_scale = 1.0e-9
  polynomial_order = 8
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e2
  [../]
  [./gr0]
  [../]
  [./gr1]
  [../]
  [./T]
    scaling = 1e8
    initial_condition = 800
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
    diffindex = 1.0
    gbindex = 1.0
    bulkindex = 1.0
    c = c
    mob_name = M
  [../]
  [./chGB]
    type = SplitCHGBPoly
    variable = c
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
  [./heat_cond]
    type = HeatConduction
    variable = T
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[BCs]
  [./T_left]
    type = PresetBC
    variable = T
    boundary = left
    value = 800
  [../]
  [./flux_right]
    type = NeumannBC
    variable = T
    boundary = right
    value = -4.3e-11
  [../]
[]

[Materials]
  [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = 1000 # K
    int_width = 40.0
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
  [../]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 1000 #K
    wGB = 40 #nm
  [../]
  [./thermal]
    type = MicroThermalUO2XeT
    block = 0
    GB_conductivity = 1.0
    eta = c
    model_type = MD
    T = 300.0
    conc_Xe = 0.01
    outputs = exodus
  [../]
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 2
    T = 1000
  [../]
[]

[Postprocessors]
  [./totalc]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
  [./thcond]
    type = ThermalCond
    variable = T
    flux = 4.3e-11
    length_scale = 1e-09
    T_hot = 800
    dx = 250
    boundary = right
  [../]
  [./rprime]
    type = EffectiveKapitzaResistance
    eff_cond = thcond
    ko = 4.5
    dx = 250
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

  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 50
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 1
  dt = 10.0
[]

[Outputs]
  interval = 1
  exodus = true
  print_perf_log = true
[]
