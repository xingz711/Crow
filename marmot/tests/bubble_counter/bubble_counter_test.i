[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 10
  zmin = 0
  zmax = 0
  elem_type = QUAD4

#  uniform_refine = 1
[]

[Variables]
  active = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE

    [./InitialCondition]
       type = MultiSmoothCircleIC
       bubspac = 1.2
       numbub = 10
       invalue = 1.0
       outvalue = 0.005
       radius = 0.45
       int_width = 0.25
       rand_seed = 132435
    [../]
  [../]
[]

[AuxVariables]
  [./proc_id]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'diff'

  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[AuxKernels]
  [./proc_id]
    type = ProcessorIDAux
    variable = proc_id
  [../]
[]

[BCs]
  active = 'left right'

  [./left]
    type = DirichletBC
    variable = u
    boundary = 3
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 1
  [../]
[]

[Postprocessors]
  [./bubbles]
    type = FeatureFloodCount
    variable = u
    execute_on = timestep_end
    threshold = 0.3
    execute_on = 'initial timestep_end'
  [../]
[]


[Executioner]
  type = Transient
  num_steps = 1
  dt = 0.1

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  nl_abs_tol = 1e-12
[]

[Outputs]
  output_initial = true
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
