[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 50
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 25
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./eta0]
  [../]
  [./eta1]
  [../]
  [./bnds]
  [../]
  [./df0]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./df1]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv0]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv1]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv_div0]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv_div1]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  # active = 'c_res time w_res'
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./motion]
    type = MultiGrainRigidBodyMotion
    variable = w
    c = c
    advection_velocity = advection_velocity
    advection_velocity_divergence = advection_velocity_divergence
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    var_name_base = eta
    op_num = 2.0
    v = 'eta0 eta1'
    block = 0
  [../]
  [./df0]
    type = MaterialStdVectorRealGradientAux
    variable = df0
    index = 0
    component = 1
    property = force_density
  [../]
  [./df1]
    type = MaterialStdVectorRealGradientAux
    variable = df1
    index = 1
    component = 1
    property = force_density
  [../]
  [./vadv0]
    type = MaterialStdVectorRealGradientAux
    variable = vadv0
    component = 1
    property = advection_velocity
  [../]
  [./vadv1]
    type = MaterialStdVectorRealGradientAux
    variable = vadv1
    index = 1
    component = 1
    property = advection_velocity
  [../]
  [./vadv_div0]
    type = MaterialStdVectorAux
    variable = vadv_div0
    property = advection_velocity_divergence
  [../]
  [./vadv_div1]
    type = MaterialStdVectorAux
    variable = vadv_div1
    index = 1
    property = advection_velocity_divergence
  [../]
[]

[Materials]
  # active = 'pfmobility free_energy force_density'
  [./pfmobility]
    type = PFMobility
    block = 0
    kappa = 0.1
    mob = 1e-3
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    block = 0
    f_name = F
    args = c
    constant_names = 'barr_height  cv_eq'
    constant_expressions = '0.1          1.0e-2'
    function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2
    derivative_order = 2
  [../]
  [./force_density]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = 'eta0 eta1'
  [../]
  [./advection_vel]
    type = GrainAdvectionVelocity
    block = 0
    grain_force = grain_force
    etas = 'eta0 eta1'
    grain_data = grain_center
  [../]
[]

[VectorPostprocessors]
  [./centers]
    type = GrainCentersPostprocessor
    grain_data = grain_center
  [../]
  [./forces]
    type = GrainForcesPostprocessor
    grain_force = grain_force
  [../]
[]

[UserObjects]
  [./grain_center]
    type = ComputeGrainCenterUserObject
    etas = 'eta0 eta1'
    execute_on = 'initial linear'
  [../]
  [./grain_force]
    type = ComputeGrainForceAndTorque
    force_density = force_density
    grain_data = grain_center
    execute_on = 'initial linear'
  [../]
[]

[Preconditioning]
  # active = ' '
  active = 'SMP'
  [./SMP]
    type = SMP
    full = true
  [../]
  [./FDP]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  # petsc_options_iname =  '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  # petsc_options_value =  'asm         31   preonly   lu      1'
  type = Transient
  nl_max_its = 20
  scheme = bdf2
  solve_type = PJFNK
  petsc_options_iname = -pc_type
  petsc_options_value = lu
  l_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  start_time = 0.0
  dt = 0.1
  end_time = 300
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.1
  [../]
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
  csv = true
  file_base = deriv
[]

[ICs]
  active = 'eta0_rnd rnd_eta1 rnd_c'
  [./ic_eta0]
    int_width = 6.0
    x1 = 20.0
    y1 = 0.0
    radius = 14.0
    outvalue = 0.0
    variable = eta0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./IC_eta1]
    int_width = 6.0
    x1 = 30.0
    y1 = 25.0
    radius = 14.0
    outvalue = 0.0
    variable = eta1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./eta0_rnd]
    variable = eta0
    type = RandomIC
    block = 0
  [../]
  [./rnd_eta1]
    variable = eta1
    type = RandomIC
    block = 0
  [../]
  [./rnd_c]
    variable = c
    type = RandomIC
    block = 0
  [../]
[]

