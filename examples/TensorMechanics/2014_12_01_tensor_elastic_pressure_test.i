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
  # Mesh
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  displacements = 'disp_x disp_y'
  elem_type = QUAD4
[]

[AuxVariables]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_xy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./Stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  # Functions
  active = 'zeroRamp'
  [./rampConstant]
    type = PiecewiseLinear
    x = '0. 1. 2.'
    y = '0. 1. 1.'
    scale_factor = 1.0
  [../]
  [./zeroRamp]
    type = PiecewiseLinear
    x = '0.0 5.0 10.0'
    y = '0.0 30.0 60.0'
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
  # Variables
  active = 'disp_y disp_x'
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
[]

[Kernels]
  [./TensorMechanics]
  [../]
[]

[AuxKernels]
  [./stress_xx]
    type = RankTwoAux
    variable = stress_xx
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    variable = Stress_yy
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
  [../]
  [./stress_xy]
    type = RankTwoAux
    variable = stress_xy
    rank_two_tensor = stress
    index_j = 1
    index_i = 0
  [../]
[]

[BCs]
  # BCs
  active = 'Pressure no_x no_y'
  [./no_x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0.0
  [../]
  [./no_y]
    type = DirichletBC
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
  [./Pressure]
    active = 'Side2'
    [./Side1]
      boundary = 1
      function = rampConstant
    [../]
    [./Side2]
      boundary = 2
      function = zeroRamp
    [../]
    [./Side3]
      boundary = 3
      function = rampUnramp
    [../]
  [../]
[]

[Materials]
  # Materials
  [./stiffStuff]
    type = LinearElasticMaterial
    block = 0
    C_ijkl = '1.0e6 0.0 0.0 1.0e6  0.0 1.0e6 0.5e6 0.5e6 0.5e6'
    T0 = 700
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  # Executioner
  type = Transient
  solve_type = PJFNK
  nl_abs_tol = 1e-10
  l_max_its = 20
  start_time = 0.0
  dt = 1.0
  end_time = 10
[]

[Outputs]
  # Outputs
  output_initial = true
  print_linear_residuals = true
  print_perf_log = true
  [./out]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]

