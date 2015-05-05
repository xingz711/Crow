# 
# Pressure Test
# 
# This test is designed to compute pressure loads on three faces of a unit cube.
# 
# The mesh is composed of one block with a single element.  Symmetry bcs are
# applied to the faces opposite the pressures.  Poisson's ratio is zero,
# which makes it trivial to check displacements.
# 

[GlobalParams]
  disp_x = disp_x
  disp_y = disp_y
[]

[Mesh]
  # Comment
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 30
  xmax = 30
  ymax = 15
  zmax = 0
  displacements = 'disp_x disp_y '
  elem_type = QUAD4
[]

[Functions]
  [./rampConstant]
    type = PiecewiseLinear
    x = '0. 1. 2.'
    y = '0. 1. 1.'
    scale_factor = 1.0
  [../]
  [./zeroRamp]
    type = PiecewiseLinear
    x = '0. 1. 2. 50.'
    y = '0. 0. 1. 1.'
    scale_factor = 2.0
  [../]
  [./rampUnramp]
    type = PiecewiseLinear
    x = '0. 1. 2.'
    y = '0. 1. 0.'
    scale_factor = 10.0
  [../]
[]

[Variables]
  active = 'disp_y disp_x c w'
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]
  [./c]
    block = 0
  [../]
  [./w]
    block = 0
  [../]
[]

[Kernels]
  active = 'TensorMechanics Time w_res cres'
  [./TensorMechanics]
  [../]
  [./c_res]
    type = SplitCHMath
    variable = c
    kappa_name = kappa_c
    w = w
    block = 0
  [../]
  [./Time]
    type = CoupledImplicitEuler
    variable = w
    block = 0
    v = c
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
    block = 0
  [../]
  [./cres]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
    block = 0
  [../]
[]

[BCs]
  active = 'Periodic PressureTM no_x no_y'
  [./no_x]
    type = PresetBC
    variable = disp_x
    boundary = 'right left bottom'
    value = 0.0
  [../]
  [./no_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
  [./no_z]
    type = DirichletBC
    variable = disp_z
    boundary = 6
    value = 0.0
  [../]
  [./PressureTM]
    active = 'Side2'
    [./Side1]
      boundary = 1
      function = rampConstant
    [../]
    [./Side2]
      boundary = top
      function = rampUnramp
    [../]
    [./Side3]
      boundary = 3
      function = rampUnramp
    [../]
  [../]
  [./Periodic]
    [./periodic]
      variable = 'c w'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  active = 'CH_mat sintering lin_elastic'
  [./Elasticity_tensor]
    type = ComputeElasticityTensor
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '0.27 390e9'
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
  [../]
  [./Eigen]
    type = SimpleEigenStrainMaterial
    block = 0
    c = c
    fill_method = symmetric_isotropic
    epsilon0 = 0.05
    C_ijkl = '0.27 390e3'
    elasticity_tensor_prefactor = rampConstant
  [../]
  [./CH_mat]
    type = PFMobility
    block = 0
    mob = 0.5
  [../]
  [./sintering]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 0
    third_derivatives = false
  [../]
  [./lin_elastic]
    type = LinearElasticMaterial
    C_ijkl = '0.27 390e9'
    fill_method = symmetric_isotropic
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  solve_type = NEWTON
  nl_abs_tol = 1e-10
  l_max_its = 20
  start_time = 0.0
  dt = 0.05
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 50.0
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  print_linear_residuals = true
  print_perf_log = true
  csv = true
  [./console]
    type = Console
  [../]
  [./out]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]

[ICs]
  [./IC_c]
    op_num = 2.0
    radius = '5.0 5.0'
    variable = c
    type = TwoParticleDensityIC
    block = 0
  [../]
[]

