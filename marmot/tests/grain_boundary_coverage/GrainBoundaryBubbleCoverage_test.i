[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 8
  ny = 8
  xmax = 250
  ymax = 250
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1
  theta = 80
  radius = 30.0
  int_width = 40.0
  polynomial_order = 8
  length_scale = 1.0e-9
  time_scale = 1.0e-9
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e3
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
    c_outvalue = 0.01
    nbub = 1
    phase = 0
    variable = gr0
  [../]
  [./gr1_IC]
    type = BubblesBicrystalIC
    c_outvalue = 0.01
    nbub = 1
    phase = 1
    variable = gr1
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./GB]
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
    # GB contribution to buble
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
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
  [./GBcalc]
    type = GBCalcAux
    variable = GB
    execute_on = timestep_end
    GBthreshold = 0.9
  [../]
[]

[BCs]
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
    T = 1000 # K
    wGB = 40 # nm
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
  [./porosity]
    type = Porosity
    variable = c
  [../]
  [./numbub]
    type = FeatureFloodCount
    variable = c
  [../]
  [./GBarea]
    type = ElementIntegralVariablePostprocessor
    variable = GB
  [../]
  [./GBcoverage]
    type = GrainBoundaryBubbleCoverage
    numbub_name = numbub
    GBwidth = 40
    GBarea_name = GBarea
    porosity_name = porosity
    Lx = 250
    Ly = 250
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
  scheme = bdf2

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 3
  dt = 40.0

  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.6
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = GrainBoundaryBubbleCoverage_out
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
