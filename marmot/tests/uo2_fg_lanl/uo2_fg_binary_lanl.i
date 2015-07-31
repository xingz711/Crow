[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 80
  ny = 1
  nz = 0
  xmin = 0
  xmax = 80
  ymin = 0
  ymax = 5
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]


[GlobalParams]
  op_num = 2
  var_name_base = gr
 v = 'gr0 gr1'
[]

[Variables]
  [./PolycrystalVariables]
 [../]

  [./cgas]
    order = THIRD
    family = HERMITE
    scaling=1.0e-0
    [./InitialCondition]
      type = ConstantIC
      value = 0.001
    [../]
  [../]

[]

[ICs]
  [./PolycrystalICs]
    [./BicrystalBoundingBoxIC]
      x1 = 0
      y1 = 0
      x2 = 40
      y2 = 5
    [../]
  [../]
[]


[AuxVariables]
  [./bnds]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
active ='PolycrystalKernel ie_cg Diff_cg interf_cg interf_cu cg_sink'

  [./PolycrystalKernel]
  [../]

      [./ie_cg]
      type = TimeDerivative
      variable = cgas
      [../]

      [./Diff_cg]
      type = XeEDiffusion
      variable = cgas
      mob_name = Mg
      [../]

      [./interf_cg]
      type = CHInterface4
      variable = cgas
      mob_name = Mg
      kappa_name = kappa_c
      grad_mob_name = grad_Mg
      [../]


      [./interf_cu]
      type = CHInterface4
      variable = cgas
      mob_name = Mg
      kappa_name = kappa_c
      grad_mob_name = grad_Mg
      [../]


        [./cg_sink]
          type = CHRadSinkAnisoXeE
          variable = cgas
      mob_name=Mg
          [../]

[]

[AuxKernels]
  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
  [../]
[]

[BCs]
active = 'Periodic'

  [./Periodic]
    [./top_bottom]
      primary = 0
      secondary = 2
      translation = '0 5.0 0'
    [../]

    [./left_right]
      primary = 1
      secondary = 3
      translation = '-80 0 0'
    [../]

  [../]

[]


[Materials]
  [./LANL]
    type = UO2AnisoConsTempXe
    block = 0
    temp = 2000 # K
    c=cgas
    kappa_cg = 10.0
    M0GB = 2.5e-6 # m^4/(J*s); Increase for smalle time steps 2.5e-6
    QGB = 2.3 # eV
    sigma_GB = 0.708 # J/m^2
  var_name_base = gr
    HXe = 4.94 #4.94
    D0Xe = 3.2650799e+12 # nm^2/s
    L0 = 9.12 #eV

  [../]
[]

[Postprocessors]
  [./dt]
    type = TimestepSize
  [../]
  [./max_cgas]
    type = NodalMaxValue
    variable = cgas
  [../]
[]

[Executioner]
  type = Transient
  scheme = 'bdf2'
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 1e-4 # Initial time step.  In this simulation it changes.
  [../]
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 25
  nl_max_its = 10
  nl_rel_tol = 1.0e-10 #1.0e-14 #1.0e-16
  nl_abs_tol = 1.0e-10 #1.0e-14 #1e-16
  start_time = 0.0
  end_time = 1.00
  num_steps = 2
  dtmax = 1e12


 [./TimePeriods]
    [./p1]
     start = 0.0
inactive_kernels ='Diff_cg interf_cg interf_cu cg_sink'
    [../]

    [./p2]
      start = 0.00001
inactive_kernels ='PolycrystalKernel'
    [../]

  [../]



  [./Adaptivity]
    print_changed_info = true
    initial_adaptivity = 2
    refine_fraction = 0.5
    coarsen_fraction = 0.2
    max_h_level = 2 [../]
[]


[Outputs]
#  file_base = output/BULK_1
#  output_initial = true
#  interval = 1
  exodus = true
#  csv = true
#  console  = true
[]



