[GlobalParams]
  var_name_base = gr
  op_num = 2.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 30
  nz = 30.0
  xmin = 0.0
  xmax = 30.0
  ymin = 0.0
  ymax = 15.0
  zmax = 30.0
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
    block = 0
  [../]
  [./disp_y]
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
    f_name = S
    args = 'gr0 gr1'
  [../]
  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = M
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
    disp_y = disp_y
    disp_x = disp_x
  [../]
  [./Elstc_en]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = 'gr0 gr1'
    Dmob_name = DD
    has_MJac = true
  [../]
  [./ElstcEn_gr0]
    type = ACParsed
    variable = 'gr0 '
    f_name = F
    args = 'c gr1'
    block = 0
  [../]
  [./elstcEn_gr1]
    type = ACParsed
    variable = gr1
    f_name = F
    args = 'c gr0 '
    block = 0
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
    [../]
  [../]
[]

[Materials]
  active = 'constant free_energy AC_mat Temp Sum'
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
    third_derivatives = false
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
  [./Temp]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = 0.5
    function = kbT*(c*log(c)+(1-c)*log(1-c))
    f_name = G
    args = c
    constant_names = kbT
    tol_values = 1e-6
    tol_names = c
  [../]
  [./Sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'F G'
    third_derivatives = false
    f_name = S
    args = 'c  gr0 gr1'
  [../]
[./elastc_energy]
    type = ElasticEnergyMaterial
    block = 0
    outputs = console
    args = c
  [../]
  [./Eigen_strn]
    type = PFEigenStrainMaterial1
    block = 0
    c = c
    disp_y = disp_y
    disp_x = disp_x
    epsilon0 = 0.05
    C_ijkl = '3 1 1 3 1 3 1 1 1'
    v = 'gr0 gr1 '
    e_v = 0.01
  [../]
[]

[Postprocessors]
  [./Elem_Avg]
    type = ElementIntegralVariablePostprocessor
    variable = c
    block = 0
  [../]
  [./Side_Int_var]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = -pc_type
  petsc_options_value = lu
  l_max_its = 30
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.05
  num_steps = 100000
  end_time = 30
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
    op_num = 2.0
    radius = '5.0 5.0 '
    variable = c
    type = TwoParticleDensityIC
    block = 0
  [../]
[]

