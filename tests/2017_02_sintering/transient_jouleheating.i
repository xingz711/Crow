[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmax = 5
  ymax = 5
  uniform_refine = 1
[]

[Variables]
  [./T]
    initial_condition = 300.0
  [../]
  [./elec]
  [../]
[]

[Kernels]
  [./HeatDiff]
    type = HeatConduction
    variable = T
  [../]
  [./HeatTdot]
    type = HeatConductionTimeDerivative
    variable = T
  [../]
  [./HeatSrc]
    type = JouleHeatingSource
    variable = T
    elec = elec
  [../]
  [./electric]
    type = HeatConduction
    variable = elec
    diffusion_coefficient = electrical_conductivity
  [../]
[]

[BCs]
  #[./lefttemp]
  #  type = DirichletBC
  #  boundary = left
  #  variable = T
  #  value = 20
  #[../]
  [./elec_left]
    type = DirichletBC
    variable = elec
    boundary = left
    value = 100
  [../]
  [./elec_right]
    type = DirichletBC
    variable = elec
    boundary = right
    value = 0
  [../]
[]

[Functions]
  [./elec_resist]
    type = PiecewiseLinear
    y = '5.5  10.5  24.30  55.70  90.40  108.50 '
    x = '293   500   1000    2000   3000   3500'
  [../]
[]

[Materials]
  [./k]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '0.4' #copper in cal/(cm sec C)
    block = 0
  [../]
  [./cp]
    type = GenericConstantMaterial
    prop_names = 'specific_heat'
    prop_values = '0.032' #W in cal/(g C)
    block = 0
  [../]
  [./rho]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '19.3' #W in g/(cm^3)
    block = 0
  [../]
  #[./sigma]
  #  type = ElectricalConductivity
  #  temp = T
  #  ref_temp = 300
  #  ref_resistivity = 5.5e-8 # in ohm-m
  #  temp_coeff = 0.0046
  #  length_scale = 1e-02
  #[../]
  [./resis]
    type = DerivativeParsedMaterial
    f_name = electrical_resist
    args = 'T'
    function ='if(T<=293, 5.5,
               if(T<=500, (10.5-5.5)/(500-293)*(T-293),
               if(T<=1000, (24.30-10.5)/500*(T-500),
               if(T<=2000, (55.70-24.30)/1000*(T-1000),
               if(T<=3000, (90.40-55.70)/1000*(T-2000),
               if(T<=3500, (108.50-90.40)/500*(T-3000),
                  108.50))))))'
  [../]
  [./sigma]
    type = DerivativeParsedMaterial
    f_name = electrical_conductivity
    material_property_names = electrical_resist
    function = 1.0/(electrical_resist)
    args = T
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
  scheme = bdf2
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         101   preonly   ilu      1'
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  l_tol = 1e-4
  dt = 0.1
  end_time = 20
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]
