/**
 ******************************************************************************
 **  Filename:      ETC_PID_data.c
 **  Date:          20-Dec-2016
 **
 **  Model Version: 1.76
 ******************************************************************************
 **/
#include "ETC_PID.h"
#include "ETC_PID_private.h"

/*  GLOBAL DATA DEFINITIONS */

/* Block parameters (auto storage) */
P_ETC_PID_T ETC_PID_P = {
  1374389535,                          /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S3>/Integrator'
                                        */
  1U,                                  /* Computed Parameter: Delay_DelayLength
                                        * Referenced by: '<S3>/Delay'
                                        */
  0,                                   /* Computed Parameter: Out_Y0
                                        * Referenced by: '<S2>/Out'
                                        */
  0,                                   /* Computed Parameter: Integrator_IC
                                        * Referenced by: '<S3>/Integrator'
                                        */
  0,                                   /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S4>/Gain'
                                        */
  0,                                   /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S3>/Constant'
                                        */
  0,                                   /* Computed Parameter: Delay_InitialCondition
                                        * Referenced by: '<S3>/Delay'
                                        */
  0,                                   /* Computed Parameter: Kp_PosNeg_Threshold
                                        * Referenced by: '<S3>/Kp_PosNeg'
                                        */
  1280,                                /* Computed Parameter: Constant_Value_k
                                        * Referenced by: '<S1>/Constant'
                                        */
  25600                                /* Computed Parameter: Gain_Gain_n
                                        * Referenced by: '<S3>/Gain'
                                        */
};

#pragma CONST_SEG CAL_RAM
const volatile t_Percent2 ETC_C_pc_INegThr = -10240;
const volatile t_Percent2 ETC_C_pc_IPosThr = 10240;
const volatile t_Percent2 ETC_C_pc_PosSetMax = 21760;
const volatile t_Percent2 ETC_C_pc_PosSetMin = -1280;
const volatile boolean_T ETC_OE_st_ETCPwmDutySet = 0;
const volatile boolean_T ETC_OE_st_ExPIDEn = 1;
const volatile uint16_T ETC_OV_pc_ETCPwmDutySet = 0U;
const volatile t_Percent2 ETC_c_fac_Kd = 0;
const volatile t_Percent2 ETC_c_fac_Ki = 768;
const volatile t_Percent2 ETC_c_fac_Kp = 2048;
const volatile t_Percent2 ETC_c_fac_KpNeg = 768;
const volatile t_Percent2 ETC_c_pc_PIDMax = 12800;
const volatile t_Percent2 ETC_c_pc_PIDMin = -12800;
const t_Torque1 ETC_MAPY_trq_ETCVlvPcSet[11] = { 0, 1,2,3,4,5,6,7,8,9,10} ;

const t_RotSpd1 ETC_MAPX_n_ETCVlvPcSetEngSpd[10] = { 800,900,1000,1100,1200,1300,1400,1500,1600,1700
  } ;

const int16_T ETC_MAP_pc_ETCVlvPcSet[10*11] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18} ;
#pragma CONST_SEG DEFAULT
/*======================== TOOL VERSION INFORMATION ==========================*
 * MATLAB 8.3 (R2014a)27-Dec-2013                                             *
 * Simulink 8.3 (R2014a)27-Dec-2013                                           *
 * Simulink Coder 8.6 (R2014a)27-Dec-2013                                     *
 * Embedded Coder 6.6 (R2014a)27-Dec-2013                                     *
 * Stateflow 8.3 (R2014a)27-Dec-2013                                          *
 * Fixed-Point Designer 4.2 (R2014a)27-Dec-2013                               *
 *============================================================================*/

/*======================= LICENSE IN USE INFORMATION =========================*
 * fixed_point_toolbox                                                        *
 * matlab_coder                                                               *
 * real-time_workshop                                                         *
 * rtw_embedded_coder                                                         *
 * simulink                                                                   *
 *============================================================================*/
