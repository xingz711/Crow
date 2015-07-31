[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 12
  ny = 12
  nz = 0
  xmin = 0
  xmax = 125
  ymin = 0
  ymax = 125
  zmin = 0
  zmax = 0
  elem_type = QUAD4

  uniform_refine = 2
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
[]

[Variables]

  [./PolycrystalVariables]
  [../]

  [./cg]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = .01
    [../]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 62.5
      y2 = 125
    [../]
  [../]
[]

[AuxVariables]

  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
active = 'PolycrystalKernel ie_cg Diff_cg CHsink'

  [./PolycrystalKernel]
  [../]

  [./ie_cg]
    type = TimeDerivative
    variable = cg
  [../]

  [./Diff_cg]
    type = MatDiffusion
    variable = cg
    D_name = Dg
  [../]

  [./CHsink]
    type = CHRadSinkAniso
    variable = cg
    mob_name = Mg
  [../]
[]

[AuxKernels]
  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[BCs]
  active = 'Periodic'

  [./Periodic]
    [./top_bottom]
      primary = 0
      secondary = 2
      translation = '0 125 0'
    [../]
  [../]

[]

[Materials]
  active = 'LANL'

  [./LANL]
    type = UO2AnisoConsTemp
    block = 0
    temp = 1300 #K
    c = cg
    kappa_cg = 100
    M0GB = 2.5e-6 #m^4/(J*s);
    QGB = 2.3 #eV
    sigma_GB = 0.708 #J/m^2
  [../]

[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30

  nl_max_its = 60
  nl_abs_tol = 1e-10

  start_time = 0.0
  num_steps = 4
  dt = 0.1

  [./TimePeriods]
    [./p1]
      start = 0.0
      inactive_kernels = 'CHsink'
      inactive_bcs = ''
    [../]

    [./p2]
      start = 0.3
      active_kernels = 'ie_cg Diff_cg CHsink IE_gr1 IE_gr0'
      inactive_bcs = ''
    [../]
  [../]

  [./Adaptivity]
    initial_adaptivity = 2
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = aniso
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]



