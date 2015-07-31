[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 20
  ny = 20
  nz = 10
  xmax = 250
  ymax = 250
  zmax = 125
  elem_type = Hex8
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1.0
  grain_radius = 75
  bubspac = 80
  numbub = 5
  grain_center = '125 125 60'
  radius = 25
  outvalue = 0.1
  invalue = 1.0
  int_width = 30
  radius_variation = 0.0
  radius_variation_type = uniform
  columnar_3D = true
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0e4
  [../]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalCircleGrainVoidIC]
    [../]
  [../]
  [./c_IC]
    variable = c
    structure_type = voids
    type = CircleGrainVoidIC
  [../]
[]

[Kernels]
  [./c_res]
    type = Diffusion
    variable = c
  [../]
  [./w_res]
    type = Diffusion
    variable = w
  [../]
  [./PolycrystalKernel]
    c = c
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y z'
    [../]
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    prop_names = 'L mu gamma_asymm tgrad_corr_mult kappa_op'
    prop_values = '1 1  1           1               1'
    block = 0
  [../]
[]

[Problem]
  type = FEProblem
  solve = false
[]

[Executioner]
  type = Transient
  num_steps = 0
[]

[Outputs]
  file_base = void_gb_circle_3Dcol
  output_initial = true
  exodus = true
  print_perf_log = true
[]

