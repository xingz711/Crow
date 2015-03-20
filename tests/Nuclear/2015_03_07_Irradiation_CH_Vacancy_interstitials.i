[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 10
  ymax = 10
  zmax = 0
[]

[Variables]
  [./cv]
    order = THIRD
    family = HERMITE
  [../]
  [./ci]
    order = THIRD
    family = HERMITE
  [../]
  [./eta]
  [../]
[]

[Kernels]
  [./CH_cv]
    type = CHParsed
    variable = cv
    mob_name = D
    f_name = F
    args = 'ci eta'
    block = 0
  [../]
  [./CH_ci]
    type = CHParsed
    variable = ci
    mob_name = D
    f_name = F
    args = 'cv eta'
  [../]
  [./AC_eta]
    type = ACParsed
    variable = eta
    f_name = F
    args = 'ci cv'
  [../]
  [./cv_dot]
    type = TimeDerivative
    variable = cv
    block = 0
  [../]
  [./ci_dot]
    type = TimeDerivative
    variable = ci
    block = 0
  [../]
  [./eta_dot]
    type = TimeDerivative
    variable = eta
  [../]
  [./Int_cv]
    type = CHInterface
    variable = cv
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
  [./Int_ci]
    type = CHInterface
    variable = ci
    mob_name = D
    kappa_name = kappa_c
    grad_mob_name = grad_D
    block = 0
  [../]
  [./eta_int]
    type = ACInterface
    variable = eta
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      variable = 'ci cv eta'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./Mobility]
    type = TempDiffusion
    block = 0
    c = cv
    Dv = 1.0
  [../]
  [./Energy_Matrix]
    type = DerivativeParsedMaterial
    block = 0
    constant_expressions = '0.8 0.8 0.11'
    function = (eta-1)^2*(Efv*cv+Efi*ci+kbT*(cv*log(cv)+ci*log(ci)+(1-cv-ci)*log(1-cv-ci)))
    f_name = Fs
    args = 'ci cv eta'
    constant_names = 'Efv Efi kbT '
    tol_values = '0.01 0.01'
    tol_names = 'cv ci'
  [../]
  [./Energy_void]
    type = DerivativeParsedMaterial
    block = 0
    function = eta^2*((cv-1)^2+ci^2)
    f_name = Fv
    args = 'ci cv eta'
  [../]
  [./Free Energy]
    type = DerivativeSumMaterial
    block = 0
    args = 'ci cv eta'
    sum_materials = 'Fs Fv'
  [../]
  [./Mobility_i]
    type = TempDiffusion
    block = 0
    Dv = 1.0
    c = ci
  [../]
  [./Constants]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '10.0 1.0'
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
  dt = 0.001
  l_max_its = 30
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  end_time = 10
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  exodus = true
  tecplot = true
  output_final = true
  [./console]
    type = Console
    additional_output_on = initial
  [../]
[]

[ICs]
  [./void_cv]
    int_width = 0.2
    x1 = 5.0
    y1 = 5.0
    3D_spheres = false
    outvalue = 0.070
    variable = cv
    radius = 1.0
    invalue = 0.999
    type = SmoothCircleIC
    block = 0
  [../]
  [./IC_ci]
    int_width = 0.2
    x1 = 5.0
    y1 = 5.0
    radius = 1.0
    outvalue = 6.9e-4
    variable = ci
    invalue = 1e-9
    type = SmoothCircleIC
    block = 0
  [../]
  [./IC_eta]
    int_width = 0.2
    x1 = 5.0
    y1 = 5.0
    radius = 1.0
    outvalue = 0.0
    variable = eta
    invalue = 1.0
    type = SmoothCircleIC
    block = 0
  [../]
[]

