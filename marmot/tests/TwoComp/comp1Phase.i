#
# See
# Hybrid Potts-phase field model for coupled microstructural-compositional evolution
# E. R. Homer, V. Tikare, E. A. Holm
# Computational Material Science, 69, p. 414-423.
# Section 3.2, Composition evolution
#
# The concentration should end as 0.3.
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 1
  xmin = 0.0
  xmax = 100.0
  ymin = 0.0
  ymax = 100.0
  elem_type = QUAD4
[]

[Functions]
  [./conc]
    type = ParsedFunction
    value = 'if (x<50, 0.6, if (x=50, 0.3, 0.0))'
  [../]
  [./alpha]
    type = ParsedFunction
    value = 0
  [../]
[]

[AuxVariables]
  [./q_alpha]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = FunctionIC
      function = alpha
    [../]
  [../]
[]

[Variables]

  [./c]
    order = FIRST
    family = LAGRANGE
  [../]

  [./w]
    order = FIRST
    family = LAGRANGE
  [../]

[]

[ICs]
  [./c]
    type = FunctionIC
    function = conc
    variable = c
  [../]
[]

[Preconditioning]
  [./SMP]
   type = SMP
   off_diag_row = 'w c'
   off_diag_column = 'c w'
  [../]
[]

[Kernels]

  [./cres]
    type = SplitCHTwoComp
    variable = c
    w = w
    kappa_name = kappa_c
    a = 0.5
    lambda = 0.3
    c1 = 0.25
    c2 = 0.75
    c3 = 0.05
    c4 = 0.95
    q_alpha = q_alpha
  [../]

  [./wres]
    type = SplitCHWRes
    variable = w
    mob_name = M
    c = c
  [../]

  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]

[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./mat]
    type = PFMobility
    block = 0
    mob = 1.0
    kappa = 1.0
  [../]
[]

[Executioner]
   type = Transient
   #petsc_options = '-snes_mf'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



   petsc_options_iname = '-pc_type'
   petsc_options_value = 'lu'

  l_max_its = 30
  l_tol = 1.0e-3

  nl_max_its = 50
  nl_rel_tol = 1.0e-10

  dt = 200
  start_time = 0
  end_time = 1600

[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
