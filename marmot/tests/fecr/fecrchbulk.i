[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 3
  nz = 0
  xmin = 0
  xmax = 2
  ymin = 0
  ymax = 2
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    initial_condition = 0.5
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    prop_names  = 'M'
    prop_values = '1'
    block = 0
  [./]
[]

[Kernels]
  [./c_res]
    type = FeCrCHBulk
    variable = c
    Temperature = 700
  [../]
  [./time]
    type = TimeDerivative
    variable = c
  [../]
  # A CHInterface kernel would be needed in a real simulation!
[]

[BCs]
  [./left]
    type = DirichletBC
    boundary = left
    value = 1.0
    variable = c
  [../]
  [./right]
    type = DirichletBC
    boundary = right
    value = 0.0
    variable = c
  [../]
[]

[Preconditioning]
  # active = ' '
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-6
  start_time = 0.0
  num_steps = 3
  dt = 1
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]

