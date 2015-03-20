[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  nz = 0
  xmax = 100
  ymax = 100
  zmax = 0
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
  [./e]
    block = 0
  [../]
[]

[AuxVariables]
  [./Energy]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
[]

[Kernels]
  active = 'CH_int c_dot CH_Parsed ACbulk e_dot AC_int CH_Pv'
  [./CH_Parsed]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = c
  [../]
  [./c_dot]
    type = TimeDerivative
    variable = c
    block = 0
  [../]
  [./CH_int]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
  [../]
  [./ACbulk]
    type = ACParsed
    variable = e
    f_name = F
    args = 'c '
    block = 0
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
    block = 0
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
    block = 0
  [../]
  [./CH_Pv]
    type = VacancySourceTermKernel
    variable = c
    c = c
    Pcasc = 0.8
    block = 0
  [../]
  [./Ac_Pv]
    type = VacancySourceTermKernel
    variable = e
    c = e
    block = 0
  [../]
[]

[AuxKernels]
  [./TotalFreeEng]
    type = TotalFreeEnergy
    variable = Energy
    interfacial_vars = 'c e'
    kappa_names = 'kappa_c kappa_op'
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = 'c e'
      auto_direction = 'x y '
    [../]
  [../]
[]

[Materials]
  [./FreeEnergy]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = '0.5 1.0 8.6173324e-5 1276 1.0 1.0 1.13e-4'
    function = '(e-1)^2*(e+1)^2*(Ef*c+kb*T*(c*log(c)+(1-c)*log(1-c))) - A*(c-ceq)^2*e*(e+2)*(e-1)^2 + B*(c-1)^2*e^2'
    args = c
    constant_names = 'e Ef kb T A B ceq '
    tol_values = 0.001
    tol_names = c
  [../]
  [./Mobility]
    type = TempDiffusion
    block = 0
    c = c
    kappa = 2.5e-3
    Dv = 0.64
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '1.0 5.0e-3'
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.5
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 100
  scheme = bdf2
[]

[ICs]
  [./randomIC]
    variable = c
    max = 0.1
    type = RandomIC
    block = 0
  [../]
  [./Ramdom_e]
    variable = e
    max = 0.1
    type = RandomIC
    block = 0
  [../]
[]

