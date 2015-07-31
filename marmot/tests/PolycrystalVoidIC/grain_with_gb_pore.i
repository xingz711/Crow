[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmax = 40.0
  ymax = 40.0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 8
  var_name_base = gr
  void_num = 10
  min_void_dist = 6.0
  max_void_radius = 2.0
  min_void_radius = 2.0
  ICType = 2
  rand_seed=10000
  maxiter = 10000
  edge_check = false
  use_min_grain_separation_dist = true
  min_grain_separation_dist = 4.0
  use_min_edge_dist = true
  min_edge_dist = 2.0
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolycrystalVoidIC
    [../]
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0e4
  [../]
  [./PolycrystalVoidVariables]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Problem]
  type = FEProblem
  solve = false
[]

[Executioner]
  type = Transient
  num_steps = 0
  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.7
    coarsen_fraction = 0.0
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  [./console]
    type = Console
    perf_log = true
  [../]
[]

