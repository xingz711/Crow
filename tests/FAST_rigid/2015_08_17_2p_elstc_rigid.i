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
    block = 0
  [../]
  [./disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./e11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./e22]
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
  [./T]
  [../]
  [./ElasticEn]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./C1111]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./C2222]
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
    y = '0.0 -1.5 -1.5 -1.5'
    x = '0.0 30.0 45.0 60.0'
  [../]
  [./temp]
    type = PiecewiseLinear
    y = '300.0 673.0 2473.0 2473.0 673.0 300.0'
    x = '0.0 5.32 21.26 45.51 50.69 60.0'
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./PBP]
    type = PBP
    solve_order = 'w c'
    preconditioner = 'AMG ASM'
    off_diag_row = 'c '
    off_diag_column = 'w '
  [../]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0  gr1 '
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
  [./Elstc_gr0]
    type = ACParsed
    variable = gr0
    f_name = E
    args = 'c gr1'
  [../]
  [./Elstc_gr1]
    type = ACParsed
    variable = gr1
    f_name = E
    args = 'c gr0 '
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
    interfacial_vars = 'c gr0 gr1 '
  [../]
  [./e11]
    type = RankTwoAux
    variable = e11
    rank_two_tensor = total_strain
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./e22]
    type = RankTwoAux
    variable = e22
    rank_two_tensor = total_strain
    index_j = 1
    index_i = 1
    block = 0
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
  [./T]
    type = FunctionAux
    variable = T
    function = temp
    block = 0
  [../]
  [./ElasticEn]
    type = TensorElasticEnergyAux
    variable = ElasticEn
    block = 0
  [../]
  [./C1111]
    type = RankFourAux
    variable = C1111
    rank_four_tensor = elasticity_tensor
    index_l = 0
    index_j = 0
    index_k = 0
    index_i = 0
    block = 0
  [../]
  [./C2222]
    type = RankFourAux
    variable = C2222
    rank_four_tensor = elasticity_tensor
    index_l = 1
    index_j = 1
    index_k = 1
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
  [./Disp_x]
    type = PresetBC
    variable = disp_x
    boundary = 'right left'
    value = 0.0
  [../]
  [./Disp_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
  [./Load]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = load
  [../]
[]

[Materials]
  active = 'AC_mat stress temp Elstc_en ElasticityTensor sum CH_mat strain free_energy force_density vadv'
  [./constant]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 2.0
  [../]
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
    v = 'gr0 gr1 '
    outputs = console
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op A B'
    prop_values = '1.0 0.5 16.0 1.0'
  [../]
  [./temp]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = 8.617e-5
    fail_on_evalerror = true
    function = kb*T*(c*log(c)+(1-c)*log(1-c))
    f_name = Ft
    args = 'c  T'
    constant_names = kb
    tol_values = 1e-3
    tol_names = c
    derivative_order = 2
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S Ft E'
    args = 'c gr0 gr1'
    derivative_order = 2
  [../]
  [./Eigen]
    type = PFEigenStrainMaterial1
    block = 0
    c = c
    disp_y = disp_y
    disp_x = disp_x
    epsilon0 = 0.05
    C_ijkl = '153e-3 180e-3'
    fill_method = symmetric_isotropic
    v = 'gr0 gr1'
    e_v = 0.01
    thermal_expansion_coeff = 4.3e-6
    T = T
  [../]
  [./Elstc_en]
    type = ElasticEnergyMaterial
    block = 0
    f_name = E
    args = 'c gr0 gr1 '
    derivative_order = 2
  [../]
  [./ElasticityTensor]
    type = ComputeConcentrationDependentElasticityTensor
    block = 0
    c = c
    C1_ijkl = '30.141 35.46'
    C0_ijkl = '10.0 10.0'
    fill_method1 = symmetric_isotropic
    fill_method0 = symmetric_isotropic
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y'
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
  [../]
  [./force_density]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = 'gr0 gr1'
    cgb = 0.14
  [../]
  [./vadv]
    type = GrainAdvectionVelocity
    block = 0
    grain_force = grain_force
    etas = 'gr0 gr1'
    c = c
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
    etas = 'gr0 gr1'
    execute_on = 'initial linear'
  [../]
  [./grain_force]
    type = ComputeGrainForceAndTorque
    grain_data = grain_center
    c = c
    execute_on = 'initial linear'
  [../]
  #[./grain_force_const]
  #  type = ConstantGrainForceAndTorque
  #  execute_on = 'initial linear'
  #  torque = '0.0 0.0 5.0 0.0 0.0 5.0'
  #  force = '0.2 0.3 0.0 -0.2 -0.3 0.0'
  #[../]
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
  [./elem_gr0]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
  [./Eleem_var_gr1]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
  [../]
  [./load]
    type = PlotFunction
    function = load
  [../]
  [./temp]
    type = PlotFunction
    function = temp
  [../]
  [./s11]
    type = ElementAverageValue
    variable = S11
  [../]
  [./s22]
    type = ElementAverageValue
    variable = S22
  [../]
  [./e11]
    type = ElementAverageValue
    variable = e11
  [../]
  [./e22]
    type = ElementAverageValue
    variable = e22
  [../]
  [./bnds_avg]
    type = ElementAverageValue
    variable = bnds
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.05
  end_time = 60
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  interval = 10
  print_linear_residuals = true
  csv = true
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
