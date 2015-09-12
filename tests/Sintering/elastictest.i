[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  nz = 0
  xmin = -10
  xmax = 10
  ymin = -10
  ymax = 10
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[MeshModifiers]
  [./center]
    type = AddExtraNodeset
    coord = '0 0'
    new_boundary = center
  [../]
[]

[AuxVariables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = FunctionIC
      function = '0.1*sin(2*x/10*3.14159265359)'
    [../]
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = FunctionIC
      function = '0.1*sin(2*y/10*3.14159265359)'
    [../]
  [../]
  [./Ftotal]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./Ftotal]
    type = TotalFreeEnergy
    # use_displaced_mesh = true
    variable = Ftotal
    interfacial_vars = c
    kappa_names = kappa_c
  [../]
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 0
      y1 = 0
      radius = 8
      int_width = 4
      invalue = 1
      outvalue = 0
    [../]
  [../]
[]

[Kernels]
  [./dt]
    type = TimeDerivative
    variable = c
  [../]
  [./bulk]
    type = CHParsed
    # use_displaced_mesh = true
    variable = c
    mob_name = M
    f_name = F
  [../]
  [./int]
    type = CHInterface
    # use_displaced_mesh = true
    variable = c
    mob_name = M
    kappa_name = kappa_c
  [../]
[]

[BCs]
  [./Periodic]
    [./All]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./consts]
    type = GenericConstantMaterial
    block = 0
    prop_names  = 'M  kappa_c'
    prop_values = '1  0.5'
  [../]

  # matrix phase
  [./elasticity_tensor]
    type = ComputeConcentrationDependentElasticityTensor
    c = c
    block = 0
    C0_ijkl = '1.0 1.0'
    C1_ijkl = '3.0 3.0'
    fill_method0 = symmetric_isotropic
    fill_method1 = symmetric_isotropic
  [../]
  [./smallstrain]
    type = ComputeSmallStrain
    block = 0
  [../]
  [./linearelastic_a]
    type = ComputeLinearElasticStress
    block = 0
  [../]
  [./elastic_free_energy]
    type = ElasticEnergyMaterial
    f_name = Fe
    block = 0
    args = 'c'
    derivative_order = 2
  [../]
  [./chemical_free_energy]
    type = DerivativeParsedMaterial
    f_name = Fc
    block = 0
    args = 'c'
    derivative_order = 2
    function = 'c^2*(1-c)^2'
  [../]
  [./sum]
    type = DerivativeSumMaterial
    block = 0
    f_name = F
    sum_materials = 'Fe Fc'
    args = c
  [../]
[]

[Postprocessors]
  # The total free enrgy of the simulation cell to observe the energy reduction.
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = Ftotal
  [../]
  [./free_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = F
  [../]
  [./chemical_free_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = Fc
  [../]
  [./elastic_free_energy]
    type = ElementIntegralMaterialProperty
    mat_prop = Fe
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
  scheme = 'bdf2'

  solve_type = PJFNK

  l_max_its = 30
  l_tol = 1.0e-6

  nl_max_its = 15
  nl_rel_tol = 1.0e-7
  nl_abs_tol = 1.0e-10

  start_time = 0.0
  end_time   = 100

  [./TimeStepper]
    type = IterationAdaptiveDT
    optimal_iterations = 8
    iteration_window = 2
    dt = 0.5e-3
  [../]
[]

[Debug]
  #show_var_residual_norms = true
[]

[Outputs]
  exodus = true
  gnuplot = true
  print_perf_log = true
[]
