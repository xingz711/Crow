[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 5
  nz = 0
  xmax = 1000
  ymax = 1000
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  wGB = 60 # interfacial width in nm
[]

[Functions]
  [./TGradient]
    type = ParsedFunction
    value = '450 + 0.01*x'
  [../]
[]

[Variables]
  [./PolycrystalVariables]
    scaling = 1e2
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 500
      y2 = 1000
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./T]
    order = FIRST
    family = LAGRANGE
    initial_condition = 450
  [../]
  [./elastic_strain11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./elastic_strain22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./elastic_strain12]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
  [../]
  [./PolycrystalElasticDrivingForce]
  [../]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]
  [./Tgrad]
    type = FunctionAux
    variable = T
    function = TGradient
  [../]
  [./elastic_strain11]
    type = RankTwoAux
    variable = elastic_strain11
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 0
    execute_on = timestep_end
  [../]
  [./elastic_strain22]
    type = RankTwoAux
    variable = elastic_strain22
    rank_two_tensor = elastic_strain
    index_i = 1
    index_j = 1
    execute_on = timestep_end
  [../]
  [./elastic_strain12]
    type = RankTwoAux
    variable = elastic_strain12
    rank_two_tensor = elastic_strain
    index_i = 0
    index_j = 1
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./x_anchor]
    type = PresetBC
    variable = disp_x
    boundary = 'left right'
    value = 0.0
  [../]
  [./y_anchor]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = T # K
  [../]
  [./ElasticityTensor]
    type = ComputePolycrystalElasticityTensor
    block = 0
    fill_method = symmetric9
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    Elastic_constants = '1.27e5 0.708e5 0.708e5 1.27e5 0.708e5 1.27e5 0.7355e5 0.7355e5 0.7355e5'
    GrainTracker_object = grain_tracker
    euler_angle_provider = euler_angle_file
    grain_num = 2
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y'
    thermal_expansion_coeff = 17.0e-6
    temperature = T
    temperature_ref = 273
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
  [../]
[]

[UserObjects]
  [./grain_tracker]
    type = GrainTracker
    threshold = 0.1
    convex_hull_buffer = 5.0
    use_single_map = false
    enable_var_coloring = true
    condense_map_info = true
    connecting_threshold = 0.05
    compute_op_maps = true
    execute_on = TIMESTEP_BEGIN
    flood_entity_type = ELEMENTAL
  [../]
  [./euler_angle_file]
    type = EulerAngleFileReader
    file_name = test.tex
  [../]
[]

[Postprocessors]
  [./area_grain0]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_row = 'gr0 gr1 disp_x disp_y'
    off_diag_column = 'gr1 gr0 disp_y disp_x'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 20
  nl_rel_tol = 1.0e-9
  start_time = 0.0
  num_steps = 6
  dt = 60
  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.8
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  exodus = true
  csv = true
  output_initial = true
  print_perf_log = true
[]
