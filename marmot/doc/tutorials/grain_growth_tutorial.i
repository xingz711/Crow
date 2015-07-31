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
  op_num = 2
  var_name_base = gr
  v = 'gr0 gr1'
[]

[Variables]

  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
    x1 = 0.0
    y1 = 0.0
    x2 = 1000.0
    y2 = 1000.0
    periodic = true
    ICType = 1
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
  [../]
[]

[BCs]
  active = 'Periodic'

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
f
[Materials]
  active = 'CuGrGr'

  [./CuGrGr]
    type = CuGrGr
    block = 0
    temp = 500 #K
    wGB = 20 #nm
  [../]
[]

[Postprocessors]
  [./ave_gr_area]
    type = PolycrystalAvGrArea
    variable = gr0
  [../]
[]

[Executioner]
  active = ' '
  type = Transient
  scheme = 'crank-nicolson'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'


  petsc_options = '-snes_ksp_ew'

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'

  l_max_its = 30

  nl_max_its = 20

  start_time = 0.0
  num_steps = 2
  dt = 20.0

  [./Adaptivity]
   initial_adaptivity = 2
    refine_fraction = 0.3
    coarsen_fraction = 0.2
    max_h_level = 2
  [../]
[]

[Outputs]
  output_linear = true
  file_base = output
  output_initial = true
  interval = 1
  exodus = true
  perf_log = true
[]



