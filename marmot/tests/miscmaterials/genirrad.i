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
  [./cv]
    order = FIRST
    family = LAGRANGE
  [../]
  [./ci]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./timev]
    type = TimeDerivative
    variable = cv
  [../]
  [./diffv]
    type = Diffusion
    variable = cv
  [../]

  [./timei]
    type = TimeDerivative
    variable = ci
  [../]
  [./diffi]
    type = Diffusion
    variable = ci
  [../]
[]

[ICs]
  [./cv]
    variable = cv
    type = SmoothCircleIC
    x1 = 20.0
    y1 = 25.0
    radius = 12.0
    invalue = 0.6
    outvalue = 0.4
    int_width = 8.0
    block = 0
  [../]
  [./ci]
    variable = ci
    type = SmoothCircleIC
    x1 = 30.0
    y1 = 25.0
    radius = 12.0
    invalue = 0.6
    outvalue = 0.4
    int_width = 8.0
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'cv ci'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./genirrad]
    type = GenIrrad
    block = 0
    cv = cv
    ci = ci
    # free_energy_form parameter is set in the 'tests' file

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

