# 
# Example problem showing how to use the DerivativeParsedMaterial with CHParsed.
# The free energy is identical to that from CHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmin = -2.5
  xmax = 2.5
  ymin = -2.5
  ymax = 2.5
[]

[Variables]
  [./cv]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./wv]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./ci]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
  [./wi]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]
[]

[ICs]
  [./IC_c]
    # int_width = 0.2
    int_width = 2
    x1 = 0.0
    y1 = 0.0
    radius = 1.5
    outvalue = 0.07
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
    block = 0
  [../]
  [./ci_IC]
    variable = ci
    value = 2e-4
    type = ConstantIC
    block = 0
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledImplicitEuler
    variable = wv
    v = cv
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_c
    w = wv
    args = ci
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = D
  [../]
  [./ci_dot]
    type = CoupledImplicitEuler
    variable = wi
    v = ci
  [../]
  [./ci_res]
    type = SplitCHParsed
    variable = ci
    f_name = F
    kappa_name = kappa_c
    w = wi
    args = cv
  [../]
  [./wi_res]
    type = SplitCHWRes
    variable = wi
    mob_name = D
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = 'cv ci'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = 'Ef*cv + Ef*ci + kbT*(cv*log(cv)+ci*log(ci)+(1-cv-ci)*log(1-cv-ci))+((1-cv)^2+ci^2)'
    args = 'cv ci'
    constant_names = 'Ef   kbT'
    constant_expressions = '1.0  0.11'
    tol_values = '1e-12 1e-12'
    tol_names = 'cv ci '
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = cv
  [../]
  [./Mobility_ci]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = ci
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = PJFNK
  end_time = 2
  dt = 0.01
  l_max_its = 35
  l_tol = 1.0e-6
  nl_max_its = 50
  nl_rel_tol = 1.0e-6
  nl_abs_tol = 1.0e-6
[]

[Outputs]
  # print_linear_residuals = true
  exodus = true
  output_on = 'timestep_end initial'
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

