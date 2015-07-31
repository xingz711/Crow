[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 20
  xmax = 15
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./phi]
    order = FIRST
    family = LAGRANGE
  [../]
  [./chem_pot]
  [../]
[]

[AuxVariables]
  [./T]
  [../]
  [./total_frenergy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  active = 'phi T c_circle'
  [./c_circle]
    variable = c
    type = SmoothCircleIC
    x1 = 0.0
    y1 = 0.0
    radius = 6.0
    invalue = 0.6
    outvalue = 0.1
    int_width = 4.0
    block = 0
  [../]
  [./phi]
    variable = phi
    type = SmoothCircleIC
    x1 = 0.0
    y1 = 0.0
    radius = 6.0
    invalue = 1.0
    outvalue = 0.0
    int_width = 4.0
    block = 0
  [../]
  [./T]
    variable = T
    type = ConstantIC
    value = 300
  [../]
  [./c_rnd]
    variable = c
    max = 0.3
    type = RandomIC
    min = 0.0
  [../]
  [./c_const]
    variable = c
    type = ConstantIC
    value = .2
  [../]
[]

[Kernels]
  [./phi_dot]
    type = TimeDerivative
    variable = phi
  [../]
  [./phi_int]
    type = ACInterface
    variable = phi
    mob_name = L_phi
    kappa_name = k_phi
  [../]
  [./phi_bulk]
    type = ACParsed
    variable = phi
    mob_name = L_phi
    f_name = Ftotal
  [../]
  [./c_dot]
    type = CoupledImplicitEuler
    variable = chem_pot
    v = c
  [../]
  [./CH]
    type = SplitCHWRes
    variable = chem_pot
    mob_name = M_c
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = Ftotal
    kappa_name = k_c
    w = chem_pot
  [../]
[]

[AuxKernels]
  [./total_frenergy]
    type = TotalFreeEnergy
    variable = total_frenergy
    f_name = Ftotal
    interfacial_vars = 'c phi'
    kappa_names = 'k_c k_phi'
  [../]
[]

[Materials]
  [./ab1cd1]
    type = CalphadAB1CD1Material
    f_name = Falpha
    block = 0
    c = c
    T = T
    ep_lo = '-7827.595    125.64905    -24.1618    -0.00437791     34971.0' # HCP_Zr
    ep_hi = ' 8055.336   -243.791       18.3135    -0.034513     -734182.8'  # H2_gas'
    mix = ' -45965    41.6    0' # FCC_ZrH
    derivative_order = 2
    plog_tol = 1e-02
    scaling_factor = 4.4519e-04 # 1/Vm in mol/nm^3 and JtoeV conversion, Vm of zr is 14.2 cm^3/mol
  [../]
  [./ab1cd2]
    type = CalphadAB1CD2Material
    f_name = Fdelta
    block = 0
    c = c
    T = T
    ep_lo = ' -227.595    124.74905   -24.1618   -0.00437791     34971' # FCC_Zr
    ep_hi = ' 8055.336   -243.791      18.3135   -0.034513     -734182.8' # H2_gas
    pure_ep1 = '-7827.595    125.64905   -24.1618   -0.00437791     34971.0'  # HCP_Zr'
    mix = '-170490   208.2   -9.47' # FCC_ZrH2
    L0 = '  14385  -6.0'
    L1 = '-106445  87.3'
    plog_tol = 1e-02
    derivative_order = 2
    scaling_factor = 4.4519e-04 # 1/Vm in mol/nm^3 and JtoeV conversion, Vm of zr is 14.2 cm^3/mol
  [../]
  [./pf_constant_parameters]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'k_c k_phi'
    prop_values = '40 40'
  [../]
  [./d2Ftotaldc2]
    type = GenericMaterialMaterial
    block = 0
    prop_old_names = d^2Ftotal/dc^2
    prop_new_names = d2Ftotaldc2
  [../]
  [./M_c]
    type = ParsedMaterial
    block = 0
    constant_names = 'D0 Em kb R JtoeV Vm length_scale time_scale'
    constant_expressions = '7e-7 0.47 8.617343e-5 8.3144621 6.24150974e18 1.42e22 1e-9 1e-6' # m^2/s eV J/K J/K/mol eV/J nm^3/mol m s
    material_property_names = d2Ftotaldc2
    function = 'D0s:= D0*time_scale/length_scale^2; D:=D0s*exp(-Em/(kb*T)); Rs:=R*JtoeV; Vm*D/(Rs*T)'
    f_name = M_c
    args = T
  [../]
  [./M_ctest2]
    type = ParsedMaterial
    block = 0
    constant_names = 'D0 Em kb length_scale time_scale'
    constant_expressions = '7e-7 0.47 8.617343e-5 1e-9 1e-6' # m^2/s eV eV/K m s
    material_property_names = d2Ftotaldc2
    function = 'D0s:= D0*time_scale/length_scale^2; D:=D0s*exp(-Em/(kb*T)); abs(D/d2Ftotaldc2)'
    f_name = M_ctest2
    args = T
  [../]
  [./h_phi]
    type = SwitchingFunctionMaterial
    block = 0
    eta = phi
  [../]
  [./g_phi]
    type = BarrierFunctionMaterial
    block = 0
    eta = phi
  [../]
  [./total_energy]
    type = DerivativeTwoPhaseMaterial
    block = 0
    f_name = Ftotal
    args = 'c T'
    eta = phi
    fa_name = Falpha
    W = 10
    fb_name = Fdelta
    third_derivatives = false
    outputs = exodus
  [../]
  [./L]
    type = ParsedMaterial
    block = 0
    function = 1.0*M_c
    f_name = L_phi
    args = T
    material_property_names = M_c
  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
  [../]
  [./int_Ftotal]
    type = ElementIntegralVariablePostprocessor
    variable = total_frenergy
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
  num_steps = 20
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 15
  l_tol = 1e-4
  nl_max_its = 10
  nl_rel_tol = 1e-9
  dt = 0.01
  dtmax = 0.05
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
  csv = true
[]
