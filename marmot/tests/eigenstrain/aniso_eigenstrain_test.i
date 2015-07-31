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
  [./c_IC]
    type = SmoothCircleIC
    x1 = 125.0
    y1 = 125.0
    radius = 30.0
    invalue = 1.0
    outvalue = 0.1
    int_width = 40
    variable = c
  [../]
[]

[AuxVariables]
  [./elastic_strain11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./C1111]
    order = CONSTANT
    family = MONOMIAL
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
    mob_name = M
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./eigenstrain]
    type = SplitCHLinearElasticity
    variable = c
  [../]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[AuxKernels]
  [./elastic_strain11]
    type = RankTwoAux
    variable = elastic_strain11
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 0
    execute_on = timestep_end
  [../]
  [./stress_11]
    type = RankTwoAux
    variable = stress_11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    execute_on = timestep_end
  [../]
  [./C1111]
    type = RankFourAux
    variable = C1111
    rank_four_tensor = elasticity_tensor
    index_l = 0
    index_j = 0
    index_k = 0
    index_i = 0
    execute_on = timestep_end
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
    type = AnisotropicEigenStrainMaterial
    block = 0
    c = c
    disp_y = disp_y
    disp_x = disp_x
    C_ijkl = '1.92e5 1.06e5 1.06e5 1.92e5 1.06e5 1.92e5 0.432e5 0.432e5 0.432e5'
    fill_method = symmetric9
    factor = 0.95
    eigenstrain_coef = '-0.1 0.05 0.0'
    inhomogeneity = true
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
    off_diag_row = 'c w'
    off_diag_column = 'w c'
  [../]
[]

[Executioner]
  type = Transient
  dt = 1
  scheme = bdf2

  solve_type = 'PJFNK'
  petsc_options = '-pc_factor_shift_nonzero'
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap -mat_fd_coloring_err -mat_fd_type'
  petsc_options_value = 'asm         31              preonly           lu                2               1e-6         ds'

  l_max_its = 100
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-8

  start_time = 0.0
  num_steps = 1
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
