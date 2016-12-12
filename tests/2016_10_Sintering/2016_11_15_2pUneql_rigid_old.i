[GlobalParams]
  var_name_base = gr
  op_num = 2.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 70
  ny = 40
  xmin = 3.0
  xmax = 38.0
  ymin = 0.0
  ymax = 20.0
  elem_type = QUAD4
[]

[Variables]
  [./c]
    #scaling = 10
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF0_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF0_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF1_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./dF1_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv0_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv0_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv1_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv1_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./grad_eta0_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
    order = CONSTANT
    family = MONOMIAL
  [./grad_eta0_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./grad_eta1_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./grad_eta1_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1'
  [../]
  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = D
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./PolycrystalSinteringKernel]
    c = c
    consider_rigidbodymotion = false
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    translation_constant = 10.0
    rotation_constant = 2.0
  [../]
  [./motion]
    type = MultiGrainRigidBodyMotion
    variable = w
    c = c
    v = 'gr0 gr1'
    #grain_force = grain_force
    #grain_tracker_object = grain_center
    #grain_volumes = grain_volumes
    #translation_constant = 10.0
    #rotation_constant = 2.0
  [../]
  [./vadv_gr0]
    type = SingleGrainRigidBodyMotion
    variable = gr0
    c = c
    v = 'gr0 gr1'
  [../]
  [./vadv_gr1]
    type = SingleGrainRigidBodyMotion
    variable = gr1
    c = c
    v = 'gr0 gr1'
    op_index = 1
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1'
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op'
    interfacial_vars = 'c  gr0 gr1'
  [../]
  [./dF00]
    type = MaterialStdVectorRealGradientAux
    variable = dF0_x
    property = force_density
  [../]
  [./dF01]
    type = MaterialStdVectorRealGradientAux
    variable = dF0_y
    property = force_density
    component = 1
  [../]
  [./dF10]
    type = MaterialStdVectorRealGradientAux
    variable = dF1_x
    property = force_density
    index = 1
  [../]
  [./dF11]
    type = MaterialStdVectorRealGradientAux
    variable = dF1_y
    property = force_density
    index = 1
    component = 1
  [../]
  [./vadv00]
    type = MaterialStdVectorRealGradientAux
    variable = vadv0_x
    property = advection_velocity
  [../]
  [./vadv01]
    type = MaterialStdVectorRealGradientAux
    variable = vadv0_y
    property = advection_velocity
    component = 1
  [../]
  [./vadv10]
    type = MaterialStdVectorRealGradientAux
    variable = vadv1_x
    index = 1
    property = advection_velocity
  [../]
  [./vadv11]
    type = MaterialStdVectorRealGradientAux
    variable = vadv1_y
    property = advection_velocity
    index = 1
    component = 1
  [../]
  [./grad_eta0_x]
    type = VariableGradientComponent
    component = x
    gradient_variable = gr0
    variable = grad_eta0_x
  [../]
  [./grad_eta0_y]
    type = VariableGradientComponent
    component = y
    gradient_variable = gr0
    variable = grad_eta0_y
  [../]
  [./grad_eta1_x]
    type = VariableGradientComponent
    component = x
    gradient_variable = gr1
    variable = grad_eta1_x
  [../]
  [./grad_eta1_y]
    type = VariableGradientComponent
    component = y
    gradient_variable = gr1
    variable = grad_eta1_y
  [../]
[]

[BCs]
  #[./Periodic]
  #  [./All]
  #    auto_direction = 'x y'
  #    variable = 'c w'
  #  [../]
  #[../]
[]

[Materials]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    #f_name = S
    derivative_order = 2
    #outputs = console
  [../]
  [./CH_mat]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1'
    outputs = console
  [../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = '  A    B   L   kappa_op kappa_c'
    prop_values = '16.0 1.0 1.0  0.5      1.0    '
  [../]
  # materials for rigid body motion / grain advection
  [./force_density]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = 'gr0 gr1'
    cgb = 0.14
    k = 20
    ceq = 1.0
  [../]
  [./advection_vel]
    type = GrainAdvectionVelocity
    c = c
    grain_force = grain_force
    etas = 'gr0 gr1'
    grain_data = grain_center
    translation_constant = 10.0
  [../]
[]

[UserObjects]
  [./grain_center]
    type = ComputeGrainCenterUserObject
    etas = 'gr0 gr1'
    execute_on = 'initial timestep_end linear'
  [../]
  [./grain_force]
    type = ComputeGrainForceAndTorque
    execute_on = 'linear nonlinear'
    grain_data = grain_center
    force_density = force_density
    c = c
    etas = 'gr0 gr1'
    #compute_jacobians = false
  [../]
[]

[Postprocessors]
  [./mat_D]
    type = ElementIntegralMaterialProperty
    mat_prop = D
  [../]
  [./elem_c]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
  [./elem_bnds]
    type = ElementIntegralVariablePostprocessor
    variable = bnds
  [../]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
  [./free_en]
    type = ElementIntegralMaterialProperty
    mat_prop = F
  [../]
  [./dofs]
    type = NumDOFs
    system = NL
  [../]
  [./tstep]
    type = TimestepSize
  [../]
  [./nonlinear_iterations]
    type = NumNonlinearIterations
  [../]
  [./linear_iterations]
    type = NumLinearIterations
  [../]
  [./elapsed_alive]
    type = PerformanceData
    event = 'ALIVE'
  [../]
  [./elapsed_active]
    type = PerformanceData
    event = 'ACTIVE'
  [../]
  [./res_calls]
    type = PerformanceData
    column = n_calls
    event = compute_residual()
  [../]
  [./jac_calls]
    type = PerformanceData
    column = n_calls
    event = compute_jacobian()
  [../]
  [./jac_total_time]
    type = PerformanceData
    column = total_time
    event = compute_jacobian()
  [../]
  [./jac_average_time]
    type = PerformanceData
    column = average_time
    event = compute_jacobian()
  [../]
  [./jac_total_time_with_sub]
    type = PerformanceData
    column = total_time_with_sub
    event = compute_jacobian()
  [../]
  [./jac_average_time_with_sub]
    type = PerformanceData
    column = average_time_with_sub
    event = compute_jacobian()
  [../]
  [./jac_percent_of_active_time]
    type = PerformanceData
    column = percent_of_active_time
    event = compute_jacobian()
  [../]
  [./jac_percent_of_active_time_with_sub]
    type = PerformanceData
    column = percent_of_active_time_with_sub
    event = compute_jacobian()
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    #full = true
    off_diag_column = 'c w c   c   gr0 gr1 gr0 gr1'
    off_diag_row    = 'w c gr0 gr1 c   c   gr1 gr0'
    #off_diag_column = 'c w c   c   gr0 gr1 gr0 gr1'
    #off_diag_row    = 'w c gr0 gr1 c   c  gr1  gr0'
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  #petsc_options_iname = '-pc_type'
  #petsc_options_value = 'lu'
  l_max_its = 20
  nl_max_its = 20
  #nl_abs_tol = 1e-10
  #nl_rel_tol = 1e-08
  l_tol = 1e-04
  end_time = 30
  #dt = 0.01
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
    growth_factor = 1.2
  [../]
  #[./Adaptivity]
  #  refine_fraction = 0.7
  #  coarsen_fraction = 0.1
  #  max_h_level = 2
  #  initial_adaptivity = 1
  #[../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
  gnuplot = true
  print_perf_log = true
  #interval = 10
  file_base = 2016_11_15_2pUneql_rigid_old_40X70
  [./console]
    type = Console
    perf_log = true
  [../]
[]

[ICs]
  [./ic_gr1]
    int_width = 2.0
    x1 = 25.0
    y1 = 10.0
    radius = 8.0
    outvalue = 0.0
    variable = gr1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./multip]
    x_positions = '11.0 25.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '13.0 10.0'
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
    variable = gr0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
