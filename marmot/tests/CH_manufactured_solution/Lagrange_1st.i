[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 70
  ny = 70
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    scaling = 1.0e4
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./c_IC]
    type = FunctionIC
    function = bc_func_red
    variable = c
  [../]
  [./w_IC]
    type = FunctionIC
    function = bc_func_black
    variable = w
  [../]
[]

[Functions]
  [./bc_func_red]
    #Used for ic on c and bc
    type = ParsedGradFunction
    value = sin(alpha*pi*x)*(t+1)
    grad_x = alpha*pi*cos(alpha*pi*x)*(t+1)
    grad_y = 0.0
    grad_z = 0.0
    vars = 'alpha'
    vals = '8'
  [../]
  [./bc_func_black]
    #Used for ic on w and bc
    type = ParsedFunction
    value = sin(alpha*pi*x)*sin(alpha*pi*x)*sin(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)-sin(alpha*pi*x)*(t+1.0)+alpha*alpha*pi*pi*sin(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
  [./forcing_func_black]
    #Used for fo
    type = ParsedFunction
    value = alpha*alpha*alpha*alpha*pi*pi*pi*pi*sin(alpha*pi*x)*(t+1.0)-6.0*alpha*alpha*pi*pi*sin(alpha*pi*x)*cos(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)+3.0*alpha*alpha*pi*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*sin(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)-alpha*alpha*pi*pi*sin(alpha*pi*x)*(t+1.0)+sin(alpha*pi*x)
    vars = 'alpha'
    vals = '8'
  [../]
  [./red_bc_func_left]
    type = ParsedFunction
    value = alpha*pi*cos(alpha*pi*x*t)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
  [./red_bc_func_right]
    type = ParsedFunction
    value = -alpha*pi*cos(alpha*pi*x*t)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
  [./black_bc_func_left]
    type = ParsedFunction
    value = -alpha*alpha*alpha*pi*pi*pi*cos(alpha*pi*x)*(t+1.0)-3.0*alpha*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)+alpha*pi*cos(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
  [./black_bc_func_right]
    type = ParsedFunction
    value = alpha*alpha*alpha*pi*pi*pi*cos(alpha*pi*x)*(t+1.0)+3.0*alpha*pi*sin(alpha*pi*x)*sin(alpha*pi*x)*cos(alpha*pi*x)*(t+1.0)*(t+1.0)*(t+1.0)-alpha*pi*cos(alpha*pi*x)*(t+1.0)
    vars = 'alpha'
    vals = '8'
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./c_res]
    type = SplitCHMath
    variable = c
    kappa_name = 1.0
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = 1.0
    c = c
  [../]
  [./w_res_forcing]
    type = UserForcingFunction
    variable = w
    function = forcing_func_black
  [../]
[]

[BCs]
  [./reddirichletbc]
    type = FunctionDirichletBC
    variable = c
    boundary = '0 1 2 3'
    function = bc_func_red
  [../]
  [./blackdirichletbc]
    type = FunctionDirichletBC
    variable = w
    boundary = '0 1 2 3'
    function = bc_func_black
  [../]
  [./redbc_left]
    type = FunctionNeumannBC
    variable = c
    boundary = '3'
    function = red_bc_func_left
  [../]
  [./redbc_right]
    type = FunctionNeumannBC
    variable = c
    boundary = '1'
    function = red_bc_func_right
  [../]
  [./blackbc_left]
    type = FunctionNeumannBC
    variable = w
    boundary = '3'
    function = black_bc_func_left
  [../]
  [./blackbc_right]
    type = FunctionNeumannBC
    variable = w
    boundary = '1'
    function = black_bc_func_right
  [../]
[]

[Postprocessors]
  [./dofs]
    type = NumDOFs
    variable = c
    execute_on = 'initial timestep_end'
  [../]
  [./L2error]
    type = ElementL2Error
    variable = c
    function = bc_func_red
    execute_on = 'initial timestep_end'
  [../]
  [./H1error]
    type = ElementH1Error
    variable = c
    function = bc_func_red
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

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'crank-nicolson'

  solve_type = 'NEWTON'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 200
  l_tol = 1.0e-4

  nl_max_its = 30
  nl_rel_tol =1.0e-10
  dt = 1.0
  end_time = 1.0
[]

[Outputs]
  interval = 1
  exodus = true
  output_initial = true
  print_linear_residuals = true
  print_perf_log = true
[]
