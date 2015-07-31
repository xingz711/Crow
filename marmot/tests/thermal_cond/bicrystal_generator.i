[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 10
  xmax = 1500.0
  ymax = 750.0
  uniform_refine = 1
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 0.8
  void_num = 4
  min_void_dist = 200
  max_void_radius = 80
  min_void_radius = 80
  ICType = 2
  polynomial_order = 8
  length_scale = 1.0e-9
  time_scale = 1.0e-9
[]

[Variables]
  [./c]
  [../]
  [./w]
    scaling = 1.0e4
  [../]
  [./PolycrystalVoidVariables]
    max_void_radius = 80
    min_void_radius = 80
  [../]
[]

[ICs]
  [./c_IC]
    type = PolycrystalVoidIC
    outvalue = 0.01
    variable = c
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
    diffindex = 0.0
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
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
      variable = 'gr0 gr1 c w'
    [../]
  [../]
[]

[Materials]
  [./Copper]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = 1000 # K
    int_width = 64.0
    D0 = 3.1e-5 # m^2/s, from Brown1980
    Em = 0.71 # in eV, from Balluffi1978 Table 2
    Ef = 1.28 # in eV, from Balluffi1978 Table 2
    surface_energy = 0.708 # Total guess
  [../]
  [./CuGrGr]
    T = 500 # K
    type = CuGrGr
    block = 0
    wGB = 64.0 # nm
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_row = 'c w'
    off_diag_column = 'w c'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 15
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 2
  dt = 36

  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.7
    coarsen_fraction = .1
    max_h_level = 2
    weight_names = 'gr0 gr1 c w'
    weight_values = '1 1 1 0'
  [../]
[]

[Outputs]
  file_base = bicrystal_gen
  exodus = true
  print_perf_log = true
  [./xda]
    file_base = bicrystal_gen
    type = XDA
    interval = 2
  [../]
[]
