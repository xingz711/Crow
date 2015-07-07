[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  xmin = -25
  xmax = 25
  ymin = -25
  ymax = 25
  elem_type = QUAD4
[]

[Variables]
  [./c]
    order = THIRD
    family = HERMITE
  [../]
[]

[AuxVariables]
  [./eta]
    block = 0
  [../]
[]

[ICs]
  [./c]
    type = SmoothCircleIC
    variable = c
    x1 = 10.0
    y1 = 5.0
    radius = 6.0
    invalue = 1.0
    outvalue = 0.0
    int_width = 4.0
    3D_spheres = false
  [../]
  [./eta_ic]
    int_width = 4.0
    x1 = 10.0
    y1 = 5.0
    3D_spheres = false
    outvalue = 0.0
    variable = eta
    radius = 6.0
    invalue = 1.0
    type = SmoothCircleIC
    block = 0
  [../]
[]

[Kernels]
  active = 'ie_c CHSolid CHInterface'
  [./ie_c]
    type = TimeDerivative
    variable = c
  [../]
  [./CHSolid]
    type = CHMath
    variable = c
    mob_name = M
  [../]
  [./CHInterface]
    type = CHInterface
    variable = c
    kappa_name = kappa_c
    mob_name = M
  [../]
  [./Grain_motion]
    type = GrainRigidBodyMotion
    variable = c
    total_grains = 1.0
    advection_velocity = advection_velocity
    block = 0
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
  active = 'dF constant'
  [./constant]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 1.0
  [../]
  [./dF]
    type = ForceDensityMaterial
    block = 0
    c = c
    etas = eta
  [../]
  [./vadv]
    type = GrainAdvectionVelocity
    block = 0
    grain_force = grain_force
    etas = eta
    grain_data = grain_center
  [../]
[]

[VectorPostprocessors]
  [./grain]
    type = GrainCenterPostprocessors
    total_grains = 1
    grain_data = grain_center
  [../]
  [./force]
    type = GrainForcesPostprocessors
    total_grains = 1
    grain_force = grain_force
  [../]
[]

[UserObjects]
  [./grain_center]
    type = ComputeGrainCenterUserObject
    block = 0
    etas = eta
    execute_on = 'TIMESTEP_END residual'
  [../]
  [./grain_force]
    type = ComputeGrainForceAndTorque
    force_density = force_density
    grain_center = grain_center
    grain_data = grain_center
    block = 0
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  l_max_its = 20
  l_tol = 1.0e-4
  nl_max_its = 40
  nl_rel_tol = 1e-9
  start_time = 0.0
  num_steps = 5.0
  dt = 1.0
[]

[Outputs]
  output_initial = true
  exodus = false
  print_linear_residuals = true
  print_perf_log = true
  [./out]
    type = Exodus
    refinements = 2
  [../]
[]

