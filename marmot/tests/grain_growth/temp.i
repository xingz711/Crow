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

[Functions]
  [./TGradient]
    type = ParsedFunction
    value = '450 + 0.1*y'
  [../]
[]

[Variables]

  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalCircleGrainIC]
      radius = 333.333
      x = 500
      y = 500
      int_width = 80
    [../]
  [../]
[]

[AuxVariables]

  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]

  [./T]
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

  [./Tgrad]
    type = FunctionAux
    variable = T
    function = TGradient
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

[Materials]

  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = T #K
    wGB = 60 #nm
  [../]
[]

[Postprocessors]
  [./ave_gr_area]
    type = PolycrystalAvGrArea
    variable = gr0
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

  solve_type = 'NEWTON'


  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'

  l_max_its = 30
  l_tol = 1.0e-4

  nl_max_its = 20
  nl_rel_tol = 1.0e-9

  start_time = 0.0
  num_steps = 6
  dt = 80.0

  [./Adaptivity]
   initial_adaptivity = 2
    refine_fraction = 0.8
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]

