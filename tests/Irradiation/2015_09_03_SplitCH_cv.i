#
# Example problem showing how to use the DerivativeParsedMaterial with SplitCHParsed.
# The free energy is identical to that from SplitCHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 150
  ny = 150
  xmax = 60
  ymax = 60
  elem_type = QUAD4
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  #[./cIC]
  #  type = RandomIC
  #  variable = c
  #  max = 0.999
  #  min = 0.001
  #[../]
  [./cIC]
    # int_width = 0.2
    int_width = 1.0
    x1 = 30.0
    y1 = 30.0
    radius = 5.0
    outvalue = 0.122
    variable = c
    invalue = 0.999
    type = SmoothCircleIC
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = fbulk
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
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
    type = GenericConstantMaterial
    prop_names  = kappa_c
    prop_values = 0.5
    block = 0
  [../]
  [./mobility]
    type = DerivativeParsedMaterial
    block = 0
    f_name = M
    args = c
    constant_names = 'D kbT'
    constant_expressions = '1.0 0.11'
    function = D*c/kbT
    #tol_names = c
    #tol_values = 0.001
    enable_jit = true
    outputs = exodus
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    block = 0
    f_name = fbulk
    args = c
    constant_names = 'Ef kbT'
    constant_expressions = '1.0 0.11'
    function = (Ef*c+kbT*(c*log(c)+(1.0-c)*log(1.0-c)))
    #function = (Ef*c+kbT*(c*plog(c,0.001)+(1.0-c)*plog((1.0-c),0.001)))
    tol_names = c
    tol_values = 0.001
    enable_jit = true
    outputs = exodus
  [../]
[]

[Postprocessors]
  [./top]
    type = SideIntegralVariablePostprocessor
    variable = c
    boundary = top
  [../]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  # petsc_options_value = 'hypre boomeramg 31'
  type = Transient
  scheme = bdf2
  dt = 0.1
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1e-4
  nl_max_its = 20
  nl_rel_tol = 1e-9
  end_time = 20.0
[]

[Outputs]
  exodus = true
  gnuplot = true
  csv = true
  output_on = 'TIMESTEP_END initial linear nonlinear'
[]
