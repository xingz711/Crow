[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  nz = 0
  xmax = 100
  ymax = 100
  zmax = 0
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
[]

[Kernels]
  [./CH_Parsed]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = c
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./CH_int]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = c
      auto_direction = 'x y '
    [../]
  [../]
[]

[Materials]
  [./FreeEnergy]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = '0.5 1.0 8.6173324e-5 1276 1.0 1.0 1.13e-4'
    function = '(e-1)^2*(e+1)^2*(Ef*c+kb*T*(c*log(c)+(1-c)*log(1-c))) - A*(c-ceq)^2*e*(e+2)*(e-1)^2 + B*(c-1)^2*e^2'
    args = c
    constant_names = 'e Ef kb T A B ceq '
    tol_values = 0.001
    tol_names = c
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    c = c
    kappa = 2.5e-3
    Dv = 0.64
  [../]
[]

[Executioner]
  type = Transient
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 100
  scheme = bdf2
[]

[Outputs]
  exodus = true
  show = c
  tecplot = true
  interval = 5
  output_on = 'timestep_end initial'
  [./console]
    type = Console
    perf_log = true
    fit_mode = 120
    output_file = true
    output_on = 'timestep_end nonlinear failed initial'
  [../]
[]

[ICs]
  [./randomIC]
    variable = c
    max = 0.1
    type = RandomIC
    block = 0
  [../]
[]

