[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 7
  ny = 7
  xmax = 250
  ymax = 250
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1.0
  length_scale = 1.0e-9
  time_scale = 1.0e-9
  diffindex = 0.0
  bulkindex = 1.0
  polynomial_order = 6
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e4
  [../]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./InitialCondition]
    type = SmoothCircleIC
    x1 = 125.0
    y1 = 125.0
    radius = 60.0
    invalue = 1.0
    outvalue = 0.001
    int_width = 60.0
    variable = c
  [../]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 125
      y2 = 250
    [../]
  [../]
[]

[AuxVariables]
  [./T]
  [../]
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
    c = c
    mob_name = M
  [../]
  [./w_res_Tgrad]
    type = SplitCHWResTGrad
    variable = w
    c = c
    T = T
    diff_name = D
    Q_name = Qstar
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
[]

[AuxKernels]
  [./Temp]
    type = FunctionAux
    variable = T
    function = '1000.0+0.1*x'
  [../]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[Materials]
  [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T # K
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
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 2
    T = T
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
    off_diag_row = 'c w gr1 gr0'
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
  nl_rel_tol = 1.0e-10

  start_time = 0.0
  num_steps = 3
  dt = 40.0
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
