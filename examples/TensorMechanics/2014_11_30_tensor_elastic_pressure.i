[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  uniform_refine = 4
[]

[GlobalParams]
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Functions]
  [./loadramp]
    type = PiecewiseLinear
    y = '0 30 60'
    scale_factor = '1 '
    x = '0 5 10'
  [../]
[]

[BCs]
  [./Pressure]
    [./top]
      function = loadramp
      boundary = top
      disp_y = disp_y
      disp_x = disp_x
      component = y
      variable = disp_y
    [../]
  [../]
  [./fix-x]
    type = DirichletBC
    variable = disp_x
    boundary = bottom
    value = 0.0
  [../]
  [./fix-y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]
[]

[Materials]
  [./mat]
    type = LinearIsotropicMaterial
    block = 0
    disp_y = disp_y
    disp_x = disp_x
    poissons_ratio = 0.3
    youngs_modulus = 1e6
  [../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  end_time = 10
[]

[Outputs]
  [./console]
    type = Console
    output_file = true
  [../]
  [./exodos]
    type = Exodus
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_y = disp_y
    block = 0
    disp_x = disp_x
  [../]
[]

