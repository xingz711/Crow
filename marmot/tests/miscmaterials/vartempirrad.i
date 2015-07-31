[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 35
  ny = 35
  xmin = 0
  xmax = 50
  ymax = 50
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./T]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./timec]
    type = TimeDerivative
    variable = c
  [../]
  [./diffc]
    type = Diffusion
    variable = c
  [../]

  [./timeT]
    type = TimeDerivative
    variable = T
  [../]
  [./diffT]
    type = Diffusion
    variable = T
  [../]
[]

[ICs]
  [./c]
    variable = c
    type = SmoothCircleIC
    x1 = 20.0
    y1 = 25.0
    radius = 12.0
    invalue = 0.6
    outvalue = 0.4
    int_width = 8.0
    block = 0
  [../]
  [./T]
    variable = T
    type = SmoothCircleIC
    x1 = 30.0
    y1 = 25.0
    radius = 12.0
    invalue = 500.0
    outvalue = 100.0
    int_width = 8.0
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'c T'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./genirrad]
    type = VarTempIrrad
    block = 0
    c = c
    temp = T

    outputs = exodus
  [../]
[]

[Problem]
  type = FEProblem
  solve = false
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  num_steps = 1
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]

