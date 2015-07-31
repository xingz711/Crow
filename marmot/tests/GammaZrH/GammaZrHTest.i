[GlobalParams]
  var_name_base = gr
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 8
  ny = 8
  xmax = 20
  ymax = 20
  elem_type = QUAD4
[]

[Variables]
  [./C]
    block = 0
  [../]
  [./eta1]
    block = 0
  [../]
  [./eta2]
    block = 0
  [../]
  [./eta3]
    block = 0
  [../]
  [./mu]
    block = 0
  [../]
  [./disp_x]
    block = 0
  [../]
  [./disp_y]
    block = 0
  [../]
[]

[AuxVariables]
  active = 'MaxShear sigma11 sigma12 sigma1 sigma22 sigma2 f'
  [./eta2]
    block = 0
  [../]
  [./eta3]
    block = 0
  [../]
  [./sigma11]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./sigma22]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./sigma12]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./sigma1]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./sigma2]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./MaxShear]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./f]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
[]

[Kernels]
  [./eta1]
    type = ACBulkZrH
    variable = eta1
    C = C
    eta2 = eta2
    eta3 = eta3
    block = 0
  [../]
  [./eta1int]
    type = ACInterface
    variable = eta1
    block = 0
  [../]
  [./eta1time]
    type = TimeDerivative
    variable = eta1
    block = 0
  [../]
  [./eta2]
    type = ACBulkZrH
    variable = eta2
    C = C
    eta2 = eta1
    eta3 = eta3
    block = 0
  [../]
  [./eta2int]
    type = ACInterface
    variable = eta2
    block = 0
  [../]
  [./eta2time]
    type = TimeDerivative
    variable = eta2
    block = 0
  [../]
  [./eta3]
    type = ACBulkZrH
    variable = eta3
    C = C
    eta2 = eta2
    eta3 = eta1
    block = 0
  [../]
  [./eta3int]
    type = ACInterface
    variable = eta3
    block = 0
  [../]
  [./eta3time]
    type = TimeDerivative
    variable = eta3
    block = 0
  [../]
  [./C]
    type = SplitCHZrH
    variable = C
    eta2 = eta2
    eta3 = eta3
    eta1 = eta1
    block = 0
    kappa_name = beta
    w = mu
  [../]
  [./mu]
    type = SplitCHWRes
    variable = mu
    block = 0
    mob_name = M
  [../]
  [./CoupledImplicitEuler]
    type = CoupledImplicitEuler
    variable = mu
    block = 0
    v = C
  [../]
  [./Elastic1]
    type = GammaZrHElasticEnergy
    variable = eta1
    C = C
    eta2 = eta2
    eta3 = eta3
    eigen2 = '0.043675 0.018232 0.057 0 0 0.022035'
    eigen3 = '0.043675 0.018232 0.057 0 0 -0.022035'
    eigen1 = '0.00551 0.0564 0.057 0 0 0'
    block = 0
  [../]
  [./Elastic2]
    type = GammaZrHElasticEnergy
    variable = eta2
    C = C
    eta2 = eta1
    eta3 = eta3
    eigen2 = '0.00551 0.0564 0.057 0 0 0'
    eigen3 = '0.043675 0.018232 0.057 0 0 -0.022035'
    eigen1 = '0.043675 0.018232 0.057 0 0 0.022035'
    block = 0
  [../]
  [./Elastic3]
    type = GammaZrHElasticEnergy
    variable = eta3
    C = C
    eta2 = eta2
    eta3 = eta1
    eigen2 = '0.043675 0.018232 0.057 0 0 0.022035'
    eigen3 = '0.00551 0.0564 0.057 0 0 0'
    eigen1 = '0.043675 0.018232 0.057 0 0 -0.022035'
    block = 0
  [../]
  [./TensorMechanics]
    disp_y = disp_y
    disp_x = disp_x
  [../]
[]

[AuxKernels]
  [./sigma11]
    type = RankTwoAux
    variable = sigma11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./sigma22]
    type = RankTwoAux
    variable = sigma22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    block = 0
  [../]
  [./sigma12]
    type = RankTwoAux
    variable = sigma12
    rank_two_tensor = stress
    index_j = 1
    index_i = 0
    block = 0
  [../]
  [./sigma1]
    type = RankTwoEigenvals
    variable = sigma1
    rank_two_tensor = stress
    index_i = 0
    block = 0
  [../]
  [./sigma2]
    type = RankTwoEigenvals
    variable = sigma2
    rank_two_tensor = stress
    index_i = 1
    block = 0
  [../]
  [./MaxShear]
    type = MaxShear
    variable = MaxShear
    rank_two_tensor = stress
    block = 0
  [../]
  [./f]
    type = ZrHFreeEnergy
    variable = f
    C = C
    eta2 = eta2
    eta3 = eta3
    block = 0
    eta1 = eta1
  [../]
[]

[BCs]
  active = 'xbc ybc'

  [./Periodic]
    [./all]
      auto_direction = 'x y'
      variable = 'C mu eta1'
    [../]
  [../]
  [./xbc]
    type = PresetBC
    variable = disp_x
    boundary = '0 1 2 3'
    value = 0
  [../]
  [./ybc]
    type = PresetBC
    variable = disp_y
    boundary = '0 1 2 3'
    value = 0
  [../]
[]

[Materials]
  [./GammaZrH]
    type = GammaZrHMaterial
    block = 0
    T = 400
  [../]
  [./ZrHTransStrain]
    type = ZrHTransStrainMaterial
    block = 0
    disp_y = disp_y
    disp_x = disp_x
    eta3 = eta3
    eta2 = eta2
    eta1 = eta1
    c = C
    C_ijkl = '259 113.4 107.7 259.0 107.7 287.5 60.5 60.5 73.5'
    temperature = 400
    all_21 = false
  [../]
[]

[Preconditioning]
  # active = 'FDP'
  active = 'SMP'
  [./FDP]
    type = FDP
    full = true
  [../]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  num_steps = 1
  dt = 0.2

  solve_type = PJFNK
  scheme = bdf2

  l_max_its = 30
  l_tol = 1.0e-6

  nl_rel_tol = 1.0e-8
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
[]

[ICs]
  active = 'C1 eta1'
  [./eta1]
    y2 = 15
    y1 = 5
    inside = 1
    x2 = 15
    variable = eta1
    x1 = 5
    type = BoundingBoxIC
    block = 0
  [../]
  [./C1]
    y2 = 15
    y1 = 5
    inside = 0.5
    x2 = 15
    outside = 0.05
    variable = C
    x1 = 5
    type = BoundingBoxIC
    block = 0
  [../]
  [./eta2]
    y2 = 65
    y1 = 55
    inside = 1
    x2 = 52.5
    variable = eta2
    x1 = 42.5
    type = BoundingBoxIC
    block = 0
  [../]
  [./eta3]
    y2 = 65
    y1 = 55
    inside = 1
    x2 = 152.5
    variable = eta3
    x1 = 142.5
    type = BoundingBoxIC
    block = 0
  [../]
  [./Crand]
    variable = C
    max = 0.1
    type = RandomIC
    block = 0
  [../]
  [./eta1rand]
    variable = eta1
    max = 0.6
    type = RandomIC
    block = 0
  [../]
  [./eta2rand]
    variable = eta2
    max = 0.6
    type = RandomIC
    block = 0
  [../]
  [./eta3rand]
    variable = eta3
    max = 0.6
    type = RandomIC
    block = 0
  [../]
[]
