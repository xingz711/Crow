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
    scaling = 1.0e2
  [../]
  [./PolycrystalVariables]
  [../]
  [./T]
    initial_condition = 1000.0
    scaling = 1.0e8
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
  [./HtCond]
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
  [./Left_T]
    type = DirichletBC
    variable = T
    boundary = 3
    value = 1000.0
  [../]
  [./Right_T]
    type = DirichletBC
    variable = T
    boundary = 1
    value = 1025.0
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
    T = 1000
    wGB = 40 #nm
  [../]
  [./UO2]
    type = MesoUO2
    block = 0
    eta = c
    temp = T
    LANL_vals = true
    Gas_conductivity = 0.5 #W/m-K (Xe)
    GBtype = 1
    v= 'gr0 gr1'
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
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   off_diag_row = 'c w gr0 gr1'
   off_diag_column = 'w c gr1 gr0'
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value =  'asm         31   preonly   lu      1'

  l_max_its = 20
  l_tol = 1.0e-4
  nl_max_its = 50
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 2
  dt = 40.0

  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.7
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
