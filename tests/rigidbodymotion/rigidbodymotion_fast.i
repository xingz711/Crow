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
  active = 'c w'
  [./c]
  [../]
  [./w]
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
  [./gr0]
  [../]
  [./gr1]
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
  active = 'wres cres time'
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
    kappa_names = 'kappa_c '
    interfacial_vars = 'c '
  [../]
[]

[Materials]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    f_name = F
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
  print_linear_residuals = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
    file_base = comb_multip
  [../]
[]

[ICs]
  [./ic_eta0]
    int_width = 2.0
    x1 = 15.0
    y1 = 0.0
    radius = 8.0
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./ic_eta1]
    int_width = 2.0
    x1 = 15.0
    y1 = 15.0
    radius = 8.0
    outvalue = 0.0
    variable = gr1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./multip]
    x_positions = '15.0 15.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '0.0 15.0 '
    radii = '8.0 8.0 '
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]

