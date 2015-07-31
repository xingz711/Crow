[Mesh]
  # uniform_refine = 4
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  nz = 0
  xmax = 250
  ymax = 250
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 3
  var_name_base = gr
  en_ratio = 1.0
  void_num = 5
  min_void_dist = 40
  max_void_radius = 20
  min_void_radius = 10
  ICType = 1
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = PolycrystalVoidIC
      typ = 0
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

[Materials]
  [./const]
    type = GenericConstantMaterial
    prop_names =  'L mu gamma_asymm tgrad_corr_mult kappa_op'
    prop_values = '1 1  1           1               1'
    block = 0
  [../]
[]

[Postprocessors]
  [./totalc]
    type = ElementIntegralVariablePostprocessor
    variable = c
    execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  # active = ' '
  [./SMP]
    # solve_order = 'w c gr0 gr1 gr2 gr3 gr4 gr5'
    # preconditioner = 'AMG ASM AMG AMG'
    # off_diag_row = 'c '
    # off_diag_column = 'w '
    type = SMP
    off_diag_row = 'c w'
    off_diag_column = 'w c'
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
  file_base = void_gb_poly
  output_initial = true
  exodus = true
  print_perf_log = true
[]
