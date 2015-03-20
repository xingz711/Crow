# 
# Example problem showing how to use the DerivativeParsedMaterial with CHParsed.
# The free energy is identical to that from CHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 60
  ymax = 60
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
  [../]
  [./eta]
    block = 0
    [./InitialCondition]
      value = 0.0
      variable = eta
      type = ConstantIC
      block = 0
    [../]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./R1]
    [./InitialCondition]
      variable = R1
      type = RandomIC
      block = 0
    [../]
  [../]
  [./R2]
    block = 0
    [./InitialCondition]
      variable = R2
      type = RandomIC
      block = 0
    [../]
  [../]
[]

[ICs]
  active = 'cIC'
  [./cIC]
    type = RandomIC
    variable = c
    max = 0.1
    min = 0.01
    block = 0
  [../]
  [./Const_c]
    variable = c
    value = 0.07
    type = ConstantIC
    block = 0
  [../]
[]

[Kernels]
  active = 'CH_Parsed c_dot CHint AC_bulk eta_dot AC_int VacancySource'
  [./c_dot]
    type = TimeDerivative
    variable = c
  [../]
  [./CH_Parsed]
    type = CHParsed
    variable = c
    f_name = fbulk
    mob_name = D
  [../]
  [./CH_Math]
    type = CHMath
    variable = c
  [../]
  [./CHint]
    type = CHInterface
    variable = c
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
  [../]
  [./AC_bulk]
    type = ACParsed
    variable = eta
    f_name = fbulk
    args = 'c '
  [../]
  [./eta_dot]
    type = TimeDerivative
    variable = eta
    block = 0
  [../]
  [./AC_int]
    type = ACInterface
    variable = eta
    block = 0
  [../]
  [./VacancySource]
    type = VacancySourceTermKernel
    variable = c
    c = c
    R2 = R2
    R1 = R1
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
      variable = 'c eta'
    [../]
  [../]
[]

[Materials]
  active = 'free_energy Temp_diffmat AC_mat'
  [./mat]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 0.5
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    block = 0
    f_name = fbulk
    args = c
    constant_names = 'kbT Ef eta'
    constant_expressions = '0.11 0.8 0.05'
    function = (eta-1)^2*(Ef*c+kbT*(c*log(c)+(1-c)*log(1-c)))+eta^2*(c-1)^2
    enable_jit = true
    tol_values = 0.01
    tol_names = c
  [../]
  [./Temp_diffmat]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = c
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '10.0 1.0 '
  [../]
[]

[Postprocessors]
  [./top]
    type = SideIntegralVariablePostprocessor
    variable = c
    boundary = top
  [../]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  dt = 2.0
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 20
  nl_rel_tol = 1e-9
  end_time = 20.0
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

