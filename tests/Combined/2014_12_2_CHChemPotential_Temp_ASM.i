[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 80
  xmax = 40
  ymax = 40
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
[]

[AuxVariables]
  [./T]
    block = 0
    [./InitialCondition]
      value = 300
      variable = T
      type = ConstantIC
      block = 0
    [../]
  [../]
[]

[Kernels]
  [./CHBulk]
    type = CHChemPotential
    variable = c
    mob_name = D
    block = 0
  [../]
  [./Cdot]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./ChInt]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
  [./CH_temp]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = T
    block = 0
  [../]
[]

[AuxKernels]
  active = 'const_temp'
  [./Temp]
    type = FunctionAux
    variable = T
    function = 'Temp_profile '
    execute_on = timestep
    block = 0
  [../]
  [./const_temp]
    type = ConstantAux
    variable = T
    value = 300.0
  [../]
[]

[BCs]
  [./Periodic]
    [./Periodic_c]
      variable = c
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./PFDiff]
    type = PFDiffusion
    block = 0
    rho = c
    kappa = 10
  [../]
  [./Ftemp]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = 8.314e-3
    function = 'T*(c*log(c)+ (1-c)*log(1-c))'
    args = 'c T'
    constant_names = R
    tol_values = 0.001
    tol_names = c
  [../]
[]

[Postprocessors]
  [./Elem_Avg]
    type = ElementIntegralVariablePostprocessor
    variable = c
    block = 0
  [../]
  [./Side_Int_var]
    type = SideIntegralVariablePostprocessor
    variable = c
    boundary = top
  [../]
[]

[Executioner]
  type = Transient
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  nl_abs_tol = 1e-9
  end_time = 10
  l_tol = 1e-04
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  exodus = true
  output_final = true
  print_perf_log = true
  [./console]
    type = Console
    output_initial = true
    output_file = true
  [../]
[]

[ICs]
  max = 0.01
  op_num = 2
  radius = '9.0 7.0'
  variable = c
  type = TwoParticleDensityIC
  block = 0
  [./2pdens]
    max = 0.01
    op_num = 2
    radius = '9.0 7.0 '
    variable = c
    type = TwoParticleDensityIC
    block = 0
  [../]
[]

