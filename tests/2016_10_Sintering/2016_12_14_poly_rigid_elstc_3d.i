[GlobalParams]
  var_name_base = gr
  op_num = 5.0
  block = 0
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 80
  ny = 40
  nz = 40
  xmax = 200.0
  ymax = 140.0
  zmax = 140.0
  #elem_type = QUAD4
  elem_type = HEX8
[]

[Variables]
  [./c]
    #scaling = 10
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  #[./dF0_x]
  #  order = CONSTANT
  #  family = MONOMIAL
  #[../]
  #[./dF0_y]
  #  order = CONSTANT
  #  family = MONOMIAL
  #[../]
  #[./dF1_x]
  #  order = CONSTANT
  #  family = MONOMIAL
  #[../]
  #[./dF1_y]
  #  order = CONSTANT
  #  family = MONOMIAL
  #[../]
  [./vt_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vt_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vr_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vr_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./unique_grains]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./var_indices]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./centroids]
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
  [./M]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./L]
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
    args = 'gr0 gr1 gr2 gr3 gr4'
  [../]
  [./wres]
    type = SplitCHWResAniso
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
    consider_rigidbodymotion = true
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    translation_constant = 10.0
    rotation_constant = 1.0
  [../]
  [./motion]
    type = MultiGrainRigidBodyMotion
    variable = w
    c = c
    v = 'gr0 gr1 gr2 gr3 gr4'
    grain_force = grain_force
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    translation_constant = 10.0
    rotation_constant = 1.0
  [../]
  [./ElstcEn_gr0]
    type = AllenCahn
    variable = gr0
    args = 'c gr1 gr2 gr3 gr4'
    f_name = E
  [../]
  [./ElstcEn_gr1]
    type = AllenCahn
    variable = gr1
    args = 'c gr0 gr2 gr3 gr4'
    f_name = E
  [../]
  [./ElstcEn_gr2]
    type = AllenCahn
    variable = gr2
    args = 'c gr0 gr1 gr3 gr4'
    f_name = E
  [../]
  [./ElstcEn_gr3]
    type = AllenCahn
    variable = gr3
    args = 'c gr0 gr1 gr2 gr4'
    f_name = E
  [../]
  [./ElstcEn_gr4]
    type = AllenCahn
    variable = gr4
    args = 'c gr0 gr1 gr2 gr3'
    f_name = E
  [../]
  [./TensorMechanics]
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1 gr2 gr3 gr4'
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op kappa_op kappa_op kappa_op'
    interfacial_vars = 'c  gr0 gr1  gr2 gr3 gr4'
  [../]
  #[./dF00]
  #  type = MaterialStdVectorRealGradientAux
  #  variable = dF0_x
  #  property = force_density
  #[../]
  #[./dF01]
  #  type = MaterialStdVectorRealGradientAux
  #  variable = dF0_y
  #  property = force_density
  #  component = 1
  #[../]
  #[./dF10]
  #  type = MaterialStdVectorRealGradientAux
  #  variable = dF1_x
  #  property = force_density
  #  index = 1
  #[../]
  #[./dF11]
  #  type = MaterialStdVectorRealGradientAux
  #  variable = dF1_y
  #  property = force_density
  #  index = 1
  #  component = 1
  #[../]
  [./vt_x]
    type = GrainAdvectionAux
    component = x
    grain_tracker_object = grain_center
    grain_force = grain_force
    grain_volumes = grain_volumes
    variable = vt_x
    translation_constant = 10.0
    rotation_constant = 0.0
  [../]
  [./vt_y]
    type = GrainAdvectionAux
    component = y
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    grain_force = grain_force
    variable = vt_y
    translation_constant = 10.0
    rotation_constant = 0.0
  [../]
  [./vr_x]
    type = GrainAdvectionAux
    component = x
    grain_tracker_object = grain_center
    grain_force = grain_force
    grain_volumes = grain_volumes
    variable = vr_x
    translation_constant = 0.0
    rotation_constant = 1.0
  [../]
  [./vr_y]
    type = GrainAdvectionAux
    component = y
    grain_tracker_object = grain_center
    grain_volumes = grain_volumes
    grain_force = grain_force
    variable = vr_y
    translation_constant = 0.0
    rotation_constant = 1.0
  [../]
  [./unique_grains]
    type = FeatureFloodCountAux
    variable = unique_grains
    flood_counter = grain_center
    field_display = UNIQUE_REGION
    execute_on = timestep_begin
  [../]
  [./var_indices]
    type = FeatureFloodCountAux
    variable = var_indices
    flood_counter = grain_center
    field_display = VARIABLE_COLORING
    execute_on = timestep_begin
  [../]
  [./centroids]
    type = FeatureFloodCountAux
    variable = centroids
    execute_on = timestep_begin
    field_display = CENTROID
    flood_counter = grain_center
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
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./right_x]
    type = PresetBC
    variable = disp_x
    boundary = right
    value = 0
  [../]
  [./front_z]
    type = PresetBC
    variable = disp_z
    boundary = 'front'
    value = 0
  [../]
  [./back_z]
    type = PresetBC
    variable = disp_z
    boundary = 'back'
    value = 0
  [../]
  [./top_y]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = load
  [../]
[]
#
#[Functions]
#  [./load]
#    type = ConstantFunction
#    value = 0.01
#  [../]
#[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -4.0 -4.0'
    x = '0.0 100.0 200.0'
  [../]
[]

[Materials]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1 gr2 gr3 gr4'
    f_name = S
    derivative_order = 2
    #outputs = console
  [../]
  #[./CH_mat]
  #  type = PFDiffusionGrowth
  #  block = 0
  #  rho = c
  #  v = 'gr0 gr1'
  #  outputs = console
  #[../]
  [./mob]
    type = SinteringMtrxMobility
    T = 2100.0
    int_width = 5
    Dvol0 = 4e-6
    Em = 0.94498
    GB_energy = 6.8
    surface_energy = 9.3
    GBmob0 = 3.986e-6
    Qv = 2.0
    Qs = 3.14
    Qgb = 4.14
    Dsurf0 = 1.41e-5
    Dgb0 = 4.0e-4
    Vm = 1.58e-29
    bulkindex = 1.0
    surfindex = 1.0
    gbindex = 1.0
    kb = 8.617343e-5
    JtoeV = 6.24150974e18
    c = c
    v = 'gr0 gr1 gr2 gr3 gr4'
    bulkindex = 1.0
    surfindex = 1.0
    gbindex = 1.0
    length_scale = 1e-07
    time_scale = 0.001
  [../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = '  A       B     L   kappa_op     kappa_c'
    prop_values = '7.9891 0.8488 1.0  15.9158      21.7673    '
  [../]
  # materials for rigid body motion / grain advection
  [./force_density]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = 'gr0 gr1 gr2 gr3 gr4'
    cgb = 0.14
    k = 20
    ceq = 1.0
  [../]
  #elastic properties for phase with c =1
  [./elasticity_tensor_phase1]
    type = ComputeElasticityTensor
    base_name = phase1
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '9.5085e3 1.2102e4'
  [../]
  [./smallstrain_phase1]
    type = ComputeSmallStrain
    base_name = phase1
    block = 0
  [../]
  [./stress_phase1]
    type = ComputeLinearElasticStress
    base_name = phase1
    block = 0
  [../]
  [./elstc_en_phase1]
    type = ElasticEnergyMaterial
    base_name = phase1
    f_name = Fe1
    block = 0
    args = 'c'
    derivative_order = 2
  [../]
  #elastic properties for phase with c = 0
  [./elasticity_tensor_phase0]
    type = ComputeElasticityTensor
    base_name = phase0
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '2.0 2.0'
  [../]
  [./smallstrain_phase0]
    type = ComputeSmallStrain
    base_name = phase0
    block = 0
  [../]
  [./stress_phase0]
    type = ComputeLinearElasticStress
    base_name = phase0
    block = 0
  [../]
  [./elstc_en_phase0]
    type = ElasticEnergyMaterial
    base_name = phase0
    f_name = Fe0
    block = 0
    args = 'c'
    derivative_order = 2
  [../]
  #switching function for elastic energy calculation
  [./switching]
    type = SwitchingFunctionMaterial
    block = 0
    function_name = h
    eta = c
    h_order = SIMPLE
  [../]
  # total elastic energy calculation
  [./total_elastc_en]
    type = DerivativeTwoPhaseMaterial
    block = 0
    h = h
    g = 0.0
    W = 0.0
    eta = c
    f_name = E
    fa_name = Fe1
    fb_name = Fe0
    derivative_order = 2
  [../]
  # gloabal Stress
  [./global_stress]
    type = TwoPhaseStressMaterial
    block = 0
    base_A = phase1
    base_B = phase0
    h = h
  [../]
  # total energy
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S E'
    args = 'c gr0 gr1 gr2 gr3 gr4'
    derivative_order = 2
  [../]
[]

[VectorPostprocessors]
  [./forces]
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
  [./grain_force]
    type = ComputeGrainForceAndTorque
    execute_on = 'linear nonlinear'
    grain_data = grain_center
    force_density = force_density
    c = c
    etas = 'gr0 gr1 gr2 gr3 gr4'
    compute_jacobians = false
  [../]
[]

[Postprocessors]
  #[./mat_D]
  #  type = ElementIntegralMaterialProperty
  #  mat_prop = D
  #[../]
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
    off_diag_column = 'c w c   c   gr0 gr1 gr2 gr3 gr4 gr0 gr0 gr0 gr0 gr1 gr1 gr1 gr1 gr2 gr2 gr2 gr2 gr3 gr3 gr3 gr3 gr4 gr4 gr4 gr4 disp_x disp_y disp_z disp_x disp_y disp_z'
    off_diag_row    = 'w c gr0 gr1 c   c   c   c    c  gr1 gr2 gr3 gr4 gr0 gr2 gr3 gr4 gr2 gr3 gr4 gr0 gr0 gr1 gr2 gr4 gr0 gr1 gr2 gr3 disp_y disp_x disp_x disp_z disp_z disp_y'
    #off_diag_column = 'c w c   c   gr0 gr1 gr0 gr1'
    #off_diag_row    = 'w c gr0 gr1 c   c  gr1  gr0'
  [../]
#[./FDP]
#  type = FDP
#  full = true
#  [../]
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
  end_time = 200
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

[Adaptivity]
  marker = errorfrac
  max_h_level = 2
  [./Indicators]
    [./error]
      type = GradientJumpIndicator
      variable = bnds
    [../]
  [../]
  [./Markers]
    #[./bound_adapt]
    #  type = ValueThresholdMarker
    #  third_state = DO_NOTHING
    #  coarsen = 1.0
    #  refine = 0.99
    #  variable = bnds
    #  invert = true
    #[../]
    [./errorfrac]
      type = ErrorFractionMarker
      coarsen = 0.5
      indicator = error
      refine = 0.8
    [../]
  [../]
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
  gnuplot = true
  print_perf_log = true
  #interval = 10
  #file_base = 2016_10_20_2puneql_oldnonlocal_adv20_25
  [./console]
    type = Console
    perf_log = true
  [../]
[]

[ICs]
  [./ic_gr1]
    x_positions = '50.0 95.0'
    y_positions = '50.0 100.0'
    z_positions = '50.0 100.0'
    int_width = 5.0
    radii = '20.0 20.0'
    3D_spheres = true
    outvalue = 0
    variable = gr1
    invalue = 1
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./multip]
    x_positions = '50.0 100.0 55.0 100.0 55.0 100.0 50.0 95.0 160.0'
    y_positions = '50.0 50.0 100.0 100.0 50.0 50.0 100.0 100.0 75.0'
    z_positions = '50.0 50.0 50.0  50.0 100.0 100.0 100.0 100.0 75.0'
    int_width = 5.0
    radii = '20.0 25.0 25.0 20.0 25.0 20.0 20.0 20.0 34.0'
    3D_spheres = true
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr0]
    x_positions = '55.0 160.0'
    int_width = 5.0
    z_positions = '50.0 75.0'
    y_positions = '100.0 75.0'
    radii = '25.0 34.0'
    3D_spheres = true
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr2]
    x_positions = '50.0 100.0'
    int_width = 5.0
    z_positions = '100.0 50.0'
    y_positions = '100.0 50.0 '
    radii = '20.0 25.0 '
    3D_spheres = true
    outvalue = 0.0
    variable = gr2
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr3]
    x_positions = '100.0 55.0'
    int_width = 5.0
    z_positions = '50.0 100.0'
    y_positions = '100.0 50.0'
    radii = '20.0 25.0'
    3D_spheres = true
    outvalue = 0.0
    variable = gr3
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr4]
    type = SmoothCircleIC
    int_width = 5.0
    x1 = 100.0
    y1 = 50.0
    z1 = 100.0
    radius = 25.0
    outvalue = 0.0
    variable = gr4
    invalue = 1.0
    3D_spheres = true
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
