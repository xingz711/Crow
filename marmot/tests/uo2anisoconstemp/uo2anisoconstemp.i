[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 35
  ny = 35
  xmin = 0
  xmax = 50
  ymax = 50
[]

[GlobalParams]
  op_num = 4
  var_name_base = gr
  v = 'gr0 gr1 gr2 gr3'
[]

[Variables]
  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
  [../]

  [./cg]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./PolycrystalKernel]
  [../]

  [./ie_cg]
    type = TimeDerivative
    variable = cg
  [../]

  [./Diff_cg]
    type = MatDiffusion
    variable = cg
    D_name = Dg
  [../]

  [./CHsink]
    type = CHRadSinkAniso
    variable = cg
    mob_name = Mg
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./PolycrystalVoronoiIC]
      grain_num = 4
    [../]
  [../]
  [./cg]
    variable = cg
    type = SmoothCircleIC
    x1 = 25.0
    y1 = 25.0
    radius = 15.0
    invalue = 0.6
    outvalue = 0.4
    int_width = 3.0
    block = 0
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'cg'
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./LANL]
    type = UO2AnisoConsTemp
    block = 0
    temp = 1100 #K
    c = cg
    kappa_cg = 100
    M0GB = 2.5e-6 #m^4/(J*s);
    QGB = 2.3 #eV
    sigma_GB = 0.708 #J/m^2
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

