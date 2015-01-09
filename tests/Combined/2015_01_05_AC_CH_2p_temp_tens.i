[GlobalParams]
  var_name_base = gr
  name = 'global var'
  op_num = 2
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 80
  xmax = 40
  ymax = 40
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
  [./Disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  [./T]
    block = 0
  [../]
[]

[Functions]
  [./Temp_profile]
    type = PiecewiseLinear
    y = '300.0 300.0 1500.0 1500.0 300.0 '
    x = '0.0 14.0 22.0 52.0 60.0'
  [../]
[]

[Kernels]
  [./CHBulk]
    type = CHChemPotential
    variable = c
    mob_name = D
    block = 0
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
  [./CHTemp]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = 'c T'
    block = 0
  [../]
  [./TensorMechanics]
    disp_y = Disp_y
    disp_x = disp_x
  [../]
[]

[AuxKernels]
  [./Temp_calc]
    type = FunctionAux
    variable = T
    function = Temp_profile
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./Periodic_c]
      variable = c
      auto_direction = 'x y'
    [../]
    [./periodic_gr0]
      variable = gr0
      auto_direction = 'x y '
    [../]
    [./periodic_gr1]
      variable = gr1
      auto_direction = 'x y '
    [../]
  [../]
  [./bc_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
  [./BC_y]
    type = PresetBC
    variable = Disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  active = 'AC_mat PFGrowth CHtemp_mat Eigen_strain energy'
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
  [./CHtemp_mat]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = 8.134e-23
    function = R*T*(c*log(c)+(1-c)*log(1-c))
    args = 'c T'
    constant_names = 'R '
    tol_values = 0.001
    tol_names = c
  [../]
  [./Eigen_strain]
    type = SimpleEigenStrainMaterial
    block = 0
    c = c
    disp_y = Disp_y
    disp_x = disp_x
    epsilon0 = 0.5
    C_ijkl = '1.0e6 0.0 0.0 1.0e6  0.0 1.0e6 0.5e6 0.5e6 0.5e6'
    T = T
  [../]
  [./energy]
    type = ElasticEnergyMaterial
    block = 0
    args = c
  [../]
[]

[Postprocessors]
  [./Elem_Avg]
    type = ElementIntegralVariablePostprocessor
    variable = c
    block = 0
  [../]
  [./Side_Int_var]
    type = SideIntegralVariablePostprocessor
    variable = c
    boundary = top
  [../]
[]

[Executioner]
  type = Transient
  dt = 2
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  nl_abs_tol = 1e-9
  end_time = 60
  l_tol = 1e-04
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  exodus = true
  output_final = true
  [./console]
    type = Console
    perf_log = true
    output_initial = true
    output_file = true
  [../]
[]

[ICs]
  block = 0
  [./2pdens]
    max = 0.005
    radius = '9.0 7.0 '
    variable = c
    type = TwoParticleDensityIC
    block = 0
    min = 0.001
  [../]
  [./PolycrystalICs]
    [./TwoParticleGrainsIC]
    [../]
  [../]
[]

