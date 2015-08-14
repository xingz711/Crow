[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  nz = 0
  xmin = 0
  xmax = 50
  ymin = 0
  ymax = 50
  zmin = 0
  zmax = 0
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
      radius = 10.0
      invalue = 1.0
      outvalue = 0.0
      int_width = 4.0
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
      x1 = 25.0
      y1 = 25.0
      radius = 10.0
      invalue = 1.0
      outvalue = 0.0
      int_width = 4.0
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
    displacements = 'disp_x disp_y'
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

[AuxVariables]
  [./C11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
 [./matl_s11]
    type = RankTwoAux
    variable = s11_aux
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
  [../]
  [./matl_C11]
    type = RankFourAux
    variable = C11_aux
    rank_four_tensor = elasticity_tensor
    index_l = 0
    index_j = 0
    index_k = 0
    index_i = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeConcentrationDependentElasticityTensor
    block = 0
    c = c
    C1_ijkl = '6 6'
    C0_ijkl = '1 1'
    fill_method1 = symmetric_isotropic
    fill_method0 = symmetric_isotropic
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y'
  [../]
  [./consts]
    type = GenericConstantMaterial
    block = 0
    prop_names = ' L   kappa_eta  M   kappa_c'
    prop_values = '1.0 1.0        1.0 1.0'
  [../]
  [./free_energy_chem]
    type = DerivativeParsedMaterial
    block = 0
    f_name = Fa
    args = 'c eta'
    function = '(3*eta*eta+2*eta*eta*eta)*c^2*(c-1)^2 + eta^2*(1.0-eta)^2'
    derivative_order = 2
    enable_jit = true
  [../]
  [./elastic_free_energy]
    type = ElasticEnergyMaterial
    f_name = Fea
    block = 0
    args = 'c eta'
  [../]
  [./free_energy_total]
    type = DerivativeSumMaterial
    block = 0
    f_name = F
    sum_materials = 'Fa Fea'
    args = 'c eta'
    derivative_order = 2
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
    type = PresetBC
    variable = disp_y
    boundary = top
    value = -0.5
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

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'

  l_max_its = 30
  nl_max_its = 10
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-8
  nl_abs_tol = 1.0e-10
  start_time = 0.0
  num_steps = 200
  time_step = 0.1
[]

[Outputs]
  output_initial = true
  exodus = true
  output_on = timestep_end
  [./console]
    type = Console
    perf_log = true
    output_on = 'initial timestep_end failed nonlinear'
  [../]
[]

#[ICs]
#  [./c_IC]
#    int_width = 0.2
#    x1 = 0
#    y1 = 0
#    radius = 0.25
#    outvalue = 0
#    variable = c
#    invalue = 1
#    type = SmoothCircleIC
#  [../]
#[]
