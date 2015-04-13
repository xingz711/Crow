[GlobalParams]
  var_name_base = gr
  op_num = 12
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 20
  ymax = 22.8
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
  [./PolycrystalVariables]
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
    block = 0
  [../]
[]

[Functions]
  active = 'disp'
  [./Load]
    type = PiecewiseLinear
    y = '0.0 0.0 65e-6 65e-6 0.0'
    x = '0 3 10 50 60'
  [../]
  [./disp]
    type = PiecewiseConstant
    data_file = Load_pureW_mod2273.csv
    format = columns
  [../]
[]

[Kernels]
  [./CHBulk]
    type = CHChemPotential
    variable = c
    mob_name = D
    block = 0
    Dmob_name = DD
  [../]
  [./Cdot]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./ChInt]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
  [./PolycrystalSinteringKernel]
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
  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./Periodic_c]
      variable = 'c gr0 gr1'
      auto_direction = 'x y'
    [../]
  [../]
  [./BC_x]
    type = PresetBC
    variable = disp_x
    boundary = 'bottom left right'
    value = 0.0
  [../]
  [./BC_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
  [./Top_load]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = disp
  [../]
[]

[Materials]
  active = 'AC_mat PFGrowth Eigen_strn elastc_energy'
  [./PFDiff]
    type = PFDiffusion
    block = 0
    rho = c
    kappa = 10
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '10.0 1.0'
  [../]
  [./PFGrowth]
    type = PFDiffusionGrowth
    block = 0
    kappa = 10
    rho = c
    v = 'gr0 gr1'
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
    variable = c
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 3743
  dt = 0.5
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  nl_abs_tol = 1e-9
  l_tol = 1e-04
  scheme = bdf2
[]

[Outputs]
  exodus = true
  output_final = true
  output_on = 'timestep_end initial'
  [./console]
    type = Console
    perf_log = true
    output_initial = true
    output_file = true
  [../]
[]

[ICs]
  active = 'PolycrystalICs multi_circle'
  [./2pdens]
    max = 0.01
    radius = '2.0 2.0'
    variable = c
    type = TwoParticleDensityIC
    block = 0
  [../]
  [./PolycrystalICs]
    [./MultiSmoothParticleIC]
      x_positions = '11.4 6.7 3.6 2.5'
      z_positions = '0 0 0 0 '
      op_num = 4
      radii = '2 2 2 2 '
      y_positions = '12.8 8.5 3.9 17.5'
      3D_spheres = false
      int_width = 0.5
    [../]
  [../]
  [./multi_circle]
    x_positions = '11.4 6.7 3.6 2.5'
    int_width = 0.5
    z_positions = '0 0 0 0'
    y_positions = '12.8 8.5 3.9 17.5'
    radii = '2 2 2 2'
    3D_spheres = false
    outvalue = 0.00001
    variable = c
    invalue = 1.0
    type = SpecifiedSmoothCircleIC
    block = 0
  [../]
  [./rnd_multic]
    int_width = 0.5
    bubspac = '5.0 '
    rand_seed = 3000
    numbub = 10
    3D_spheres = false
    outvalue = 0.0001
    variable = c
    radius = 2.0
    invalue = 1.0
    type = MultiSmoothCircleIC
    block = 0
  [../]
  [./multi_particle]
    int_width = 0.5
    bubspac = 5
    rand_seed = 10000
    numbub = 4
    3D_spheres = false
    outvalue = 0.0001
    variable = c
    radius = 2.0
    invalue = 1.0
    type = MultiSmoothParticleIC
    numtries = 1000000
  [../]
[]

