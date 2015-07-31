[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 1
  nz = 0
  xmin = 0
  xmax = 20
  ymin = 0
  ymax = 5
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 1
#  crys_num = 1
  var_name_base = gr
  v = 'gr0'
temp =2000
[]

[Variables]
  [./PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
 [../]

  [./iu]
    order = THIRD
    family = HERMITE
    scaling=1.0
    [./InitialCondition]
      type = ConstantIC
value = 2.640011e-06
    [../]
  [../]

  [./cgas]
    order = THIRD
    family = HERMITE
  scaling=1.0
    [./InitialCondition]
      type = ConstantIC
      value = 2.490887e-06
    [../]
  [../]

  [./cvac]
    order = THIRD
    family = HERMITE
scaling=1.0e0
    [./InitialCondition]
     type = ConstantIC
      value = 4.904904e-05
    [../]
  [../]

      [./cgasvac]
      order = THIRD
      family = HERMITE
scaling=1.0
    [./InitialCondition]
     type = ConstantIC
      value = 2.287580e-05
    [../]
      [../]

          [./cgasvacvac]
          order = THIRD
          family = HERMITE
scaling=1.0
    [./InitialCondition]
     type = ConstantIC
      value = 6.481994e-08
    [../]
          [../]

      [./cvacvac]
    order = THIRD
    family = HERMITE
scaling=1.0e-2
   [./InitialCondition]
      type = ConstantIC
value = 4.070600e-10
    [../]
 [../]

[]

[]

[AuxVariables]
  [./cgasTOT]
    order = FIRST
    family = LAGRANGE
  [../]
  [./cuTOT]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
active = 'PolycrystalKernel ie_cg ie_cgv ie_cv ie_cvv ie_iu ie_cgvv Diff_cg Diff_cv Diff_cvv Diff_cui Diff_cgvv Rate_cg Rate_cgv Rate_cvg Rate_cv Rate_cvv Rate_uiva Rate_vaui Rate_uiva2 Rate_va2ui Rate_va_va2ui Rate_uixeva Rate_xevaui Rate_xe_xevaui Rate_xeva2ui Rate_ui_xeva2ui Rate_xeva_xeva2ui Rate_cxeva2_xeva2M Rate_cva2xeva2M Rate_cxexeva2M Rate_xeva2_xeva2B Rate_va_xeva2B Rate_xeva_xeva2B cv_void cvv_void cui_void UISource VaSource interf_cgv interf_cv interf_cvv interf_cgvu interf_cvu interf_cvvu interf_cui interf_cgvv interf_cgvvu'

      [./PolycrystalKernel]
      #time_periods = p1
      [../]

          [./ie_iu]
          type = TimeDerivative
          variable = iu
          [../]

      [./ie_cg]
      type = TimeDerivative
      variable = cgas
      [../]

      [./ie_cgv]
      type = TimeDerivative
      variable = cgasvac
      [../]

          [./ie_cgvv]
          type = TimeDerivative
          variable = cgasvacvac
          [../]

      [./ie_cv]
      type = TimeDerivative
      variable = cvac
      [../]

      [./ie_cvv]
      type = TimeDerivative
      variable = cvacvac
      [../]

          [./Diff_cui]
          type = UIDiffusion
          variable = iu
          mob_name = MI
          [../]

      [./Diff_cg]
      type = XeVaDiffusion
      variable = cgasvac
      c2 = cvac
      c1 = cgas
      c4 = cvacvac
      LXeVa_name = LgXeVa
      LUVa_name = LgUVa
      LXeU_name = LgXeU
      LXeVaU_name = LgXeVaU
      LXeVaVa_name = LgXeVaVa
      LXeVaXe_name = LgXeVaXe
      LXeVa2_name = LgXeVa2
      LXeVaVa2_name = LgXeVaVa2
      LUVa2_name = LgUVa2
      LVaVa2_name = LgVaVa2
      mob_name = Mg
      [../]


          [./Diff_cgvv]
          type = XeVa2Diffusion
          variable = cgasvacvac
          LXeVa_name = LgXeVa
          LUVa_name = LgUVa
          LXeU_name = LgXeU
      LXeVaU_name = LgXeVaU
          LXeVaVa_name = LgXeVaVa
          LXeVaXe_name = LgXeVaXe
          LXeVa2_name = LgXeVa2
          LXeVaVa2_name = LgXeVaVa2
          LUVa2_name = LgUVa2
          LVaVa2_name = LgVaVa2
          mob_name = Mgv2
          [../]


      [./Diff_cv]
      type = VaDiffusion
      variable = cvac
      c1 = cgas
      c3 = cgasvac
      c4 = cvacvac
      LXeVa_name = LgXeVa
      LUVa_name = LgUVa
      LXeU_name = LgXeU
      LXeVaU_name = LgXeVaU
      LXeVaVa_name = LgXeVaVa
      LXeVaXe_name = LgXeVaXe
      LXeVa2_name = LgXeVa2
      LXeVaVa2_name = LgXeVaVa2
      LUVa2_name = LgUVa2
      LVaVa2_name = LgVaVa2
      mob_name = Mv
      [../]

      [./Diff_cvv]
      type = VaVaDiffusion
      variable = cvacvac
      c2 = cvac
      c1 = cgas
      c3 = cgasvac
      LXeVa_name = LgXeVa
      LUVa_name = LgUVa
      LXeU_name = LgXeU
      LXeVaU_name = LgXeVaU
      LXeVaVa_name = LgXeVaVa
      LXeVaXe_name = LgXeVaXe
      LXeVa2_name = LgXeVa2
      LXeVaVa2_name = LgXeVaVa2
      LUVa2_name = LgUVa2
      LVaVa2_name = LgVaVa2
      mob_name = Mv2

[../]

      [./interf_cgv]
      type = CHInterface4
      variable = cgasvac
      mob_name = Mg
      kappa_name = kappa_cGV
      grad_mob_name = grad_Mg
      [../]

          [./interf_cgvv]
          type = CHInterface4
          variable = cgasvacvac
          mob_name = Mgv2
          kappa_name = kappa_cGVV
          grad_mob_name = grad_Mgv2
          [../]


          [./interf_cgvvu]
          type = CHInterface3
          variable = cgasvacvac
          mob_name = Mgv2
c1=cgas
c2=cvac
c3=cgasvac
c4=cvacvac
          kappa_name = kappa_cGVVU
          grad_mob_name = grad_Mgv2
          [../]

      [./interf_cv]
      type = CHInterface4
      variable = cvac
      mob_name = Mv
      kappa_name = kappa_cV
      grad_mob_name = grad_Mv
      [../]


      [./interf_cvv]
      type = CHInterface4
      variable = cvacvac
      mob_name = Mv2
      kappa_name = kappa_cVV
      grad_mob_name = grad_Mv2
      [../]

      [./interf_cgvu]
      type = CHInterface3
      variable = cgasvac
      mob_name = Mg
c1=cgas
c2=cvac
c3=cgasvac
c4=cvacvac
      kappa_name = kappa_cGVU
      grad_mob_name = grad_Mg
      [../]

      [./interf_cvu]
      type = CHInterface2
      variable = cvac
      mob_name = Mv
c1=cgas
c2=cvac
c3=cgasvac
c4=cvacvac
      kappa_name = kappa_cVU
      grad_mob_name = grad_Mv
      [../]


      [./interf_cvvu]
      type = CHInterface3
      variable = cvacvac
      mob_name = Mv2
c1=cgas
c2=cvac
c3=cgasvac
c4=cvacvac
  kappa_name = kappa_cVVU
      grad_mob_name = grad_Mv2
      [../]

         [./interf_cui]
          type = CHInterface
          variable = iu
          mob_name = MI
          kappa_name = kappa_cUI
          grad_mob_name = grad_MI
          [../]

          [./Rate_uiva]
          type = UIVaRate
          variable = iu
          MVa_name = Mv
          MUI_name = MI
          c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_vaui]
          type = VaUIRate
          variable = cvac
          MVa_name = Mv
          MUI_name = MI
          c5=iu
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_uiva2]
          type = UIVa2Rate
          variable = iu
          MVa2_name = Mv2
          MUI_name = MI
          c4=cvacvac
      c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

        [./Rate_va2ui]
          type = Va2UIRate
          variable = cvacvac
          MVa2_name = Mv2
          MUI_name = MI
          c5=iu
      c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

       [./Rate_va_va2ui]
          type = Va_Va2UIRate
          variable = cvac
          MVa2_name = Mv2
          MUI_name = MI
          c5=iu
      c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_uixeva]
          type = UIXeVaRate
          variable = iu
          MXeVa_name = Mg
          MUI_name = MI
          c3=cgasvac
      c1=cgas
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_xevaui]
          type = XeVaUIRate
          variable = cgasvac
          MXeVa_name = Mg
          MUI_name = MI
          c5=iu
      c1=cgas
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_xe_xevaui]
          type = Xe_XeVaUIRate
          variable = cgas
          MXeVa_name = Mg
          MUI_name = MI
          c5=iu
      c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_xeva2ui]
          type = XeVa2UIRate
          variable = cgasvacvac
          MXeVa2_name = Mgv2
          MUI_name = MI
          c5=iu
          c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_ui_xeva2ui]
          type = UI_XeVa2UIRate
          variable = iu
          MXeVa2_name = Mgv2
          MUI_name = MI
          c6=cgasvacvac
          c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_xeva_xeva2ui]
          type = XeVa_XeVa2UIRate
          variable = cgasvac
          MXeVa2_name = Mgv2
          MUI_name = MI
          c6=cgasvacvac
          c5=iu
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

      [./Rate_cg]
      type = Xe_XeVaRate
      variable = cgas
      mob_name = Mv
      c2=cvac
      c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
      [../]

      [./Rate_cvg]
      type = Va_XeVaRate
      variable = cvac
      mob_name = Mv
      c1=cgas
      c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
      [../]

      [./Rate_cgv]
      type = XeVaRate
      variable = cgasvac
      mob_name = Mv
      c1=cgas
      c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
      [../]

      [./Rate_cv]
      type = Va_VaVaRate
      variable = cvac
      mob_name = Mv
      c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
      [../]

      [./Rate_cvv]
      type = VaVaRate
      variable = cvacvac
      mob_name = Mv
      c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
      [../]


          [./Rate_cxeva2]
          type = XeVa2Rate
          variable = cgasvac
          mob_name = Mv2
          mob_name2 = Mg
          mob_name3 = Mv
          c1=cgas
      c2=cvac
      c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_cva2xeva2]
          type = Va2_XeVa2Rate
          variable = cvacvac
          mob_name = Mv2
          mob_name2 = Mg
          mob_name3 = Mv
          c1=cgas
      c2=cvac
      c3=cgasvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_cvaxeva2]
          type = Va_XeVa2Rate
          variable = cvac
          mob_name = Mv2
          mob_name2 = Mg
          c1=cgas
      c3=cgasvac
      c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_cxexeva2]
          type = Xe_XeVa2Rate
          variable = cgas
          mob_name = Mv2
          mob_name2 = Mg
          c2=cvac
          c3=cgasvac
          c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_cxeva2_xeva2M]
          type = XeVa2_XeVa2MRate
          variable = cgasvacvac
          mob_name = Mv2
          c1=cgas
          c6=cgasvacvac
          c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_cva2xeva2M]
          type = Va2_XeVa2MRate
          variable = cvacvac
          mob_name = Mv2
          c1=cgas
          c6=cgasvacvac
          c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

         [./Rate_cxexeva2M]
          type = Xe_XeVa2MRate
          variable = cgas
          mob_name = Mv2
          c1=cgas
          c6=cgasvacvac
          c4=cvacvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_xeva2_xeva2B]
          type = XeVa2_XeVa2BRate
          variable = cgasvacvac
          mob_name = Mv
          mob_name2 = Mg
          c6=cgasvacvac
          c3=cgasvac
          c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_xeva_xeva2B]
          type = XeVa_XeVa2BRate
          variable = cgasvac
          mob_name = Mv
          mob_name2 = Mg
          c6=cgasvacvac
          c3=cgasvac
          c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./Rate_va_xeva2B]
          type = Va_XeVa2BRate
          variable = cvac
          mob_name = Mv
          mob_name2 = Mg
          c6=cgasvacvac
          c3=cgasvac
          c2=cvac
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

          [./UISource]
          type = UISource
          variable = iu
          R = 4.05e-6
          Factor = 1.0
          [../]

         [./VaSource]
          type = VaSource
          variable = cvac
          R = 4.05e-6
          Factor = 1.0
          [../]

        [./Va2Source]
          type = VaSource
          variable = cvacvac
          R = 4.05e-6/4
          Factor = 1.0
          [../]

       [./cv_void]
          type = VaSinkRate
          variable = cvac
          MVa_name=Mv
          LogC_name = LogC
          LogTol_name = LogTol
          [../]


       [./cvv_void]
          type = Va2SinkRate
          variable = cvacvac
          MVa2_name=Mv2
          LogC_name = LogC
          LogTol_name = LogTol
          [../]

       [./cui_void]
       type = UISinkRate
       variable = iu
       MUI_name=MI
       LogC_name = LogC
       LogTol_name = LogTol
       [../]

[]

[AuxKernels]
  [./Cg_aux]
    type = CgTOT3Aux
    variable = cgasTOT
    coupled1 = cgas
    coupled2 = cgasvac
    coupled3 = cgasvacvac
#    coupled4 = cgasgasvac
  [../]
  [./Cu_aux]
    type = CuTOT3Aux
    variable = cuTOT
    coupled1 = cgas
    coupled2 = cvac
    coupled3 = cgasvac
    coupled4 = cvacvac
    coupled5 = cgasvacvac
  [../]
[]

[BCs]
active = 'Neumann_gasleft Neumann_gasright Neumann_gasvacleft Neumann_gasvacright Neumann_vacleft Neumann_vacright Neumann_vacvacleft Neumann_vacvacright Neumann_iuleft Neumann_iuright Neumann_cgvvleft Neumann_cgvvright'

  [./Periodic]
    [./top_bottom]
      primary = 0
      secondary = 2
      translation = '0 5.0 0'
    [../]

    [./left_right]
      primary = 1
      secondary = 3
      translation = '-20 0 0'
    [../]

  [../]

  [./Neumann_gasleft]
    type = NeumannBC
    variable = cgas
    boundary = 'left'
    value = 0.0
  [../]

  [./Neumann_gasright]
    type = NeumannBC
    variable = cgas
    boundary = 'right'
    value = 0.0
  [../]

  [./Neumann_gasvacleft]
    type = NeumannBC
    variable = cgasvac
    boundary = 'left'
    value = 1.0e-10 #1e-10
  [../]

  [./Neumann_gasvacright]
    type = NeumannBC
    variable = cgasvac
    boundary = 'right'
    value = -1.0e-10 #-1e10
  [../]

  [./Neumann_vacleft]
    type = NeumannBC
    variable = cvac
    boundary = 'left'
    value = 0.0
  [../]

  [./Neumann_vacright]
    type = NeumannBC
    variable = cvac
    boundary = 'right'
    value = 0.0
  [../]

  [./Neumann_vacvacleft]
    type = NeumannBC
    variable = cvacvac
    boundary = 'left'
    value = 0
  [../]

  [./Neumann_vacvacright]
    type = NeumannBC
    variable = cvacvac
    boundary = 'right'
    value = 0
  [../]

  [./Neumann_iuleft]
    type = NeumannBC
    variable = iu
    boundary = 'left'
    value = 0.0
  [../]

  [./Neumann_iuright]
    type = NeumannBC
    variable = iu
    boundary = 'right'
    value = 0.0
  [../]

  [./Neumann_cgvvleft]
    type = NeumannBC
    variable = cgasvacvac
    boundary = 'left'
    value = 2.783e-09
  [../]

  [./Neumann_cgvvright]
    type = NeumannBC
    variable = cgasvacvac
    boundary = 'right'
    value = -2.783e-09
  [../]

  [./Neumann_cggvleft]
    type = NeumannBC
    variable = cgasgasvac
    boundary = 'left'
    value = 4.715e-15 #0.0
  [../]

  [./Neumann_cggvright]
    type = NeumannBC
    variable = cgasgasvac
    boundary = 'right'
    value = -4.715e-15 #0.0
  [../]

[]


[Materials]
  [./LANL]
    type = UO2AnisoConsTempXeUVa2
    block = 0
    temp = 2000 # K
    c=cgas
    c1 = cgas
    c2 = cvac
    c3 = cgasvac
    c4 = cvacvac
    c6 = cgasvacvac
    kappa_cg = 10.0
    M0GB = 2.5e-6 # m^4/(J*s); Increase for smalle time steps 2.5e-6
    QGB = 2.3 # eV
    sigma_GB = 0.708 # J/m^2
    wGB = 20 # nm
  crys_num = 1
  var_name_base = gr
    kappa_cu = 1.0
    kappa_cv = 10.0
    kappa_cvv = 10.0
    kappa_cgv = 10.0
    kappa_cgvv = 10.0
    kappa_cggv = 10.0
    kappa_cui = 10.0
    LXeVa = -1.82*6 #eV
    LUVa =  0.30*6 #eV
    LXeU = 1.52*6 #eV
    LXeVaU = 5.040964939*1 #eV
    LXeVaVa= -3.274396364*2 #eV
    LXeVaXe= -3.862906545*2 #eV
    LXeVa2= -4.753279212*2 #eV
    LXeVaVa2= 16.489800*1 #eV
    LUVa2=  4.80*1 #eV
    LVaVa2= -1.67*2 #eV
    LTXeVa = 0.0 #eV
    LTUVa = 0.0 #eV
    LTXeU = 0.0 #eV
    LTXeVaU = 0.0 #eV
    LTXeVaVa= 0.0 #eV
    LTXeVaXe= 0.0 #eV
    LTXeVa2= 0.0 #eV
    LTXeVaVa2= 0.0 #eV
    LTUVa2= 0.0 #eV
    LTVaVa2= 0.0 #eV
    EBVaVa = -0.30 #eV
    SBVaVa = 7.18643981 #eV
    EBXeVa = -1.82 #eV
    SBXeVa = 1.97 #eV
    EBXeVa2 = -4.753279212 #eV
    SBXeVa2 = 0.0 #eV
    EBXeVa2B = -3.274396364 #eV
    SBXeVa2B = 0.0 # eV
    EBXeXeVa = -2.235 #eV
    SBXeXeVa = 0.0 # eV
    EBXeVaXeVa = -1.566000 #eV
    SBXeVaXeVa = 0.0 #eV
    HXe = 4.94 #eV
    D0Xe = 1.481e13 # nm^2/s
    HV = 4.72 #eV
    D0V = 7.121e11 # nm^2/s
    HV2 = 2.84 #eV
    D0V2 = 5.446e13 #nm^2/s
    HI = 4.7 #eV
    D0I = 1e12 #nm^2/s
    D0GV2 = 1.481e13 #nm^2/s
    HGV2 = 3.619752 #eV
    D0GGV = 7.121e11 #nm^2/s
    HGGV = 5.276308 #eV
    Z = 1.0
    dHVaUI = 13.2 #eV
    dHVa2 = -0.30 #eV
dSVa2 = +7.18643981 #kB
    dHVa = -0.189862667 #eV
dSVa = -16.34924227 #eV
    dHUI = 13.2 #eV
dSHUI= 0 #kB
    dHXeVaUI = 11.38 #eV
    dHXeVa2UI = 8.1056036 #eV
     ki2=0.012564
    kv2=0.012564
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
  [./max_cgasvac]
    type = NodalMaxValue
    variable = cgasvac
  [../]

  [./max_cvac]
    type = NodalMaxValue
    variable = cvac
  [../]

  [./max_cvacvac]
    type = NodalMaxValue
    variable = cvacvac
  [../]

  [./max_iu]
    type = NodalMaxValue
    variable = iu
  [../]

  [./max_cgasvacvac]
    type = NodalMaxValue
    variable = cgasvacvac
  [../]

  [./Diff]
    type = DiffusivityBox
    variable = cgas
    length = 20
    flux = 1e-7
    c3 = cgasvac
  [../]
[]

[Executioner]

[Executioner]
  type = Transient
  scheme = 'bdf2'
  perf_log = true
  [./TimeStepper]
    type = SolutionTimeAdaptiveDT
    dt = 1e1 # Initial time step.  In this simulation it changes.
  [../]
  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'



  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 101'
  l_max_its = 50
  nl_max_its = 20
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1e-10
  start_time = 0.0
  end_time = 150000000000000000.0
  num_steps = 1
  dtmax = 1e12
  max_increase = 10

  [./Adaptivity]
[]

#[Outputs]
#  linear_residuals = true
#  file_base = output2000BF/BULK_1
#  output_initial = true
#  interval = 1
#  exodus = true
#  csv = true
#  console = true
#  [./checkpoint]
#    type = Checkpoint
#    num_files = 2
#  [../]
#  max_pps_rows_screen = 10
#[]


[Outputs]
  linear_residuals = true
#  file_base = output1600BF/BULK_1
#  output_initial = true
#  interval = 1
  exodus = true
# csv = true
#  console = true
#  [./checkpoint]
#    type = Checkpoint
#    num_files = 2
#  [../]
#  max_pps_rows_screen = 10
[]




