[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 3
  ny = 3
  nz = 0
  xmax = 400 # nm
  ymax = 400 # nm
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 1
  displacements = 'disp_x disp_y'
[]

[Functions]
  [./top_tensile]
    type = ParsedFunction
    value = -0.1*t # unit: 0.01 nm/ns, [t] = ns
  [../]
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  wGB = 40 # nm, width of the diffuse GB
  length_scale = 1.0e-9 # nm
  time_scale = 1.0e-9 # ns
[]

[Variables]
  [./PolycrystalVariables]
  [../]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0.0
      y1 = 0.0
      x2 = 200.0
      y2 = 400.0
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./PolycrystalKernel]
    use_displaced_mesh = true
  [../]
[]

[AuxKernels]
  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./top_y_tensile]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = top_tensile
  [../]
[]

[Materials]
  [./Copper]
    type = CuGrGr
    block = 0
    T = 750 # K
    use_displaced_mesh = true
  [../]
  [./ElasticityTensor]
    type = ComputePolycrystalElasticityTensor
    block = 0
    fill_method = symmetric9
    #reading C_11  C_12  C_13  C_22  C_23  C_33  C_44  C_55  C_66
    Elastic_constants = '1.684e5 1.214e5 1.214e5 1.684e5 1.214e5 1.684e5 0.754e5 0.754e5 0.754e5' # Unit: Mpa. Read MP values, which agree with units of stress
    GrainTracker_object = grain_tracker
    euler_angle_provider = euler_angle_file
    grain_num = 2
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y'
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
    file_name = test.txt
  [../]
[]

[Preconditioning]
  [./disp]
    type = SMP
    off_diag_row = 'disp_y disp_x gr1 gr0'
    off_diag_column = 'disp_x disp_y gr0 gr1'
  [../]
[]

[Executioner]
  scheme = bdf2
  type = Transient
  solve_type = PJFNK
  petsc_options = -snes_ksp_ew
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre    boomeramg    101'
  nl_abs_tol = 1e-8
  nl_abs_step_tol = 1e-8
  nl_rel_tol = 1e-8
  nl_rel_step_tol = 1e-8
  ss_check_tol = 1e-8
  dt = 100 # ns
  num_steps = 4
  [./Adaptivity]
    initial_adaptivity = 1
    refine_fraction = 0.4
    coarsen_fraction = 0.1
    max_h_level = 3
    weight_names = 'gr0 gr1 disp_x disp_y'
    weight_values = '1 1 0 0'
  [../]
[]

[Outputs]
  interval = 1
  print_perf_log = true
  [./exodus]
    type = Exodus
  [../]
[]
