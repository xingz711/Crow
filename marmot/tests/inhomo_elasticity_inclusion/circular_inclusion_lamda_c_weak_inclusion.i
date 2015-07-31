[Mesh]
  type = FileMesh
  file = square_cross_bounds.e
  dim = 2
  uniform_refine = 0 #Change to 1 for actual verification simulations
[]

[Variables]
  active = 'disp_y disp_x'
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  active = 'stress_11 c elastic_strain11 C1111'
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
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      radius = 12.5
      int_width = 5
      x1 = 125
      y1 = 125
      outvalue = 0
      variable = c
      3D_spheres = false
      invalue = 1
      type = SmoothCircleIC
    [../]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[AuxKernels]
  active = 'stress_11 elastic_strain11 C1111'
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
  [./PressureTM]
    [./left]
      boundary = 3
      factor = -8
      disp_x = disp_x
    [../]
    [./right]
      boundary = 5
      factor = -8
      disp_x = disp_x
    [../]
  [../]
  [./horizontal]
    type = DirichletBC
    variable = disp_y
    boundary = 1
    value = 0
  [../]
  [./vertical]
    type = DirichletBC
    variable = disp_x
    boundary = 2
    value = 0
  [../]
[]

[Materials]
  [./GenIrrad]
    # T = T
    type = CuVacProps
    block = 1
    cv = c
    T = 1000 # K
    int_width = 5
    length_scale = 1.0e-9
    free_energy_form = 6
    time_scale = 1.0e-9
  [../]
  [./EigenStrainMechanics]
    type = AnisotropicEigenStrainMaterial
    block = 1
    c = c
    disp_y = disp_y
    disp_x = disp_x
    C_ijkl = '1.92e5 1.06e5 1.06e5 1.92e5 1.06e5 1.92e5 0.432e5 0.432e5 0.432e5'
    fill_method = symmetric9
    factor = 0.5
    eigenstrain_coef = '0 0 0'
    inhomogeneity = true
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./SMP]
    type = SMP
    full = true
  [../]
  [./FDP]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'

  petsc_options = '-pc_factor_shift_nonzero'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  l_max_its = 100
  l_tol = 1.0e-5
  nl_rel_tol = 1.0e-8
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
