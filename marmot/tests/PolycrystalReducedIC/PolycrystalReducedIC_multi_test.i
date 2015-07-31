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
  op_num = 4
  var_name_base = gr
[]

[Variables]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./PolycrystalVoronoiIC]
      grain_num = 8 #Number of grains
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./grain_map0]
    order = FIRST
    family = LAGRANGE
  [../]
  [./grain_map1]
    order = FIRST
    family = LAGRANGE
  [../]
  [./grain_map2]
    order = FIRST
    family = LAGRANGE
  [../]
  [./grain_map3]
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
  [./mapper0]
    type = FeatureFloodCountAux
    variable = grain_map0
    execute_on = timestep_end
    bubble_object = grains
    map_index = 0
  [../]
  [./mapper1]
    type = FeatureFloodCountAux
    variable = grain_map1
    execute_on = timestep_end
    bubble_object = grains
    map_index = 1
  [../]
  [./mapper2]
    type = FeatureFloodCountAux
    variable = grain_map2
    execute_on = timestep_end
    bubble_object = grains
    map_index = 2
  [../]
  [./mapper3]
    type = FeatureFloodCountAux
    variable = grain_map3
    execute_on = timestep_end
    bubble_object = grains
    map_index = 3
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 500 # K
    wGB = 50 # nm
  [../]
[]

[UserObjects]
  [./grains]
    type = FeatureFloodCount
    variable = 'gr0 gr1 gr2 gr3'
    threshold = 0.05
    execute_on = timestep_end
    use_single_map = false
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
  scheme = bdf2

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  l_tol = 1.0e-4
  l_max_its = 30
  nl_max_its = 20
  nl_rel_tol = 1.0e-9
  start_time = 0.0
  num_steps = 1
  dt = 15.0
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
