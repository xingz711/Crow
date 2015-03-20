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
  active = 'wv cv'
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
  [./e]
  [../]
[]

[ICs]
  [./IC_c]
    # int_width = 0.2
    int_width = 0.5
    x1 = 0.0
    y1 = 0.0
    radius = 1.5
    outvalue = 0.07
    variable = cv
    invalue = 0.999
    type = SmoothCircleIC
    block = 0
  [../]
[]

[Kernels]
  active = 'c_res w_res c_dot'
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
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = D
  [../]
  [./AC_bulk]
    type = ACParsed
    variable = e
    f_name = F
    args = cv
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./Energy_matrix]
    type = DerivativeParsedMaterial
    block = 0
    function = '(e^2-1)^2*(Ef*cv+ kbT*(cv*log(cv)+(1-cv)*log(1-cv)))'
    args = 'cv '
    constant_names = 'Ef kbT e'
    constant_expressions = '1.0  0.11 0.5'
    tol_values = 0.01
    tol_names = cv
    third_derivatives = false
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = cv
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
  tecplot = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

