[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 20
  xmin = -10
  xmax = 10
  ymin = -5
  ymax = 5
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
  [../]
[]

[ICs]
  [./ic_c]
    x_positions = '-2.0 2.0 '
    int_width = 3.0
    z_positions = '0.0 0.0 '
    y_positions = '-5.0 5.0 '
    radii = '5.0 5.0 '
    outvalue = -1.0
    variable = c
    invalue = '1.0 '
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Kernels]
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./CH_bulk]
    type = CHParsed
    f_name = F
    variable = c
  [../]
  [./CH_int]
    type = CHInterface
    variable = c
    mob_name = M
    kappa_name = kappa_c
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'M kappa_c'
    prop_values = '1.0 0.5'
  [../]
  [./free_energy]
    type = MathFreeEnergy
    c = c
    block = 0
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  #petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  #petsc_options_value = 'asm         31   preonly   lu      1'
  petsc_options_iname = -pc_type
  petsc_options_value = lu
  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10

  end_time = 5.0
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
  [../]
[]

[Outputs]
  csv = true
  output_initial = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
  [./out]
    type = Exodus
    refinements = 2
  [../]
[]
