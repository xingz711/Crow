[GlobalParams]
  var_name_base = gr
  op_num = 2.0
  #use_displaced_mesh = true
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
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
    block = 0
  [../]
  [./disp_y]
    block = 0
  [../]
  [./c]
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -1.5 -1.5 -1.5'
    x = '0.0 30.0 45.0 60.0'
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
    use_displaced_mesh = true
  [../]
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

[BCs]
  [./symmy]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./symmx]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  #[./symmz]
  #  type = PresetBC
  #  variable = disp_z
  #  boundary = back
  #  value = 0
  #[../]
  [./load]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = load
  [../]
[]

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
    prop_names = 'A B L  kappa_op'
    prop_values = '16.0 1.0 1.0 0.5'
  [../]
  #[./elasticity_tensor]
  #  type = ComputeConcentrationDependentElasticityTensor
  #  #base_name = phase0
  #  block = 0
  #  fill_method0 = symmetric_isotropic
  #  fill_method1 = symmetric_isotropic
  #  C0_ijkl = '30.141 35.46'
  #  C1_ijkl = '10.0 10.0 '
  #  c = c
  #[../]
  #[./strain]
  #  type = ComputeFiniteStrain
  #  block = 0
  #  #base_name = phase0
  #  displacements = 'disp_x disp_y'
  #  use_displaced_mesh = true
  #[../]
  [./Finite_strain0]
    type = FiniteStrainElasticMaterial
    base_name = phase0
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '30.141 35.46'
    disp_x = disp_x
    disp_y = disp_y
    use_displaced_mesh = true
  [../]
  [./Finite_strain1]
    type = FiniteStrainElasticMaterial
    base_name = phase1
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '10.0 10.0'
    disp_x = disp_x
    disp_y = disp_y
    use_displaced_mesh = true
  [../]
  #[./elasticity_tensor1]
  #  type = ComputeElasticityTensor
  #  base_name = phase1
  #  block = 0
  #  fill_method = symmetric_isotropic
  #  C_ijkl = '10.0 10.0'
  #[../]
  #[./strain1]
  #  type = ComputeFiniteStrain
  #  block = 0
  #  base_name = phase1
  #  displacements = 'disp_x disp_y'
  #[../]
  #[./stress1]
  #  type = ComputeFiniteStrainElasticStress
  #  base_name = phase1
  #  block = 0
  #[../]
  [./elastic_en0]
    type = ElasticEnergyMaterial
    args = 'disp_x disp_y'
    base_name = phase0
    block = 0
    f_name = E0
    #outputs = exodus
  [../]
  [./elastic_en1]
    type = ElasticEnergyMaterial
    args = 'disp_x disp_y'
    base_name = phase1
    block = 0
    f_name = E1
    #outputs = exodus
  [../]
  #switching function for elastic energy calculation
  [./switching]
    type = SwitchingFunctionMaterial
    block = 0
    function_name = h
    eta = c
    h_order = SIMPLE
  [../]
  # total elastic energy calculation
  [./total_elastc_en]
    type = DerivativeTwoPhaseMaterial
    block = 0
    h = h
    g = 0.0
    W = 0.0
    eta = c
    f_name = E
    fa_name = E1
    fb_name = E0
    args = 'disp_x disp_y gr0 gr1'
    derivative_order = 2
  [../]
  # gloabal Stress
  [./global_stress]
    type = TwoPhaseStressMaterial
    block = 0
    base_A = phase1
    base_B = phase0
    h = h
  [../]
  # total energy
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    sum_materials = 'S E'
    args = 'c gr0 gr1 disp_x disp_y'
    derivative_order = 2
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.005
  end_time = 100
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 1
  [../]
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
    y_positions = '10.0 10.0 '
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

[Outputs]
  exodus = true
[]
