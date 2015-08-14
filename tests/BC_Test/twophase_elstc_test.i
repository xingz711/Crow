[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 28
  ny = 20
  nz = 0
  xmin = 10
  xmax = 40
  ymin = 15
  ymax = 35
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 25.0
      y1 = 25.0
      radius = 6.0
      invalue = 0.9
      outvalue = 0.1
      int_width = 3.0
    [../]
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
  [./eta]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 30.0
      y1 = 25.0
      radius = 4.0
      invalue = 0.9
      outvalue = 0.1
      int_width = 2.0
    [../]
  [../]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./TensorMechanics]
    disp_x = disp_x
    disp_y = disp_y
  [../]
  [./detadt]
    type = TimeDerivative
    variable = eta
  [../]
  [./ACBulk]
    type = ACParsed
    variable = eta
    f_name = F
  [../]
  [./ACInterface]
    type = ACInterface
    variable = eta
    kappa_name = kappa_eta
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = F
    kappa_name = kappa_c
    w = w
    args = eta
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
      variable = 'c eta w'
    [../]
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./top_y]
    type = PresetBC
    variable = disp_y
    boundary = 'top'
    value = 5
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = 'left'
    value = 0
  [../]
[]

[Materials]
  [./consts]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_eta'
    prop_values = '1 1        '
  [../]
  [./consts2]
    type = PFMobility
    block = 0
    kappa = 1
    mob = 1
  [../]
  [./switching]
    type = SwitchingFunctionMaterial
    block = 0
    eta = eta
    h_order = SIMPLE
  [../]
  [./barrier]
    type = BarrierFunctionMaterial
    block = 0
    eta = eta
    g_order = SIMPLE
  [../]
  [./free_energy_A]
    type = DerivativeParsedMaterial
    block = 0
    f_name = Fa
    args = c
    function = '(c-0.1)^2*(c-1)^2 + c*0.01'
    derivative_order = 2
    enable_jit = true
  [../]
  [./free_energy_B]
    type = DerivativeParsedMaterial
    block = 0
    f_name = Fb
    args = c
    function = 'c^2*(c-0.9)^2 + (1-c)*0.01'
    derivative_order = 2
    enable_jit = true
  [../]
  [./eigenstrain_a]
    type = LinearElasticMaterial
    base_name = phasea
    block = 0
    disp_y = disp_y
    disp_x = disp_x
    # Stiffness tensor lambda, mu values
    C_ijkl = '7 7'
    fill_method = symmetric_isotropic
  [../]
  [./elastic_free_energy_a]
    type = ElasticEnergyMaterial
    base_name = phasea
    f_name = Fea
    block = 0
    args = ''
  [../]

  [./eigenstrain_b]
    type = LinearElasticMaterial
    base_name = phaseb
    block = 0

    # we use thermal expansion with a constant temperature to achieve a 10% oversize
    # commenting out the following 3 lines will switch off the oversize effect
    thermal_expansion_coeff = 0.1
    T0 = 0
    T = 1

    disp_y = disp_y
    disp_x = disp_x

    # Stiffness tensor lambda, mu values
    # Note that the two phases could have different stiffnesses.
    # Try reducing the precipitate stiffness (to '1 1') rather than making it oversized
    C_ijkl = '7 7'
    fill_method = symmetric_isotropic
  [../]
  [./elastic_free_energy_a]
    type = ElasticEnergyMaterial
    base_name = phasea
    f_name = Fea
    block = 0
    args = 'c eta'
  [../]
  [./elastic_free_energy_b]
    type = ElasticEnergyMaterial
    base_name = phaseb
    f_name = Feb
    block = 0
    args = 'c eta'
  [../]
  [./free_energy]
    type = DerivativeTwoPhaseMaterial
    block = 0
    f_name = F
    fa_name = Fa
    fb_name = Fb
    args = c
    eta = eta
    derivative_order = 2
    outputs = exodus
    output_properties = 'F dF/dc dF/deta d^2F/dc^2 d^2F/dcdeta d^2F/deta^2'
  [../]
  [./global_stress]
    type = TwoPhaseStressMaterial
    block = 0
    base_A = phasea
    base_B = phaseb
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  l_max_its = 15
  l_tol = 1.0e-4
  nl_max_its = 10
  nl_rel_tol = 1.0e-11
  start_time = 0.0
  num_steps = 10
  dt = 0.1
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]
