[GlobalParams]
  var_name_base = gr
  op_num = 2.0
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 50
  xmin = 0.0
  xmax = 100
  ymin = 0.0
  ymax = 50
  elem_type = QUAD4
[]

[Variables]
  [./c]
    [./InitialCondition]
      type = TwoParticleDensityIC
      op_num = 2.0
      radius = '20.0 20.0'
      variable = c
    [../]
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
    var_name_base = gr
    op_num = 2
  [../]
[]

[Kernels]
  [./cres]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'gr0 gr1'
  [../]
  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = D
  [../]
  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]
  [./PolycrystalSinteringKernel]
    op_num = 2
  [../]
[]

[BCs]
  [./Periodic]
    active = 'periodic'
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
    [./periodic]
      auto_direction = 'x y '
    [../]
  [../]
[]

[Materials]
  [./constant]
    type = PFDiffusionGrowth
    block = 0
    rho = c
    v = 'gr0 gr1'
  [../]
  [./free_energy]
    type = SinteringFreeEnergy
    block = 0
    c = c
    v = 'gr0 gr1'
    third_derivatives = false
  [../]
  [./AC_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '1.0 0.5'
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = BDF2
  solve_type = PJFNK
  petsc_options_iname = -pc_type
  petsc_options_value = asm
  l_max_its = 30
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-10
  dt = 0.5
  num_steps = 123456789
  end_time = 50.0
[]

[Outputs]
  output_initial = true
  exodus = true
  output_on = 'initial timestep_end'
  csv = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./TwoParticleGrainsIC]
    [../]
  [../]
[]

