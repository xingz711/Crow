[GlobalParams]
  disp_x = disp_x
  disp_y = disp_y
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
    var_name_base = gr
    op_num = 2.0
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./bnds]
  [../]
[]

[Functions]
  [./Load]
    type = PiecewiseLinear
    y = '0.0 0.0 10.0 10.0'
    x = '0.0 5.0 10.0 50.0'
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
    args = 'gr0  gr1'
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
    v = 'gr0 gr1'
  [../]
  [./TensorMechanics]
  [../]
[]

[AuxKernels]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'gr0 gr1'
  [../]
[]

[BCs]
  active = 'PressureTM disp_y disp_x'
  [./disp_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
  [./disp_x]
    type = PresetBC
    variable = disp_x
    boundary = 'left right'
    value = 0.0
  [../]
  [./Load]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = Load
  [../]
  [./PressureTM]
    [./pressure]
      function = Load
      boundary = top
    [../]
  [../]
[]

[Materials]
  active = 'AC_mat Eigen temp Elstc_en sum CH_mat free_energy'
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
    tol_values = 0.001
    tol_names = c
    derivative_order = 2
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S T E'
    args = 'c gr0 gr1'
    derivative_order = 2
  [../]
  [./Eigen]
    type = PFEigenStrainMaterial1
    block = 0
    c = c
    fill_method = symmetric_isotropic
    epsilon0 = 0.05
    C_ijkl = '15.0 15.0'
    v = 'gr0 gr1'
    e_v = 0.01
  [../]
  [./Elstc_en]
    type = ElasticEnergyMaterial
    block = 0
    f_name = E
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
  [./PolycrystalICs]
    [./TwoParticleGrainsIC]
    [../]
  [../]
  [./2p_dens]
    radius = '5.0 5.0 '
    variable = c
    type = TwoParticleDensityIC
    block = 0
    tol = 0.001
  [../]
[]

