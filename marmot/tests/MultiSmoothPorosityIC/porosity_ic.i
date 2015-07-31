[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmax = 25.0
  ymax = 25.0
  zmax = 0.0
  elem_type = QUAD4
[]

[GlobalParams]
  radius_mean = 2.0
  radius_variation = 0.5
  bubspac = 0.5
  porosity = 0.2
  radius_min = 0.5
  radius_min_geom = 2.0
  rand_seed = 12345
  edge_min_dist = 0.5
  include_small_pore = true
  lx_size = 5
  ly_size = 5
  max_loc_porosity = 0.8
  numtries = 100
  radius_variation_type = normal
  invalue = 1.0
  outvalue = 0.0
  int_width = 0.0
[../]

[Variables]
  [./void]
    block = 0
    [./InitialCondition]
       type = MultiSmoothCirclePorosityIC
       property_type = concentration
    [../]
  [../]
[]

[AuxVariables]
  [./porosity]
    order = FIRST
    family = LAGRANGE
    block = 0
    [./InitialCondition]
       type = MultiSmoothCirclePorosityIC
       property_type = porosity
    [../]
  [../]
  [./rad_max]
    order = FIRST
    family = LAGRANGE
    block = 0
    [./InitialCondition]
       type = MultiSmoothCirclePorosityIC
       property_type = maximum_radius
    [../]
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 0
[]

[Outputs]
  file_base = porosity_ic_out
  output_initial = true
  exodus = true
  print_perf_log = true
[]

[Problem]
  type = FEProblem
  solve = false
[]
