[GlobalParams]
  var_name_base = gr
  op_num = 2.0
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
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S22]
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
  [./vadv00]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv01]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv10]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vadv11]
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

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -0.4 -0.4 -0.4'
    x = '0.0 30.0 45.0 60.0'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_column = 'c w c   c   gr0 gr1 disp_x disp_y'
    off_diag_row    = 'w c gr0 gr1 c   c   disp_y disp_x'
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0  gr1'
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
  [../]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./motion]
    type = MultiGrainRigidBodyMotion
    variable = w
    c = c
    v = 'gr0 gr1'
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
    v = 'gr0 gr1 '
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op'
    interfacial_vars = 'c  gr0 gr1'
  [../]
  [./S11]
    type = RankTwoAux
    variable = S11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./S22]
    type = RankTwoAux
    variable = S22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    block = 0
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
  [./vadv00]
    type = MaterialStdVectorRealGradientAux
    variable = vadv00
    property = advection_velocity
  [../]
  [./vadv01]
    type = MaterialStdVectorRealGradientAux
    variable = vadv01
    property = advection_velocity
    component = 1
  [../]
  [./vadv10]
    type = MaterialStdVectorRealGradientAux
    variable = vadv10
    property = advection_velocity
    index = 1
  [../]
  [./vadv11]
    type = MaterialStdVectorRealGradientAux
    variable = vadv11
    property = advection_velocity
    index = 1
    component = 1
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

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1'
    [../]
  [../]
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
  [./top_y]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = load
  [../]
[]

[Materials]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    f_name = S
    derivative_order = 2
    outputs = console
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
    prop_names = 'A B L  kappa_op kappa_c'
    prop_values = '16.0 1.0 1.0 0.5 1.0'
  [../]
  # materials for rigid body motion / grain advection
  [./force_density]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = 'gr0 gr1'
    cgb = 0.14
    k = 10
  [../]
  [./vadv]
    type = GrainAdvectionVelocity
    block = 0
    grain_force = grain_force
    etas = 'gr0 gr1'
    c = c
    grain_data = grain_center
  [../]
  #elastic properties for phase with c =1
  [./elasticity_tensor_phase1]
    type = ComputeElasticityTensor
    base_name = phase1
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '30.141 35.46'
  [../]
  [./smallstrain_phase1]
    type = ComputeSmallStrain
    base_name = phase1
    block = 0
    displacements = 'disp_x disp_y'
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
    C_ijkl = '10.0 10.0'
  [../]
  [./smallstrain_phase0]
    type = ComputeSmallStrain
    base_name = phase0
    block = 0
    displacements = 'disp_x disp_y'
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
    args = 'c gr0 gr1'
    derivative_order = 2
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
    etas = 'gr0 gr1'
    execute_on = 'initial linear'
  [../]
  [./grain_force]
    type = ComputeGrainForceAndTorque
    grain_data = grain_center
    c = c
    etas = 'gr0 gr1'
    execute_on = 'initial linear'
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
  [./s11]
    type = ElementIntegralVariablePostprocessor
    variable = S11
  [../]
  [./s22]
    type = ElementIntegralVariablePostprocessor
    variable = S22
  [../]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
  [./free_en]
    type = ElementIntegralMaterialProperty
    mat_prop = F
  [../]
  [./chem_free_en]
    type = ElementIntegralMaterialProperty
    mat_prop = S
  [../]
  [./elstc_en0]
    type = ElementIntegralMaterialProperty
    mat_prop = Fe0
  [../]
  [./elstc_en1]
    type = ElementIntegralMaterialProperty
    mat_prop = Fe1
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.005
  end_time = 60
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  print_linear_residuals = true
  csv = true
  gnuplot = true
  interval = 10
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
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
    y_positions = '13.0 10.0 '
    radii = '5.5 8.0'
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
    radius = 5.5
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
