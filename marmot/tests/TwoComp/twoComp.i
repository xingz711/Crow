#
# See
# Hybrid Potts-phase field model for coupled microstructural-compositional evolution
# E. R. Homer, V. Tikare, E. A. Holm
# Computational Material Science, 69, p. 414-423.
# Section 3.1, Variables and initial implementation
#
# The concentration should end as 0.295454545... at the left with a narrow
#   transition to 0.70454545... on the right.
#
# These values can be found by determining the minimum of the energy function
#   with q_alpha=1, q_beta=0 and then q_alpha=0, q_beta=1.
#

[GlobalParams]
  order = FIRST
  family = LAGRANGE
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  ny = 1
  xmin = 0.0
  xmax = 100.0
  ymin = 0.0
  ymax = 100.0
  elem_type = QUAD9
[]

[Functions]
  [./conc]
    type = ParsedFunction
    value = 'if (x<50, 0.3, 0.7)'
  [../]
  [./alpha]
    type = ParsedFunction
    value = 'if (x>=50, 1, 0)'
  [../]
[]

[AuxVariables]
  [./q_alpha]
    [./InitialCondition]
      type = FunctionIC
      function = alpha
    [../]
  [../]
[]

[Variables]

  [./c]
  [../]

  [./w]
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
  [../]

  [./time]
    type = CoupledImplicitEuler
    variable = w
    v = c
  [../]

[]

[BCs]
[]

[Materials]
  [./mat]
    type = PFMobility
    block = 0
    mob = 100.0
    kappa = 1.0 #0.1
  [../]
[]

[Executioner]
   type = Transient
   scheme = 'BDF2'
   #petsc_options = '-snes_mf'

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



   petsc_options_iname = '-pc_type'
   petsc_options_value = 'lu'

  l_max_its = 30
  l_tol = 1.0e-3

  nl_max_its = 50
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1e-9

  dt = 1.0
  start_time = 0
  end_time = 20

[]

[Outputs]
  output_initial = true
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
