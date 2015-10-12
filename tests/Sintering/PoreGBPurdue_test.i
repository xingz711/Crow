[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 9
  ny = 9
  xmax = 250
  ymax = 250
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 1
  var_name_base = gr
  int_width = 20
  dom_dim = 2D
  amplitude = 50
  sub_height = 125
  ncycl = 1
[]

[MeshModifiers]
  [./SubdomainBoundingBox]
    type = SubdomainBoundingBox
    block_id = '1'
    bottom_left = '0 125 0'   #  substrate_height
    top_right = '125 185 0'   #  substrate_height + Amplitude + int_width/2
  [../]
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
  [./gr0]
    scaling = 1e3
  [../]
[]

[ICs]
  [./c_IC]
    type = SinusoidalSurfaceIC
    var_type = conserved
    variable = c
  [../]
  [./gr0_IC]
    type = SinusoidalSurfaceIC
    var_type = nonconserved
    variable = gr0
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./free_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHPoreGBPurdue
    variable = c
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWResMtrxMob
    variable = w
    c = c
    mob_name = M
    dmob_name = dMdc
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./PoreGBPurdueKernel]
    c = c
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
  [./free_energy]
    type = PoreGBPurdueFreeEnergyCalc
    variable = free_energy
    c = c
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./x]
      auto_direction = 'x'
    [../]
  [../]
[]

[Materials]
  [./MoHe]
    type = PoreGBPurdueMaterial
    block = '0 1'
    T = 500.0
    int_width = 20
    D0 = 4e-4
    Em = 1.0
    GB_energy = 6.86
    surface_energy = 9.32
    GBmob0 = 3.986e-6
    Q = 1.0307
    c = c
    omega = 31.855013e-30
    bulkindex = 1.0
    surfindex = 1.0
    gbindex = 1.0
    output = exodus
  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
    execute_on = 'initial timestep_end'
  [../]
  [./Amp]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
    block = 1
    execute_on = 'initial timestep_end'
  [../]
  [./total_C]
    type = ElementIntegralVariablePostprocessor
    variable = c
    block = '0 1'
    execute_on = 'initial timestep_end'
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
  scheme = bdf2

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'

  nl_max_its = 30
  l_max_its = 20
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10

  start_time = 0.0
  num_steps = 3
  dt = 40.0
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]
