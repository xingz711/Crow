[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  ny = 30
  xmin = 0.0
  xmax = 30.0
  ymin = 0.0
  ymax = 30.0
  elem_type = QUAD4
  uniform_refine = 0
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./c_IC]
    type = PlanarSmoothCircleIC
    variable = c
    direction = x
    position = 21
    int_width = 3
    bubspac = 8
    numbub = 3
    radius = 3
    radius_variation = 0.75
    radius_variation_type = uniform
    outvalue = -1
    invalue = 1
    rand_seed = 123445
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHMath
    variable = c
    kappa_name = kappa_c
    w = w
  [../]
  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = M
    c = c
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
[]

[BCs]
  [./Periodic]
    [./top_bottom]
      primary = 0
      secondary = 2
      translation = '0 30.0 0'
    [../]
    [./left_right]
      primary = 1
      secondary = 3
      translation = '-30.0 0 0'
    [../]
  [../]
[]

[Materials]
  [./constant]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 2.0
  [../]
[]

[Executioner]
  # petsc_options = '-snes_mf'
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   lu      1'
  l_max_its = 30
  l_tol = 1.0e-3
  nl_max_its = 50
  nl_rel_tol = 1.0e-10
  dt = 10.0
  num_steps = 1
[]

[Outputs]
  file_base = uniform
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]


