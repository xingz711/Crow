[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 125
  ny = 125
  nz = 0
  xmax = 250
  ymax = 250
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  en_ratio = 1.0
  grain_radius = 100
  bubspac = 60
  numbub = 5
  grain_center = '125 0 0'
  radius = 15
  outvalue = 0.001
  invalue = 1.0
  int_width = 6
  radius_variation = 0.3
  radius_variation_type = uniform
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
      auto_direction = 'x y'
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
  file_base = void_gb_circle_var
  output_initial = true
  exodus = true
  print_perf_log = true
[]

