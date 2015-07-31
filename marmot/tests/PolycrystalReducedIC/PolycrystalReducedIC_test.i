[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 35
  ny = 35
  nz = 0
  xmin = 0
  xmax = 500
  ymin = 0
  ymax = 500
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 7
  var_name_base = gr
[]

[Variables]

  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./PolycrystalVoronoiIC]
      grain_num = 8 #Number of grains
    [../]
  [../]
[]

[AuxVariables]

  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]

  [./grain_map]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]

  [./PolycrystalKernel]
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
  [../]

  [./mapper]
    type = FeatureFloodCountAux
    variable = grain_map
    execute_on = timestep_end
    bubble_object = grains
  [../]
[]

[BCs]
  active = 'Periodic'

  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]

[]

[Materials]
  active = 'CuGrGr'

  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 500 #K
    wGB = 75 #nm
  [../]
[]

[UserObjects]
  [./grains]
    type = FeatureFloodCount
    variable = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6'
    threshold = 0.3
    execute_on = timestep_end
  [../]
[]

[Postprocessors]
  [./num_grains]
    type = FeatureFloodCount
    variable = bnds
    threshold = 0.7
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'

  l_tol = 1.0e-4
  l_max_its = 30

  nl_max_its = 20
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 1
  dt = 30.0
[]

[Outputs]
  file_base = out
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]
