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
  en_ratio = 0.7
  polynomial_order = 6
  length_scale = 1.0e-9
  time_scale = 1.0e-9
[]

[Variables]
  [./c]
  [../]
  [./w]
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

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
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
  [./free_energy]
    type = PolynomialFreeEnergy
    block = 0
    c = c
    derivative_order = 2
    T = 1000
  [../]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 1000 # K
    wGB = 40 # nm
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_row = 'w c'
    off_diag_column = 'c w'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'PJFNK'
  petsc_options = '-pc_factor_shift_nonzero'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 2
  dt = 50
[]

[Outputs]
  output_initial = true
  file_base = split
  interval = 1
  exodus = true
  print_perf_log = true
[]
