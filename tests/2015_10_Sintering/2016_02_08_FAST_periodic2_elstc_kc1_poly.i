# [VectorPostprocessors]
# [./neck]
# type = LineValueSampler
# variable = 'c bnds'
# start_point = '17.5 0.0 0.0'
# end_point = '17.5 20.0 0.0'
# sort_by = id
# num_points = 40
# [../]
# []

[GlobalParams]
  # use_displaced_mesh = true
  # outputs = exodus
  var_name_base = gr
  op_num = 18.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 224
  ny = 120
  nz = 0
  xmin = 0.0
  xmax = 56.0
  ymin = 0.0
  ymax = 30.0
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
[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -1.5 -1.5'
    x = '0.0 30.0 100.0'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_column = 'c w c   c   c    c   c   c   c   c   c  c    c    c   c    c     c   c     c     c gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17 disp_x disp_y'
    off_diag_row =    'w c gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17 c   c    c   c   c   c  c    c  c   c  c    c   c    c     c   c     c     c   disp_y disp_x'
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
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
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op  kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op kappa_op'
    interfacial_vars = 'c  gr0        gr1     gr2       gr3     gr4       gr5     gr6       gr7     gr8       gr9      gr10     gr11     gr12      gr13    gr14      gr15     gr16     gr17 '
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
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
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
  # elastic properties for phase with c =1
  # elastic properties for phase with c = 0
  # switching function for elastic energy calculation
  # total elastic energy calculation
  # gloabal Stress
  # total energy
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
    f_name = S
    derivative_order = 2
    outputs = console
  [../]
  [./CH_mat]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
    outputs = console
  [../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'A B  kappa_op kappa_c'
    prop_values = '16.0 1.0 1.0 1.0'
  [../]
  [./AC_prop]
    type = ParsedMaterial
    block = 0
    f_name = L
    function = D
    material_property_names = D
  [../]
  [./elasticity_tensor_phase1]
    type = ComputeElasticityTensor
    base_name = phase1
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '35.46 30.141'
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
    args = c
    derivative_order = 2
  [../]
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
    args = c
    derivative_order = 2
  [../]
  [./switching]
    type = SwitchingFunctionMaterial
    block = 0
    function_name = h
    eta = c
    h_order = SIMPLE
  [../]
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
  [./global_stress]
    type = TwoPhaseStressMaterial
    block = 0
    base_A = phase1
    base_B = phase0
    h = h
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S E'
    args = 'c gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17'
    derivative_order = 2
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
  [./gr0]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
  [./gr1]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
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
  # dt = 0.01
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  nl_abs_tol = 1e-10
  end_time = 200
  #[./Adaptivity]
  #  refine_fraction = 0.7
  #  coarsen_fraction = 0.1
  #  max_h_level = 2
  #  initial_adaptivity = 1
  #[../]
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.005
    growth_factor = 1.2
  [../]
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  csv = true
  gnuplot = true
  interval = 50
  [./console]
    type = Console
    perf_log = true
    output_on = 'initial timestep_end failed nonlinear linear'
  [../]
[]

[ICs]
  [./multip]
    x_positions = '7.0 15.5 24.0 32.5 41.0 49.5 7.0 15.5 24.0 32.5 41.0 49.0 7.0 15.5 24.0 32.5 41.0 49.5'
    int_width = 1.0
    z_positions = '0 0'
    y_positions = '6.0 6.0 6.0 6.0 6.0 6.0 14.5 14.5 14.5 14.5 14.5 14.5 23.0 23.0 23.0 23.0 23.0 23.0 '
    radii =       '4.0 4.0 4.0 4.0 4.0 4.0  4.0  4.0 4.0   4.0  4.0  4.0 4.0  4.0  4.0  4.0  4.0  4.0 '
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr0]
    int_width = 1.0
    x1 = 7.0
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr1]
    int_width = 1.0
    x1 = 15.5.0
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr2]
    int_width = 1.0
    x1 = 24.0
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr2
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr3]
    int_width = 1.0
    x1 = 32.5
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr3
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr4]
    int_width = 1.0
    x1 = 7.0
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr4
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr5]
    int_width = 1.0
    x1 = 15.5
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr5
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr6]
    int_width = 1.0
    x1 = 24.0
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr6
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr7]
    int_width = 1.0
    x1 = 32.5
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr7
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr8]
    int_width = 1.0
    x1 = 41.0
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr8
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr9]
    int_width = 1.0
    x1 = 41.0
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr9
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr10]
    int_width = 1.0
    x1 = 7.0
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr10
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr11]
    int_width = 1.0
    x1 = 15.5
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr11
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr12]
    int_width = 1.0
    x1 = 24.0
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr12
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr13]
    int_width = 1.0
    x1 = 32.5
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr13
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr14]
    int_width = 1.0
    x1 = 41.0
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr14
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr15]
    int_width = 1.0
    x1 = 49.5
    y1 = 6.0
    radius = 4.0
    outvalue = 0.0
    variable = gr15
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr16]
    int_width = 1.0
    x1 = 49.5
    y1 = 14.5
    radius = 4.0
    outvalue = 0.0
    variable = gr16
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_gr17]
    int_width = 1.0
    x1 = 49.5
    y1 = 23.0
    radius = 4.0
    outvalue = 0.0
    variable = gr17
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
