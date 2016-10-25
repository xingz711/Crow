# test file for showing summing forces and torques obtained from other userobjects
[GlobalParams]
  var_name_base = eta
  op_num = 2
  block = 0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 40
  nz = 0
  xmin = 0.0
  xmax = 40.0
  ymin = 0.0
  ymax = 20.0
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
  [./eta0]
  [../]
  [./eta1]
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
    args = 'eta0 eta1'
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
    v = 'eta0 eta1'
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
  [../]
  #[./RigidBodyMultiKernel]
  #  # Creates all of the necessary Allen Cahn kernels automatically
  #  c = c
  #  f_name = F
  #  mob_name = M
  #  kappa_name = kappa_eta
  #  grain_force = grain_force
  #  grain_tracker_object = grain_center
  #  grain_volumes = grain_volumes
  #[../]
  [./PolycrystalSinteringKernel]
    c = c
  [../]
  [./vadv_gr0]
    type = SingleGrainRigidBodyMotion
    variable = eta0
    c = c
    v = 'eta0 eta1'
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
  [../]
  [./vadv_gr1]
    type = SingleGrainRigidBodyMotion
    variable = eta1
    c = c
    v = 'eta0 eta1'
    op_index = 1
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
  [../]
[]

[Functions]
  [./load]
    type = ConstantFunction
    value = 0.01
  [../]
[]

[Materials]
  #[./pfmobility]
  #  type = GenericConstantMaterial
  #  prop_names = 'M    kappa_c  kappa_eta'
  #  prop_values = '5.0  2.0      0.1'
  #[../]
  #[./free_energy]
  #  type = DerivativeParsedMaterial
  #  f_name = F
  #  args = c
  #  constant_names = 'barr_height  cv_eq'
  #  constant_expressions = '0.1          1.0e-2'
  #  #function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2
  #  function = 16*barr_height*(c-cv_eq)^2*(1-cv_eq-c)^2+eta0*(1-eta0)*c+eta1*(1-eta1)*c
  #  derivative_order = 2
  #[../]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'eta0 eta1'
    #f_name = S
    derivative_order = 2
    outputs = console
  [../]
  #[./CH_mat]
  #  type = PFDiffusionGrowth
  #  block = 0
  #  rho = c
  #  v = 'gr0 gr1'
  #  outputs = console
  #[../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'A B L  kappa_op kappa_c M'
    prop_values = '16.0 1.0 1.0 0.5 1.0 1.0'
  [../]
  [./force_density]
    type = ForceDensityMaterial
    c = c
    etas ='eta0 eta1'
    cgb = 0.14
    k = 10
  [../]
  [./force_density_ext]
    type = ExternalForceDensityMaterial
    c = c
    etas = 'eta0 eta1'
    k = 1.0
    force_y = load
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF00]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF01]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF10]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'eta0 eta1 '
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op'
    interfacial_vars = 'c  eta0 eta1'
  [../]
  [./dF00]
    type = MaterialStdVectorRealGradientAux
    variable = dF00
    property = force_density
  [../]
  [./dF01]
    type = MaterialStdVectorRealGradientAux
    variable = dF01
    property = force_density
    component = 1
  [../]
  [./dF10]
    type = MaterialStdVectorRealGradientAux
    variable = dF10
    property = force_density
    index = 1
  [../]
  [./dF11]
    type = MaterialStdVectorRealGradientAux
    variable = dF11
    property = force_density
    index = 1
    component = 1
  [../]
  [./vadv_x]
    type = GrainAdvectionAux
    component = x
    grain_tracker_object = grain_center
    grain_force = grain_force
    grain_volumes = grain_volumes
    variable = vadv_x
    execute_on = 'timestep_end'
  [../]
  [./vadv_y]
    type = GrainAdvectionAux
    component = y
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    grain_force = grain_force
    variable = vadv_y
    execute_on = 'timestep_end'
  [../]
[]

[ICs]
  [./ic_gr1]
    int_width = 2.0
    x1 = 25.0
    y1 = 10.0
    radius = 8.0
    outvalue = 0.0
    variable = eta1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./multip]
    x_positions = '11.0 25.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '13.0 10.0 '
    radii = '6.0 8.0'
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr0]
    int_width = 2.0
    x1 = 11.0
    y1 = 13.0
    radius = 6.0
    outvalue = 0.0
    variable = eta0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

[VectorPostprocessors]
  [./forces_dns]
    type = GrainForcesPostprocessor
    grain_force = grain_force_dns
  [../]
  [./forces_cosnt]
    type = GrainForcesPostprocessor
    grain_force = grain_force_const
  [../]
  [./forces_total]
    type = GrainForcesPostprocessor
    grain_force = grain_force
  [../]
  [./grain_volumes]
    type = FeatureVolumeVectorPostprocessor
    flood_counter = grain_center
    execute_on = 'initial timestep_begin'
  [../]
[]

[UserObjects]
  [./grain_center]
    type = GrainTracker
    outputs = none
    compute_var_to_feature_map = true
    execute_on = 'initial timestep_begin'
  [../]
  [./grain_force_dns]
    type = ComputeGrainForceAndTorque
    c = c
    etas = 'eta0 eta1'
    execute_on = 'linear nonlinear'
    grain_data = grain_center
    force_density = force_density
  [../]
  [./grain_force_ext]
    type = ComputeGrainForceAndTorque
    execute_on = 'linear nonlinear'
    grain_data = grain_center
    c = c
    etas = 'eta0 eta1'
    force_density = force_density_ext
  [../]
  [./grain_force_const]
    type = ConstantGrainForceAndTorque
    execute_on = 'linear nonlinear'
    force =  '0.5 0.0 0.0 -0.5 0.0 0.0'
    torque = '0.0 0.0 0.0 0.0 0.0 0.0'
  [../]
  [./grain_force]
    type = GrainForceAndTorqueSum
    execute_on = 'linear nonlinear'
    grain_forces = 'grain_force_dns grain_force_const grain_force_ext'
    grain_num = 2
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
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-4
  #nl_rel_tol = 1.0e-10
  #start_time = 0.0
  #num_steps = 2
  end_time = 20
  dt = 0.01
[]

[Outputs]
  exodus = true
  csv = true
  print_perf_log = true
[]

#[Debug]
#  show_var_residual_norms = true
#[]
