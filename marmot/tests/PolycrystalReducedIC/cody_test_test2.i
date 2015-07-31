[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmin = 0
  xmax = 500
  ymin = 0
  ymax = 500
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 5
  var_name_base = gr
[]

[Variables]
  [./PolycrystalVariables]
    x1 = 0.0
    y1 = 0.0
    x2 = 500.0
    y2 = 500.0
    periodic = '0 0 0'
    ICType = -4
    grain_num = 10
    cody_test = true
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./var_indices]
    order = FIRST
    family = LAGRANGE
  [../]
  [./unique_grains]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]
  [./var_indices]
    type = FeatureFloodCountAux
    variable = var_indices
    execute_on = timestep_end
    bubble_object = var_indices
  [../]
  [./unique_grains]
    type = GrainTrackerAux
    variable = unique_grains
    execute_on = timestep_end
    bubble_object = unique_grains
  [../]
[]

[BCs]
[]

[Materials]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    temp = 500 # K
    wGB = 50 # nm
  [../]
[]

[UserObjects]
  [./remapper]
    type = GrainTracker
    threshold = 0.01
    convex_hull_buffer = 20.0
    execute_on = timestep_end
    remap_grains = true
    use_single_map = false
  [../]
  [./var_indices]
    type = FeatureFloodCount
    variable = 'gr0 gr1 gr2 gr3 gr4'
    threshold = 0.5
    execute_on = timestep_end
    use_single_map = true
    show_var_coloring = true
  [../]

  [./unique_grains]
    type = GrainTracker
    threshold = 0.5
    execute_on = timestep_begin
    remap_grains = false
    use_single_map = true
    grain_remapper = remapper
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  l_tol = 1.0e-4
  l_max_its = 30
  nl_max_its = 20
  nl_rel_tol = 1.0e-9
  nl_abs_tol = 1.0e-10
  start_time = 0.0
  num_steps = 20
  dt = 60.0
[]

[Outputs]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
