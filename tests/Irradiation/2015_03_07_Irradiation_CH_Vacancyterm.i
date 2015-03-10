[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 40
  nz = 0
  xmax = 50
  ymax = 20
  zmax = 0
[]

[Variables]
  active = 'c e'
  [./c]
    order = THIRD
    family = HERMITE
    block = 0
  [../]
  [./e]
    block = 0
  [../]
  [./phi1]
    block = 0
  [../]
  [./phi2]
    block = 0
  [../]
[]

[AuxVariables]
  active = 'Energy R2 R1'
  [./Energy]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./eta0]
  [../]
  [./eta1]
  [../]
  [./R1]
  [../]
  [./R2]
  [../]
[]

[Kernels]
  active = 'AC_int c_dot e_dot CH_Parsed ACbulk CH_int CH_Pv'
  [./CH_Parsed]
    type = CHParsed
    variable = c
    mob_name = D
    f_name = F
    args = e
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
    c = e
    block = 0
    R2 = R2
    R1 = R1
  [../]
  [./CH_Sv]
    type = VacancyAnnihilationKernel
    variable = c
    c = e
    ceq = 1.13e-4
    v = 'eta0 eta1'
    block = 0
  [../]
  [./AC_Pv]
    type = VacancySourceTermKernel
    variable = e
    c = e
    R2 = R2
    block = 0
    R1 = R1
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
      variable = 'c e '
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
    tol_values = 0.1
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
    prop_names = 'L kappa_op S'
    prop_values = '10.0 5.0e-3 100'
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  dt = 0.1
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 50
  scheme = bdf2
[]

[Outputs]
  exodus = true
  show = 'c e Energy'
  tecplot = true
  output_on = 'timestep_end initial'
  [./console]
    type = Console
    perf_log = true
    fit_mode = 120
    output_file = true
    output_on = 'timestep_end nonlinear failed initial'
  [../]
[]

[ICs]
  active = 'R2 R1 circle circle_e'
  [./R1]
    variable = R1
    type = RandomIC
    block = 0
  [../]
  [./R2]
    variable = R2
    type = RandomIC
  [../]
  [./ConstIC]
    variable = c
    value = 1.13e-4
    type = ConstantIC
    block = 0
  [../]
  [./const_e]
    variable = e
    value = 0.0
    type = ConstantIC
    block = 0
  [../]
  [./circle]
    x1 = 25
    y1 = 10
    radius = 7
    outvalue = 1.13e-4
    variable = c
    invalue = 0.999
    type = SmoothCircleIC
  [../]
  [./circle_e]
    x1 = 25
    y1 = 10
    radius = 7.0
    outvalue = 0.0
    variable = e
    invalue = 1.0
    type = SmoothCircleIC
  [../]
[]

