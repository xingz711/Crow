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
  [./phi]
    order = FIRST
    family = LAGRANGE
  [../]
  [./T]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = 'c T'
  [../]
[]

[ICs]
  [./c]
    variable = c
    type = SmoothCircleIC
    x1 = 20.0
    y1 = 20.0
    radius = 12.0
    invalue = 0.45
    outvalue = 0.01
    int_width = 8.0
    block = 0
  [../]

  [./T]
    variable = T
    type = SmoothCircleIC
    x1 = 25.0
    y1 = 30.0
    radius = 14.0
    invalue = 300
    outvalue = 400
    int_width = 12.0
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'c T '
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./ab1cd1]
    type = CalphadAB1CD1Material
    f_name = Falpha
    block = 0
    c = c
    T = T

    ep_lo = '-7827.595    125.64905    -24.1618    -0.00437791     34971.0'  # HCP_Zr
    ep_hi = ' 8055.336   -243.791       18.3135    -0.034513     -734182.8'  # H2_gas'
    mix = ' -45965    41.6    0'     # FCC_ZrH

    outputs = exodus
  [../]

  [./ab1cd2]
    type = CalphadAB1CD2Material
    f_name = Fdelta
    block = 0
    c = c
    T = T

    ep_lo    = ' -227.595    124.74905   -24.1618   -0.00437791     34971'    # FCC_Zr
    ep_hi    = ' 8055.336   -243.791      18.3135   -0.034513     -734182.8'  # H2_gas
    pure_ep1 = '-7827.595    125.64905   -24.1618   -0.00437791     34971.0'  # HCP_Zr'

    mix = '-170490   208.2   -9.47'  # FCC_ZrH2
    L0 = '  14385  -6.0'
    L1 = '-106445  87.3'

    outputs = exodus
  [../]
[]

[Problem]
  type = FEProblem
  solve = false
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
