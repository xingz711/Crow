[GlobalParams]
  var_name_base = gr
  op_num = 2
  length_scale = 1.0e-9
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 75
  ny = 33
  nz = 0
  xmax = 1500
  ymax = 750
  uniform_refine = 0
  elem_type = QUAD4
[]

[Variables]
  [./T]
    initial_condition = 800
    scaling = 1.0e10
  [../]
[]

[AuxVariables]
  [./c]
    [./InitialCondition]
      inside = '1.0 1.0'
      y1 = '0 0'
      y2 = '750 750'
      x1 = '340 1090'
      x2 = '410 1160'
      type = MultiBoundingBoxIC
    [../]
  [../]
  [./ThCond]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gr0]
    [./InitialCondition]
      y2 = 750
      inside = 1.0
      y1 = 0
      outside = 0.0
      x2 = 1090
      x1 = 410
      type = BoundingBoxIC
    [../]
  [../]
  [./gr1]
    [./InitialCondition]
      y2 = 750
      y1 = 0
      outside = 1
      x2 = 1160
      x1 = 340
      type = BoundingBoxIC
    [../]
  [../]
[]

[Kernels]
  [./HtCond]
    type = HeatConduction
    variable = T
  [../]
[]

[AuxKernels]
  [./ThCond]
    type = MaterialRealAux
    variable = ThCond
    execute_on = timestep_end
    property = thermal_conductivity
  [../]
[]

[BCs]
  [./Const_T]
    type = PresetBC
    variable = T
    boundary = left
    value = 800
  [../]
  [./Const_fluxx]
    type = NeumannBC
    variable = T
    boundary = right
    value = -0.5e-11
  [../]
[]

[Materials]
  [./Thermal]
    type = MicroThermal
    block = 0
    Gas_conductivity = 0.00565
    GB_conductivity = 2.5
    eta = c
    Bulk_conductivity = 4.75
  [../]
[]

[Postprocessors]
  [./EffThCond]
    type = ThermalCond
    variable = T
    flux = 0.5e-11
    T_hot = 800
    dx = 1500
    boundary = right
  [../]
  [./Rprime_x]
    type = EffectiveKapitzaResistance
    dx = 750.0 # Grain size
    ko = 4.75
    eff_cond = EffThCond
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-5

  nl_max_its = 45
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-10
  start_time = 0.0
  num_steps = 10
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  print_perf_log = true
[]

