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
  [./TensorMechanics]
    disp_y = Disp_y
    disp_x = disp_x
  [../]
  [./Elstc_gr0]
    type = ACParsed
    variable = gr0
    f_name = E
    args = 'c gr1 gr2 gr3 gr4 gr5 gr6 gr7'
  [../]
  [./Elstc_gr1]
    type = ACParsed
    variable = gr1
    f_name = E
    args = 'c gr0 gr2 gr3 gr4 gr5 gr6 gr7'
  [../]
  [./Elstc_gr2]
    type = ACParsed
    variable = gr2
    f_name = E
    args = 'c gr0 gr1 gr3 gr4 gr5 gr6 gr7'
  [../]
  [./Elstc_gr3]
    type = ACParsed
    variable = gr3
    f_name = E
    args = 'c gr0 gr1 gr2 gr4 gr5 gr6 gr7'
  [../]
  [./Elstc_gr4]
    type = ACParsed
    variable = gr4
    f_name = E
    args = 'c gr0 gr1 gr1 gr3 gr5 gr6 gr7'
  [../]
  [./Elstc_gr5]
    type = ACParsed
    variable = gr5
    f_name = E
    args = 'c gr0 gr1 gr2 gr3 gr4 gr6 gr7'
  [../]
  [./Elstc_gr6]
    type = ACParsed
    variable = gr6
    f_name = E
    args = 'c gr0 gr1 gr2 gr3 gr4 gr5 gr7'
  [../]
  [./Elstc_gr7]
    type = ACParsed
    variable = gr7
    f_name = E
    args = 'c gr0 gr1 gr2 gr3 gr4 gr5 gr6'
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
  [./Disp_x]
    type = PresetBC
    variable = disp_x
    boundary = 'right left'
    value = 0.0
  [../]
  [./Disp_y]
    type = PresetBC
    variable = Disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  active = 'AC_mat Eigen temp sum CH_mat free_energy Elstc_en'
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
    derivative_order = 2
  [../]
  [./CH_mat]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
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
    derivative_order = 2
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S T E'
    args = 'c gr0 gr1'
    f_name = 'F '
    derivative_order = 2
  [../]
  [./Eigen]
    type = PFEigenStrainMaterial1
    block = 0
    c = c
    disp_y = Disp_y
    disp_x = disp_x
    epsilon0 = 0.05
    C_ijkl = '7.0 7.0'
    fill_method = symmetric_isotropic
    v = 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
  [../]
  [./Elstc_en]
    type = ElasticEnergyMaterial
    block = 0
    f_name = E
    args = 'c gr0 gr1 gr2 gr3 gr4 gr5  gr6 gr7'
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
      z_positions = 0
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
    z_positions = 0
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

