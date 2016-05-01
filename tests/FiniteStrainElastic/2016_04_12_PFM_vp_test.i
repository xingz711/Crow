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

[GlobalParams]
  displacements = 'disp_x disp_y'
  var_name_base = gr
  op_num = 2.0
  block = 0
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
  [./peeq]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./pe11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./pe22]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./TensorMechanics]
  [../]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1'
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
  [./pe11]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = pe11
    index_i = 0
    index_j = 0
  [../]
    [./pe22]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = pe22
    index_i = 1
    index_j = 1
  [../]
  [./eqv_plastic_strain]
    type = MaterialRealAux
    property = eqv_plastic_strain
    variable = peeq
  [../]
[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -1.0 -1.0'
    x = '0.0 0.1 2.0'
  [../]
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c w gr0 gr1'
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
    function = '-t'
  [../]
[]

#[UserObjects]
#  [./flowstress]
#    type = HEVPLinearHardening
#    yield_stress = 300
#    slope = 1000
#    intvar_prop_name = ep_eqv
#  [../]
#  [./flowrate]
#    type = HEVPFlowRatePowerLawJ2
#    reference_flow_rate = 0.0001
#    flow_rate_exponent = 10.0
#    flow_rate_tol = 1
#    strength_prop_name = flowstress
#  [../]
#  [./ep_eqv]
#     type = HEVPEqvPlasticStrain
#     intvar_rate_prop_name = ep_eqv_rate
#  [../]
#  [./ep_eqv_rate]
#     type = HEVPEqvPlasticStrainRate
#     flow_rate_prop_name = flowrate
#  [../]
#[]

[Materials]
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
    v = 'gr0 gr1'
    outputs = console
  [../]
  [./constant_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'A B  kappa_op kappa_c'
    prop_values = '16.0 1.0 1.0 10.0'
  [../]
  [./AC_prop]
    type = ParsedMaterial
    block = 0
    f_name = L
    function = D*10.0
    material_property_names = D
  [../]
  [./finitestrain_phase1]
    type = FiniteStrainRatePlasticMaterial
    block = 0
    #base_name = phase1
    fill_method = symmetric_isotropic
    disp_x = disp_x
    disp_y = disp_y
    C_ijkl = '35.46 30.141'
    yield_stress = '0. 2.5 0.04 2.743 0.06 5.682 0.12 7.837 0.2 6.858'
    #yield_stress = '0. 5.0 0.05 6.1 0.1 6.8 0.38 8.1 0.95 9.2 2. 9.5'
    ref_pe_rate = 0.01
    exponent = 3.0
  [../]
  [./elstc_en_phase0]
    type = ElasticEnergyMaterial
    #base_name = phase0
    f_name = E
    block = 0
    args = 'c'
    derivative_order = 2
    outputs = console
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S E'
    args = 'c gr0 gr1'
    derivative_order = 2
    outputs = console
  [../]
  #[./viscop_damage]
  #  type = HyperElasticPhaseFieldIsoDamage
  #  #block = 1
  #  resid_abs_tol = 1e-18
  #  resid_rel_tol = 1e-8
  #  maxiters = 50
  #  max_substep_iteration = 5
  #  flow_rate_user_objects = 'flowrate'
  #  strength_user_objects = 'flowstress'
  #  internal_var_user_objects = 'ep_eqv'
  #  internal_var_rate_user_objects = 'ep_eqv_rate'
  #  numerical_stiffness = false
  #  damage_stiffness = 1e-8
  #  c = c
  #[../]
  #[./elasticity_tensor]
  #  type = ComputeElasticityTensor
  #  #block = 1
  #  C_ijkl = '120.0 80.0'
  #  fill_method = symmetric_isotropic
  #[../]
  #[./strain]
  #  type = ComputeFiniteStrain
  #  #block = 1
  #  displacements = 'disp_x disp_y'
  #[../]
[]

[ICs]
  [./ic_gr1]
    int_width = 2.0
    x1 = 25.0
    y1 = 10.0
    radius = 7.0
    outvalue = 0.0
    variable = gr1
    invalue = 1.0
    type = SmoothCircleIC
  [../]
  [./multip]
    x_positions = '10.0 25.0'
    int_width = 2.0
    z_positions = '0 0'
    y_positions = '10.0 10.0'
    radii = '7.0 7.0'
    3D_spheres = false
    outvalue = 0.001
    variable = c
    invalue = 0.999
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./ic_gr0]
    int_width = 2.0
    x1 = 10.0
    y1 = 10.0
    radius = 7.0
    outvalue = 0.0
    variable = gr0
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

#[Postprocessors]
#  [./resid_x]
#    type = NodalSum
#    variable = resid_x
#    boundary = 2
#  [../]
#  [./resid_y]
#    type = NodalSum
#    variable = resid_y
#    boundary = 2
#  [../]
#[]

[Preconditioning]
  [Preconditioning]
    [./SMP]
      type = SMP
      off_diag_column = 'c w c   c   gr0 gr1  disp_x disp_y'
      off_diag_row    = 'w c gr0 gr1 c   c    disp_y disp_x'
    [../]
  []
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly       lu           1'

  nl_rel_tol = 1e-8
  l_max_its = 100
  nl_max_its = 100

  dt = 0.01
  dtmin = 1e-4
  #num_steps = 20
  end_time = 18
[]

[Outputs]
  exodus = true
[]
