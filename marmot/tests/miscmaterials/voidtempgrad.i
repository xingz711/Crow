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
  [./T]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
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
      variable = 'T'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./genirrad]
    type = VoidTempGrad
    block = 0
    temp = T
    Dvan_zero = 2e10
    Act_energy = 0.5
    Heat_trans = 0.33

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

