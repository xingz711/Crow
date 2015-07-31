[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 3
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
  Euler_angles_file_name = test.tex
[]

[Variables]
active = 'PolycrystalVariables disp_x disp_y'

  [./PolycrystalVariables]
    scaling = 1.0e4
  [../]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 500
      y2 = 1000
    [../]
  [../]
[]

[AuxVariables]
#active = 'bnds elastic_energy'

  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]

  [./elastic_strain11]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elastic_strain22]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./elastic_strain12]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[Kernels]

  [./PolycrystalKernel]
  [../]

  [./PolyElasticDFKernel]
    disp_x_name = disp_x
    disp_y_name = disp_y
  [../]

[]

[AuxKernels]
#active = 'BndsAuxKernel elastic_energy'

  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
    execute_on = timestep_end
  [../]

  [./elastic_strain11]
    type = MaterialTensorAux
    variable = elastic_strain11
    tensor = elastic_strain
    index = 0
    execute_on = timestep_end
  [../]

  [./elastic_strain22]
    type = MaterialTensorAux
    variable = elastic_strain22
    tensor = elastic_strain
    index = 1
    execute_on = timestep_end
  [../]

  [./elastic_strain12]
    type = MaterialTensorAux
    variable = elastic_strain12
    tensor = elastic_strain
    index = 3
    execute_on = timestep_end
  [../]

[]

[BCs]

  [./top_displacement]
    type = PresetBC
    variable = disp_y
    boundary = top
    value = -10.0
  [../]

  [./x_anchor]
    type = PresetBC
    variable = disp_x
    boundary = 'left right'
    value = 0.0
  [../]

  [./y_anchor]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]

[]

[Materials]
  active = 'CuGrGr LinPoly'

  [./CuGrGr]
    type = CuGrGr
    block = 0
    temp = 500 #K
    wGB = 75 #nm
    time_scale = 1.0e-6
  [../]

  [./LinPoly]
    type = LinearPolycrystal
    block = 0
    c11_0 = 1.815e5
    c12_0 = 1.018e5
    c44_0 = 1.018e5
    dc11 = -10.9e1
    dc12 = -6.2e1
    dc44 = -5.65e1
    temp_C = 500 #K
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[Postprocessors]
#active = 'dt ave_gr_area'

  [./dt]
    type = TimestepSize
  [../]

  [./gr0_area]
    type = ElementIntegralVariablePostprocessor
    variable = gr0
  [../]

  [./GB_xpos]
    type = GBPosition
    variable = bnds
    boundary = 0
    direction_index = 0
  [../]
[]

[Preconditioning]
#active = ''

  [./SMP]
   type = SMP
   off_diag_row = 'gr0 gr1 disp_x disp_y'
   off_diag_column = 'gr1 gr0 disp_y disp_x'
  [../]
[]

[Executioner]
  type = Transient

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  #petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  #petsc_options_value = 'hypre boomeramg 31'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  l_max_its = 30
  l_tol = 1e-4

  nl_max_its = 30
  nl_rel_tol = 1e-10
  nl_abs_tol = 1.e-11

  start_time = 0.0
  num_steps = 10
  dt = 0.15

  [./Adaptivity]
   initial_adaptivity = 2
    refine_fraction = 0.8
    coarsen_fraction = 0.05
    max_h_level = 2
  [../]
[]

[Outputs]
  file_base = stress
  interval = 1
  csv = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]



