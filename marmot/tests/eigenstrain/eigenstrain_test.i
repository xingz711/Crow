[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 12
  ny = 12
  xmax = 250
  ymax = 250
  elem_type = QUAD4
  uniform_refine = 1
[]

[GlobalParams]
  polynomial_order = 6
  length_scale = 1.0e-9
  time_scale = 1.0e-9
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[ICs]
  [./InitialCondition]
    type = SmoothCircleIC
    x1 = 125.0
    y1 = 125.0
    radius = 60.0
    invalue = 1.0
    outvalue = 0.1
    int_width = 50.0
    variable = c
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
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./eigenstrain]
    type = SplitCHEigenStrain
    variable = c
    epsilon0 = 0.05
  [../]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[BCs]
  [./bottom]
    type = PresetBC
    boundary = bottom
    variable = disp_y
    value = 0.
  [../]
  [./left]
    type = PresetBC
    boundary = left
    variable = disp_x
    value = 0.
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
  [./EigenStrainMechanics]
    type = SimpleEigenStrainMaterial
    block = 0
    epsilon0 = 0.05
    c = c
    disp_y = disp_y
    disp_x = disp_x
    C_ijkl = '3 1 1 3 1 3 1 1 1 '
    fill_method = symmetric9
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

  solve_type = 'PJFNK'
  petsc_options = '-pc_factor_shift_nonzero'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10

  start_time = 0.0
  num_steps = 2
  dt = 25.0
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
