# This input file models the growth of voids in post-irradiation annealing of UO2.
# Simulation is 2-D
# Mesh adaptivity and time step adaptivity are used
# Postprocessors are used to record time step and the number of bubbles

[Mesh]
  # Mesh block.  Meshes can be read in or automatically generated
  type = GeneratedMesh # Here, the mesh is automatically generated
  dim = 2 # Problem dimension, here 2D
  nx = 24 # Coarsest number of elements in the x-direction
  ny = 24 # Coarsest number of elements in the y-direction
  xmax = 60 # Max x-coordinate of the mesh
  ymax = 60 # Max y-coordinate of the mesh
  elem_type = QUAD4 # Element type
  uniform_refine = 2 # Initial number of uniformed refinements of the mesh
[]

[GlobalParams]
  polynomial_order = 8
[]

[Variables]
  # Variable block, where all variables in the simulation are declared
  [./c]
  [../]
  [./w]
    scaling = 1e9 # Scaling parameter for variable
  [../]
[]

[ICs]
  [./c_IC]
    # Initial condition for this variable
    type = LatticeSmoothCircleIC # This initial condition creates circles that are randomly varied from a set lattice
    variable = c # Variable corresponding to the IC
    radius = 1.0 # Mean radius of each circle
    invalue = 1.0 # Initial value inside circles
    outvalue = 0.005 # Initial value outside circles
    int_width = 1.0 # Interfacial width
    circles_per_side = '6 6 0' # Number of circles per side in the lattice
    Rnd_variation = 3.0 # Random variation in circle position
    radius_variation = 0.1 # Random variation in circle radius
    radius_variation_type = uniform
  [../]
[]

[AuxVariables]
  [./T]
  [../]
[]

[Kernels]
  # Kernel block, where the kernels defining the residual equations are set up. The problem uses the split form of the Cahn-Hilliard equation
  [./c_res]
    # Defines the chemical potential, using an 8th order polynomial free energy
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    w = w
    f_name = F
  [../]
  [./w_res]
    # Cahn-hilliard equation coupled to the w variable
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    # Time derivative of c for w residual
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
[]

[Functions]
  [./Temp_grad]
    # This funciton defines the temperature increase for the annealing
    type = PiecewiseLinear
    x = '0 500e3'
    y = '1073 2073'
  [../]
[]

[AuxKernels]
  [./Temp]
    type = FunctionAux
    variable = T
    function = Temp_grad
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
  [./XeinUO2]
    # This material defines all required properties to model the gas migration within UO2. The time scale is in ms and the length scale in nm.
    type = PFParamsPolyFreeEnergy
    block = 0
    c = c
    T = T
    time_scale = 1.0e-3
    length_scale = 1.0e-9
    int_width = 0.65 # nm
    D0 = 5.0e-5 # m^2/s
    Em = 3.9 # eV
    Ef = 4.35 # eV
    surface_energy = 0.6 # J/m^2
  [../]
  [./PolyFreeEnergy]
    type = PolynomialFreeEnergy
    c = c
    c_eq_name = c_eq
    block = 0
    derivative_order = 2
    enable_jit = true
    f_name = F
  [../]
[]

[Postprocessors]
  [./dt]
    # Records time step size
    type = TimestepSize
    execute_on = timestep_end
  [../]
  [./T]
    # Temperature at each time step
    type = ElementAverageValue
    variable = T
    execute_on = timestep_end
  [../]
  [./Por]
    # Percent of domain that is a bubble
    type = Porosity
    variable = c
    execute_on = timestep_end
  [../]
  [./n_nodes]
    # Records current number of nodes
    type = NumNodes
    execute_on = timestep_end
  [../]
  [./n_bubbles]
    # Counts the number of bubbles.
    type = FeatureFloodCount
    variable = c
    bubble_volume_file = bubble_volume
  [../]
[]

[Preconditioning]
  # This block defines special instructions about the preconditioner
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  # dt = 1.0
  # Preconditioned JFNK (default)
  type = Transient # Type of executioner, here it is transient with an adaptive time step
  scheme = bdf2 # Type of time integration, here we use 2nd order backward euler.
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  l_max_its = 30 # Max number of linear iterations
  l_tol = 1.0e-4 # Relative tolerance for linear solves
  nl_max_its = 15 # Max number of nonlinear iterations
  nl_rel_tol = 1.0e-9 # Relative tolerance for nonlienar solves
  nl_abs_tol = 1.0e-11 # Absolute tolerance for nonlienar solves
  start_time = 0.0
  num_steps = 100
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 2.0e4 # Initial time step
    growth_factor = 1.25 # Maximum about by which the time step and increase each step
    optimal_iterations = 6 # Time step will change to maintain this number of nonlinear iterations
  [../]
  [./Adaptivity]
    # Block that turns on mesh adaptivity. Note that mesh will never coarsen beyond initial mesh (before uniform refinement)
    initial_adaptivity = 2 # Number of times mesh is adapted to initial condition
    refine_fraction = 0.7 # Fraction of high error that will be refined
    coarsen_fraction = 0.05 # Fraction of low error that will coarsened
    max_h_level = 4 # Max number of refinements used, starting from initial mesh (before uniform refinement)
    weight_names = 'c w' # This combo of values makes the mesh adapt to c but not w
    weight_values = '1.0 0.0'
  [../]
[]

[Outputs]
  file_base = bub_growth # Output base file name.  Note the output will be saved in the "output" directory, that MUST be created before you run the simulation
  output_initial = true # Output initial condition
  interval = 1
  exodus = true
  print_linear_residuals = true
  print_perf_log = true
[]
