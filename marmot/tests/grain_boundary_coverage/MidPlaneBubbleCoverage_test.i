[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  nz = 0
  xmin = 0
  xmax = 1000
  ymin = 0
  ymax = 1000
  zmin = 0
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  op_num = 4
  var_name_base = gr
[]

[Variables]
  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./PolycrystalHexGrainIC]
      grain_num = 4
      x_offset = 0.25
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./left_right]
      primary = 0
      secondary = 2
      translation = '0 1000 0'
    [../]
    [./top_bottom]
      primary = 1
      secondary = 3
      translation = '-1000 0 0'
    [../]
  [../]
[]

[Materials]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 500 # K
    wGB = 40 # nm
  [../]
[]

[Postprocessors]
  [./ave_gr_area]
    type = PolycrystalAvGrArea
    variable = gr0
    execute_on = 'initial timestep_end'
  [../]
  [./gb_area]
    type = MidPlaneBubbleCoverage
    GB_lengthy = 20
    Lx = 1000
    variable = bnds
    execute_on = 'initial timestep_end'
  [../]
[]

[Executioner]
  type = Transient
  scheme = crank-nicolson

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 30
  nl_max_its = 20
  start_time = 0.0
  num_steps = 2
  dt = 50.0
[]

[Outputs]
  file_base = MidPlaneBubbleCoverage_out
  output_initial = true
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
