[GlobalParams]
  var_name_base = gr
  op_num = 2.0
  use_displaced_mesh = true
  outputs = exodus
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
[]

[Preconditioning]
  [./SMP]
    type = SMP
    off_diag_column = 'c w c   c   gr0 gr1'
    off_diag_row = 'w c gr0 gr1 c   c'
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
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1'
    [../]
  [../]
[]

[Materials]
  active = 'mob free_energy constant_mat'
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    f_name = F
    derivative_order = 2
    outputs = console
  [../]
  [./mob]
    type = SinteringMobility
    block = 0
    v = 'gr0 gr1'
    outputs = console
    Em = 2.0
    Qs = 3.14
    GBmob0 = 0.05076
    surface_energy = 9.32947
    c = c
    Ds0 = 1.44
    Qgb = 4.143
    Q = 4.143
    T = 1500
    GB_energy = 6.8653
    omega = 1.855013e-29
    Dgb0 = 0.05076
    D0 = 1.44
    int_width = 2
    gbindex = 1
    surfindex = 1
    length_scale = 1e-09
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
    #prop_names = 'A B L  kappa_op'
    #prop_values = '16.0 1.0 1.0 0.5'
    prop_names = 'D'
    prop_values = '0.5'
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
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options = '-ksp_converged_reason'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  end_time = 100
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
    growth_factor = 1.5
  [../]
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
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
    type = RandomIC
    variable = gr1
  [../]
  [./multip]
    type = RandomIC
    variable = c
  [../]
  [./ic_gr0]
    type = RandomIC
    variable = gr0
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
