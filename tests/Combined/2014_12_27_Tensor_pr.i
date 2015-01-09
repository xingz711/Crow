[GlobalParams]
  disp_x = disp_x
  disp_y = disp_y
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 80
  xmax = 40
  ymax = 40
  elem_type = QUAD4
[]

[Variables]
  active = 'disp_x disp_y'
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
  [./disp_x]
    block = 0
  [../]
  [./disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  [./stress_xx]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./stress_xy]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
[]

[Functions]
  [./Pres]
    type = PiecewiseLinear
    y = '0.0 0.0 55.0 55.0 0.0'
    scale_factor = 10
    x = '0.0 18.0 22.0 52.0 60.0'
  [../]
[]

[Kernels]
  active = 'TensorMechanics'
  [./CHBulk]
    type = CHChemPotential
    variable = c
    mob_name = D
    block = 0
  [../]
  [./Cdot]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./ChInt]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
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
    block = 0
  [../]
  [./stress_xy]
    type = RankTwoAux
    variable = stress_xy
    rank_two_tensor = stress
    index_j = 1
    index_i = 0
    block = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    block = 0
  [../]
[]

[BCs]
  [./Pressure]
    variable = Disp_y
    boundary = top
    function = Pres
    component = 1
    [./pres]
      function = Pres
      boundary = top
    [../]
  [../]
  [./x_0]
    type = PresetBC
    variable = disp_x
    boundary = bottom
    value = 0.0
  [../]
  [./y_0]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  active = 'Lin_Elastc'
  [./PFDiff]
    type = PFDiffusion
    block = 0
    rho = c
    kappa = 10
  [../]
  [./Lin_Elastc]
    type = LinearElasticMaterial
    block = 0
    C_ijkl = '1.0e6 0.0 0.0 1.0e6  0.0 1.0e6 0.5e6 0.5e6 0.5e6'
  [../]
[]

[Executioner]
  type = Transient
  dt = 2
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  nl_abs_tol = 1e-9
  end_time = 60
  l_tol = 1e-04
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  exodus = true
  output_final = true
  [./console]
    type = Console
    perf_log = true
    output_initial = true
    output_file = true
  [../]
[]

