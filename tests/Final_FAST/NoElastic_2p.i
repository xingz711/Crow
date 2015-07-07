[GlobalParams]
  var_name_base = gr
  op_num = 2.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 30
  nz = 0
  xmin = 0.0
  xmax = 30.0
  ymin = 0.0
  ymax = 15
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
  [./T]
  [../]
[]

[Functions]
  [./temp]
    type = PiecewiseLinear
    y = '300.0 673.0 1673.0 1673.0 673.0 300.0'
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
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalSinteringKernel]
    c = c
    v = 'gr0 gr1 '
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
  [./T]
    type = FunctionAux
    variable = T
    function = temp
    block = 0
  [../]
[]

[Materials]
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
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '1.0 0.5'
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
    outputs = console
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S Ft'
    args = 'c gr0 gr1'
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
  [./elem_gr0]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
  [./Eleem_var_gr1]
    type = ElementIntegralVariablePostprocessor
    variable = gr1
  [../]
  [./temp]
    type = PlotFunction
    function = temp
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
  num_steps = 10000
  end_time = 60
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  interval = 10
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
    file_base = comb_multip
  [../]
[]

[ICs]
  active = 'PolycrystalICs multip'
  [./PolycrystalICs]
    [./MultiSmoothParticleIC]
      x_positions = '15.0 15.0 '
      z_positions = '0 0'
      radii = '7.0 7.0 '
      y_positions = '0.0 15.0 '
      int_width = 2.0
    [../]
  [../]
  [./2p_dens]
    radius = '5.0 5.0 '
    variable = c
    type = TwoParticleDensityIC
    block = 0
    tol = 1e-3
  [../]
  [./multip]
    x_positions = '15.0 15.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '0.0 15.0 '
    radii = '7.0 7.0 '
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]
