#The units for this simulation are nm and s
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 15
  ny = 15
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 50
  zmin = 0
  zmax = 0
  elem_type = QUAD4

   #uniform_refine = 3
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  temp = 500
[]

[Variables]
active = 'cv PolycrystalVariables'

  [./PolycrystalVariables]
    order = THIRD
    family = HERMITE
  [../]

  [./cv]
    order = THIRD
    family = HERMITE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 25
      y2 = 50
    [../]
  [../]

  [./cv]
    type = SmoothCircleIC
    variable = cv
    invalue = 1.0
    outvalue = 0.1
    radius = 10.0
    x1 = 25.0
    y1 = 25.0
    int_width = 5.0
  [../]
[]


[Kernels]
active = 'PolycrystalBubbleKernel cv_dot cv_bulk cv_interface'

  [./PolycrystalBubbleKernel]
    cg = cv
  [../]

  [./cv_dot]
    type = TimeDerivative
    variable = cv
  [../]

  [./cv_bulk]
    type = CHVacGB
    variable = cv
    mob_name = Mv
  [../]

  [./cv_interface]
    type = CHInterface
    variable = cv
    kappa_name = kappa
    mob_name = Mv
    grad_mob_name = grad_Mv
  [../]

[]


[BCs]
  active = 'penaltygr0 penaltygr1 penaltycv'

  [./penaltycv]
    type = CHRadPenaltyBC
    variable = cv
    boundary = '0 1 2 3 '
  [../]

  [./penaltygr0]
    type = CHRadPenaltyBC
    variable = gr0
    boundary = '0 1 2 3 '
  [../]

  [./penaltygr1]
    type = CHRadPenaltyBC
    variable = gr1
    boundary = '0 1 2 3 '
  [../]

[]

[Materials]
  active = 'vac_props'

  [./vac_props]
    type = UO2VacGBConsT
    block = 0
    M0_GB = 2.0e7 #(10 nm)^4/(eV s)
    Q = 0.23 #ev
    l_GB = 5.0 #*10nm
    l_SS = 5.0 #*10nm
    sigma_SS = 2.0e0 #eV/(10 nm)^2
    sigma_GB = 1.1609e0 #eV/(10 nm)^2
    Dv0 = 32.0e9 #(10 nm)^2/s
    Evm = 2.4 #eV
    cv = cv
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 20
 # l_tol = 1e-8

  nl_max_its = 30
  nl_rel_tol = 1e-10

  start_time = 0.0
  num_steps = 1
  dt = 0.005

[]

[Outputs]
  file_base = vac
  output_initial = true
  interval = 1
  exodus = true
  print_perf_log = true
[]



