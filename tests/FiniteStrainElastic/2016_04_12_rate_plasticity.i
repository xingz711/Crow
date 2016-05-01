[Mesh]
  type = GeneratedMesh
  elem_type = HEX8
  dim = 3
  nx = 10
  ny = 10
  nz = 10
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
  zmin = 0.0
  zmax = 1.0
  displacements = 'x_disp y_disp z_disp'
[]

[MeshModifiers]
  [./cnode]
    type = AddExtraNodeset
    coord = '0.0 0.0 0.0'
    new_boundary = 6
  [../]

  [./snode]
    type = AddExtraNodeset
    coord = '1.0 0.0 0.0'
    new_boundary = 7
  [../]
[]

[Variables]
  [./x_disp]
    order = FIRST
    family = LAGRANGE
  [../]

 [./y_disp]
    order = FIRST
    family = LAGRANGE
  [../]

  [./z_disp]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'x_disp y_disp z_disp'
    use_displaced_mesh = true
  [../]
[]

[Materials]
  active='felastic'
  [./felastic]
    type = FiniteStrainRatePlasticMaterial
    block=0
    fill_method = symmetric_isotropic
    disp_x = x_disp
    disp_y = y_disp
    disp_z = z_disp
    C_ijkl = '35.46 30.141'
    yield_stress = '0. 0.25 0.04 0.2743 0.06 0.5682 0.12 0.7837 0.2 0.6858'
    ref_pe_rate = 0.01
    exponent = 3.0
  [../]
[]

[Functions]
 #[./topfunc]
 #  type = ParsedFunction
 #  value = '0.01*t'
 #[../]
 [./load]
   type = PiecewiseLinear
   y = '0.0 1.5 1.5'
   x = '0.0 5.0 20.0'
 [../]

[]

[BCs]
  [./bottom3]
    type = PresetBC
    variable = z_disp
    boundary = 0
    value = 0.0
  [../]
  [./top]
    type = FunctionPresetBC
    variable = z_disp
    boundary = 5
    function = load
  [../]
  [./corner1]
    type = PresetBC
    variable = x_disp
    boundary = 6
    value = 0.0
  [../]
  [./corner2]
    type = PresetBC
    variable = y_disp
    boundary = 6
    value = 0.0
  [../]
  [./corner3]
    type = PresetBC
    variable = z_disp
    boundary = 6
    value = 0.0
  [../]

  [./side1]
    type = PresetBC
    variable = y_disp
    boundary = 7
    value = 0.0
  [../]
  [./side2]
    type = PresetBC
    variable = z_disp
    boundary = 7
    value = 0.0
  [../]
[]

[AuxVariables]
  [./stress_zz]
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
  [./pe33]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
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
  [./pe33]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = pe33
    index_i = 2
    index_j = 2
  [../]
  [./eqv_plastic_strain]
    type = MaterialRealAux
    property = eqv_plastic_strain
    variable = peeq
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   full=true
  [../]
[]

[Executioner]
  start_time = 0.0
  end_time = 20.0
  dt = 0.01
  dtmax = 1
  dtmin = 0.0001
  type = Transient

  solve_type = 'PJFNK'

  petsc_options_iname = -pc_hypre_type
  petsc_options_value = boomerang
  #petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  #petsc_options_value = 'asm         31   preonly   lu      1'
  nl_abs_tol = 1e-10
[]


[Outputs]
  file_base = out
  exodus = true
[]
