[Mesh]
  type = EBSDMesh
  filename = 'Marmot_2D_Input.txt'
[]

[UserObjects]
  [./ebsd]
    type = EBSDReader
    op_num = 9
  [../]
[]

[ICs]
  [./PolycrystalICs]
    [./ReconAuxVarIC]
      ebsd_reader = ebsd
      sd = 1
    [../]
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[AuxKernels]
  [./phi1_aux]
    type = TestEBSDAux
    variable = PHI1
    ebsd_reader = ebsd
    data_name = 'phi1'
  [../]
  [./phi_aux]
    type = TestEBSDAux
    variable = PHI
    ebsd_reader = ebsd
    data_name = 'phi'
  [../]
  [./phi2_aux]
    type = TestEBSDAux
    variable = PHI2
    ebsd_reader = ebsd
    data_name = 'phi2'
  [../]
  [./grain_aux]
    type = TestEBSDAux
    variable = GRAIN
    ebsd_reader = ebsd
    data_name = 'grain'
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Executioner]
  type = Steady
  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
