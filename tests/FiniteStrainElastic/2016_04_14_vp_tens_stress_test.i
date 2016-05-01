[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 40
  nz = 0
  xmin = 0.0
  xmax = 40.0
  ymin = 0.0
  ymax = 20.0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
  block = 0
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./S11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./S22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./peeq]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./pe11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./pe22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./elastic_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./TensorMechanics]
  [../]
[]

[AuxKernels]
  [./S11]
    type = RankTwoAux
    variable = S11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    block = 0
  [../]
  [./S22]
    type = RankTwoAux
    variable = S22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    block = 0
  [../]
  [./pe11]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = pe11
    index_i = 0
    index_j = 0
  [../]
    [./pe22]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = pe22
    index_i = 1
    index_j = 1
  [../]
  [./eqv_plastic_strain]
    type = MaterialRealAux
    property = eqv_plastic_strain
    variable = peeq
  [../]
  [./elastic_en]
    type = TensorElasticEnergyAux
    variable = elastic_en
  [../]
[]

[Functions]
  [./load]
    type = PiecewiseLinear
    y = '0.0 -15.0 -15.0'
    x = '0.0 10.0 20.0'
  [../]
[]

[BCs]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./right_x]
    type = PresetBC
    variable = disp_x
    boundary = right
    value = 0
  [../]
  [./Pressure]
    [./top]
      boundary = top
      function = load
      disp_x = disp_x
      disp_y = disp_y
    [../]
  [../]
[]

[Materials]
  [./finitestrain]
    type = FiniteStrainRatePlasticMaterial
    block = 0
    fill_method = symmetric_isotropic
    disp_x = disp_x
    disp_y = disp_y
    C_ijkl = '35.46 30.141'
    yield_stress = '0. 0.25 0.04 0.2743 0.06 0.5682 0.12 0.7837 0.2 0.6858'
    #yield_stress = '0. 5.0 0.05 6.1 0.1 6.8 0.38 8.1 0.95 9.2 2. 9.5'
    ref_pe_rate = 0.01
    exponent = 3.0
  [../]
[]

[Postprocessors]
  [./pe22]
    type = ElementAverageValue
    variable = pe22
  [../]
  [./pe11]
    type = ElementAverageValue
    variable = pe11
  [../]
  [./peeq]
    type = ElementAverageValue
    variable = peeq
  [../]
  [./s22]
    type = ElementAverageValue
    variable = S22
  [../]
  [./s11]
    type = ElementAverageValue
    variable = S11
  [../]
  [./S11]
    type = ElementIntegralVariablePostprocessor
    variable = S11
  [../]
  [./S22]
    type = ElementIntegralVariablePostprocessor
    variable = S22
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

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly       lu           1'

  nl_rel_tol = 1e-8
  l_max_its = 100
  nl_max_its = 100

  dt = 0.01
  dtmin = 1e-4
  #num_steps = 20
  end_time = 18
[]

[Outputs]
  exodus = true
  csv = true
  gnuplot = true
[]
