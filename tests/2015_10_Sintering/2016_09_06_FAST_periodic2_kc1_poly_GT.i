[GlobalParams]
  # use_displaced_mesh = true
  # outputs = exodus
  var_name_base = gr
  op_num = 4.0
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
[]

[AuxVariables]
  [./bnds]
  [../]
  [./total_en]
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
  [./active_bounds_elemental]
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
    off_diag_column = 'c w c   c   c    c  gr0 gr1 gr2 gr3'
    off_diag_row =    'w c gr0 gr1 gr2 gr3 c   c    c   c '
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1 gr2 gr3'
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
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1 gr2 gr3'
  [../]
  [./Total_en]
    type = TotalFreeEnergy
    variable = total_en
    kappa_names = 'kappa_c kappa_op kappa_op kappa_op kappa_op'
    interfacial_vars = 'c  gr0        gr1     gr2       gr3 '
  [../]
  [./unique_grains]
    type = FeatureFloodCountAux
    variable = unique_grains
    flood_counter = grain_tracker
    execute_on = 'initial timestep_begin'
    field_display = UNIQUE_REGION
  [../]
  [./var_indices]
    type = FeatureFloodCountAux
    variable = var_indices
    flood_counter = grain_tracker
    execute_on = 'initial timestep_begin'
    field_display = VARIABLE_COLORING
  [../]
  [./active_bounds_elemental]
    type = FeatureFloodCountAux
    variable = active_bounds_elemental
    field_display = ACTIVE_BOUNDS
    execute_on = 'initial timestep_begin'
    flood_counter = grain_tracker
  [../]
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1 gr2 gr3'
    [../]
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
    v = 'gr0 gr1 gr2 gr3'
    #f_name = S
    derivative_order = 2
    outputs = console
  [../]
  [./CH_mat]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1 gr2 gr3'
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
[]

[UserObjects]
  [./grain_tracker]
    type = GrainTracker
    connecting_threshold = 0.05
    compute_op_maps = true
    flood_entity_type = elemental
    execute_on = 'initial timestep_begin'
    outputs = none
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
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = total_en
  [../]
  [./free_en]
    type = ElementIntegralMaterialProperty
    mat_prop = F
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
  end_time = 100
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
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
  [./console]
    type = Console
    perf_log = true
    output_on = 'initial timestep_end failed nonlinear linear'
  [../]
[]

[ICs]
  [./ic_gr1]
    x_positions = '15.5 32.5'
    z_positions = '0.0 0.0'
    y_positions = '6.0 6.0'
    radii = '4.0 4.0'
    int_width = 1.0
    3D_spheres = false
    outvalue = 0
    variable = gr1
    invalue = 1
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./multip]
    x_positions = '7.0 15.5 24.0 32.5 7.0  15.5 24.0 32.5'
    z_positions = '0.0 0.0  0.0  0.0  0.0  0.0  0.0  0.0'
    y_positions = '6.0 6.0  6.0  6.0  14.5 14.5 14.5 14.5'
    radii = '4.0 4.0 4.0 4.0 4.0 4.0 4.0 4.0'
    int_width = 1.0
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr0]
    x_positions = '7.0 24.0'
    z_positions = '0.0 0.0'
    y_positions = '6.0 14.5'
    int_width = 1.0
    radii = '4.0 4.0'
    3D_spheres = false
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr2]
    x_positions = '24.0 7.0'
    z_positions = '0.0  0.0'
    y_positions = '6.0  14.5 '
    radii = '4.0 4.0 '
    int_width = 1.0
    3D_spheres = false
    outvalue = 0.0
    variable = gr2
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr3]
    x_positions = '15.5 32.5'
    z_positions = '0.0  0.0'
    y_positions = '14.5 14.5'
    radii = '4.0 4.0'
    int_width = 1.0
    3D_spheres = false
    outvalue = 0.0
    variable = gr3
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
