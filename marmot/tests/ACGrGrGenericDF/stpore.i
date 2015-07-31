[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  nz = 0
  xmax = 1000
  ymax = 1000
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 1
[]

[GlobalParams]
  op_num = 2
  var_name_base = gr
  wGB = 60 # interfacial width in nm
  en_ratio = 0.8
[]

[Variables]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 630
      y2 = 1000
    [../]
  [../]
[]

[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      int_width = 60
      x1 = 500
      y1 = 500
      radius = 90
      outvalue = 0.001
      invalue = 0.99
      type = SmoothCircleIC
      variable = c
    [../]
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
    c = c
  [../]
  [./PolyGenericDFKernel]
    DF = '1.111111111111111e8 0 0'
    c = c
  [../]
[]

[AuxKernels]
  [./BndsCalc]
    execute_on = timestep_end
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[Materials]
  [./CuGrGr]
    type = CuGrGr
    block = 0
    T = 450 # K
  [../]
[]

[Postprocessors]
  [./area_grain0]
    type = ElementIntegralVariablePostprocessor
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
  scheme = bdf2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  l_max_its = 30
  l_tol = 1.0e-4
  nl_max_its = 20
  nl_rel_tol = 1.0e-9
  start_time = 0.0
  num_steps = 8
  dt = 30
[]

[Outputs]
  exodus = true
  output_initial = false
  csv = true
  print_linear_residuals = true
  print_perf_log = true
[]

