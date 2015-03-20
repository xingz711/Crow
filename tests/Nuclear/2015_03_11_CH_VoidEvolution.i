# 
# Example problem showing how to use the DerivativeParsedMaterial with CHParsed.
# The free energy is identical to that from CHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 10
  ymax = 10
[]

[Variables]
  [./cv]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
[]

[ICs]
  [./IC_c]
    int_width = 0.2
    x1 = 5.0
    y1 = 5.0
    radius = 1.5
    outvalue = 0.3
    variable = cv
    invalue = 0.7
    type = SmoothCircleIC
    block = 0
  [../]
[]

[Kernels]
  [./c_dot]
    type = TimeDerivative
    variable = cv
    block = 0
  [../]
  [./CH_bulk]
    type = CHParsed
    variable = cv
    mob_name = D
    f_name = F
    block = 0
  [../]
  [./CH_int]
    type = CHInterface
    variable = cv
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = cv
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = '1.0 0.11'
    function = 'Ef*cv+ kbT*(cv*log(cv)+(1-cv)*log(1-cv))'
    args = cv
    constant_names = 'Ef kbT'
    tol_values = 0.3
    tol_names = cv
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = cv
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  dt = 0.1
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 2
[]

[Outputs]
  exodus = true
  output_on = 'TIMESTEP_END initial'
  tecplot = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

