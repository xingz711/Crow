[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  xmax = 1.0
  ymax = 1.0
  elem_type = QUAD4
[]

[Variables]
  [./u]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = CHMMSIC
      alpha = 8.0
    [../]
  [../]
[]



[Functions]
  active = 'bc_func forcing_func left_func right_func ch_func'

  [./bc_func]
    type = ParsedGradFunction
    value = sin(alpha*pi*x)*(t+1)
    grad_x = alpha*pi*cos(alpha*pi*x)*(t+1)
    grad_y = 0.0
    grad_z = 0.0
    vars = 'alpha'
    vals = '8'
  [../]

  [./forcing_func]
    type = ParsedFunction
    value = alpha*alpha*alpha*alpha*pi*pi*pi*pi*sin(alpha*pi*x)*(t+1.0)-6.0*alpha*alpha*pi*pi*sin(alpha*pi*x)*cos(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)+3.0*alpha*alpha*pi*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*sin(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)-alpha*alpha*pi*pi*sin(alpha*pi*x)*(t+1.0)+sin(alpha*pi*x)
    vars = 'alpha'
    vals = '8'
  [../]

  [./left_func]
    type = ParsedFunction
    value = -alpha*alpha*alpha*pi*pi*pi*cos(alpha*pi*x)*(t+1.0)-3.0*alpha*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)+alpha*pi*cos(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]

  [./right_func]
    type = ParsedFunction
    value = alpha*alpha*alpha*pi*pi*pi*cos(alpha*pi*x)*(t+1.0)+3.0*alpha*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)-alpha*pi*cos(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]

  [./ch_func]
    type = ParsedFunction
    value = -alpha*alpha*pi*pi*sin(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
[]

[Kernels]
  [./ie_c]
    type = TimeDerivative
    variable = u
  [../]

   [./CHInterfaceMMS]
    type = CHInterfaceMMS
    variable = u
   [../]

  [./forcing]
    type = UserForcingFunction
    variable = u
    function = forcing_func
  [../]
[]


[BCs]
  [./left]
    type = FunctionNeumannBC
    variable = u
    boundary = '3'
    function = left_func
  [../]

  [./right]
    type = FunctionNeumannBC
    variable = u
    boundary = '1'
    function = right_func
  [../]

  [./ch]
    type = CHFunctionBC
    variable = u
    boundary = '0 1 2 3'
    function = ch_func
  [../]

  [./top_bottom]
    type = FunctionDirichletBC
    variable = u
    boundary = '0 1 2 3'
    function = bc_func
  [../]
[]



[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type '
  petsc_options_value = ' lu '

  l_max_its = 200
  l_tol = 1.0e-4

  nl_max_its = 30
  nl_rel_tol =1.0e-10
  dt = 1.0
  end_time = 1.0
[]

[Postprocessors]
  [./dofs]
    type = NumDOFs
    execute_on = 'initial timestep_end'
  [../]

  [./L2error]
    type = ElementL2Error
    variable = u
    function = bc_func
    execute_on = 'initial timestep_end'
  [../]

  [./H1error]
    type = ElementH1Error
    variable = u
    function = bc_func
    execute_on = 'initial timestep_end'
  [../]

  [./NEls]
    type = NumElems
    execute_on = 'initial timestep_end'
  [../]

  [./NNodes]
    type = NumNodes
    execute_on = 'initial timestep_end'
  [../]
[]

[Outputs]
  interval = 1
  exodus = true
  output_initial = true
  print_linear_residuals = true
  print_perf_log = true
[]
