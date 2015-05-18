[GlobalParams]
  var_name_base = gr
  op_num = 8.0
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
  active = 'PolycrystalVariables c w'
  [./c]
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
  [./disp_x]
    block = 0
  [../]
  [./Disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  [./bnds]
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
    args = 'gr0  gr1 gr2 gr3 gr4 gr5 gr6 gr7'
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
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1 gr2 gr3 gr4  gr5 gr6 gr7'
  [../]
[]

[BCs]
  [./Periodic]
    active = 'periodic'
    [./top_bottom]
      primary = 0
      secondary = 2
      translation = '0 30.0 0'
    [../]
    [./left_right]
      primary = 1
      secondary = 3
      translation = '-30.0 0 0'
    [../]
    [./periodic]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
    [../]
  [../]
[]

[Materials]
  active = 'AC_mat temp sum CH_mat free_energy'
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
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
    third_derivatives = false
    f_name = S
  [../]
  [./CH_mat]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1'
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
    constant_expressions = 0.11
    fail_on_evalerror = true
    function = kbT*(c*log(c)+(1-c)*log(1-c))
    f_name = T
    args = 'c '
    constant_names = kbT
    tol_values = 1e-3
    tol_names = c
    third_derivatives = false
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S T'
    third_derivatives = false
    args = 'c gr0 gr1'
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
  end_time = 50
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

[ICs]
  active = 'PolycrystalICs multip'
  [./PolycrystalICs]
    [./MultiSmoothParticleIC]
      x_positions = '4.0 11.0 18.0 25.0 4.0 11.0 18.0 25.0'
      z_positions = '0. 0. 0. 0. 0. 0. 0. 0.'
      radii = '3.0 3.0 3.0 3.0 3.0 3.0 3.0 3.0'
      y_positions = '4.0 4.0 4.0 4.0 11.0 11.0 11.0 11.0'
      3D_spheres = false
      int_width = 1.0
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
    x_positions = '4.0 11.0 18.0 25.0 4.0 11.0 18.0 25.0'
    int_width = 1.0
    z_positions = '0. 0. 0. 0. 0. 0. 0. 0.'
    y_positions = '4.0 4.0 4.0 4.0 11.0 11.0 11.0 11.0'
    radii = '3.0 3.0 3.0 3.0 3.0 3.0 3.0 3.0'
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
[]

