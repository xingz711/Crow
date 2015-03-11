[GlobalParams]
  var_name_base = par
  name = 'global param'
  op_num = 2
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 40
  ymax = 40
  elem_type = QUAD4
[]

[Variables]
  [./PolycrystalVariables]
  [../]
[]

[AuxVariables]
  [./bnds]
    block = 0
  [../]
[]

[Kernels]
  [./PolycrystalSinteringKernel]
  [../]
[]

[AuxKernels]
  [./bnd_aux]
    type = BndsCalcAux
    variable = bnds
    block = 0
    v = PolycrystalVariables
  [../]
[]

[BCs]
  [./Periodic]
    [./periodic]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./PFDiffusnGrowth]
    type = PFDiffusionGrowth
    block = 0
    c = 0.0
    L = 10
    v = PolycrystalVariables
  [../]
[]

[Postprocessors]
  [./Num_grain]
    type = NodalFloodCount
    variable = bnds
  [../]
[]

[Executioner]
  type = Transient
  l_max_its = 30
  nl_max_its = 40
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -mat_mffd_type'
  petsc_options_value = 'hypre boomeramg 101 ds'
  nl_rel_tol = 1e-09
  end_time = 60
  scheme = bdf2
[]

[Outputs]
  output_initial = true
  exodus = true
  output_final = true
[]

[ICs]
  active = 'PolycrystalICs'
  [./PolycrystalICs]
    [./TwoParticleGrainsIC]
    [../]
  [../]
  [./2Pdens]
    variable = PolycrystalVariables
    type = TwoParticleDensityIC
    radius = '9.0 7.0'
    block = 0
  [../]
[]

