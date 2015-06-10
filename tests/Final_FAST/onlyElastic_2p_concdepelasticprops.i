[GlobalParams]
  var_name_base = gr
  op_num = 2.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 30
  nz = 0
  xmin = 0.0
  xmax = 30.0
  ymin = 0.0
  ymax = 15.0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./disp_x]
    block = 0
  [../]
  [./disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  [./e11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./e22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./C11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -1.0 -1.0 0.0'
    x = '0.0 15.0 45.0 60.0'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_y = disp_y
    disp_x = disp_x
  [../]
[]

[AuxKernels]
  [./e11]
    type = RankTwoAux
    variable = e11
    rank_two_tensor = total_strain
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./e22]
    type = RankTwoAux
    variable = e22
    rank_two_tensor = total_strain
    index_j = 1
    index_i = 1
    block = 0
  [../]
  [./S11]
    type = RankTwoAux
    variable = S11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./S22]
    type = RankTwoAux
    variable = S22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    block = 0
  [../]
  [./C11]
    type = RankFourAux
    variable = C11
    index_i = 0
    index_j = 0
    index_k = 0
    index_l = 0
    block = 0
    rank_four_tensor = elasticity_tensor
  [../]
  [./S]
    type = RankTwoScalarAux
    variable = S
    scalar_type = VonMisesStress
    rank_two_tensor = stress
[]

[BCs]
  [./Disp_x]
    type = PresetBC
    variable = disp_x
    boundary = 'right left'
    value = 0.0
  [../]
  [./Disp_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
  [./Load]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = load
  [../]
[]

[Materials]
  [./ElasticityTensor]
    type = ComputeConcentrationDependentElasticityTensor
    block = 0
    c = c
    C1_ijkl = '30.141 35.46'
    C0_ijkl = '10.0 10.0'
    fill_method1 = symmetric_isotropic
    fill_method0 = symmetric_isotropic
    outputs = console
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    disp_y = disp_y
    disp_x = disp_x
    outputs = console
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
    outputs = console
  [../]
[]

[Postprocessors]
  [./load]
    type = PlotFunction
    function = load
  [../]
  [./s11]
    type = ElementAverageValue
    variable = S11
  [../]
  [./s22]
    type = ElementAverageValue
    variable = S22
  [../]
  [./e11]
    type = ElementAverageValue
    variable = e11
  [../]
  [./e22]
    type = ElementAverageValue
    variable = e22
  [../]
[]

[Debug]
  show_var_residual_norms = true

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.05
  num_steps = 10000
  end_time = 60

[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  interval = 10
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
    file_base = comb_multip
  [../]ss_check_tol = 1e-08
  print_linear_residuals = true
[]

[ICs]
  [./multip]
    x_positions = '15.0 15.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '0.0 15.0 '
    radii = '8.0 8.0 '
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]
