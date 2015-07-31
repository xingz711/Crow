[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmax = 20
  ymax = 20
  elem_type = QUAD4
[]

[GlobalParams]
  polynomial_order = 8
  time_scale = 1.0e-3
  length_scale = 1.0e-9
[]

[Variables]
  [./c]
    initial_condition = 0.0001
  [../]
  [./w]
    scaling = 1e5
  [../]
[]

[AuxVariables]
  [./T]
  [../]
  [./fission_rate]
    initial_condition = 2e19
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    c = c
    mob_name = M
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./Xe_source]
    type = RadSourceVExp
    variable = w
    c = c
    exp = 10
    fission_rate = fission_rate
  [../]
[]

[AuxKernels]
  [./Temp]
    type = ConstantAux
    execute_on = 'initial timestep_end'
    variable = T
    value = 1200
  [../]
  [./fission_rate]
    type = ConstantAux
    execute_on = 'initial timestep_end'
    variable = fission_rate
    value = 2e19
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./XeinUO2]
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T
    int_width = 0.65 # nm
    D0 = 5.0e-5 # m^2/s
    Em = 3.9 # eV
    Ef = 4.35 # eV
    surface_energy = 0.6 # J/m^2
  [../]
  [./FreeEnergy]
    type = PolynomialFreeEnergy
    block = 0
    derivative_order = 2
    T = T
    c = c
  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
    execute_on = 'initial timestep_end'
  [../]
  [./T]
    type = ElementAverageValue
    variable = T
    execute_on = 'initial timestep_end'
  [../]
  [./Por]
    type = Porosity
    variable = c
    execute_on = 'initial timestep_end'
  [../]
  [./n_nodes]
    type = NumNodes
    execute_on = 'initial timestep_end'
  [../]
  [./n_bubbles]
    type = FeatureFloodCount
    variable = c
    execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 30
  nl_rel_tol = 1.0e-9
  nl_abs_tol = 1.0e-11

  start_time = 0.0
  num_steps = 2
  dt = 4.0e5
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
