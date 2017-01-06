/**
 ******************************************************************************
 **  Filename:      ETC_PID.h
 **  Date:          20-Dec-2016
 **
 **  Model Version: 1.76
 ******************************************************************************
 **/

#ifndef RTW_HEADER_ETC_PID_h_
#define RTW_HEADER_ETC_PID_h_
#include <stddef.h>
#include <string.h>
#ifndef ETC_PID_COMMON_INCLUDES_
# define ETC_PID_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* ETC_PID_COMMON_INCLUDES_ */

#include "ETC_PID_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  t_Percent2 Integrator_DSTATE;        /* '<S3>/Integrator' */
  t_Percent2 Delay_DSTATE;             /* '<S3>/Delay' */
  boolean_T Subsystem_MODE;            /* '<S1>/Subsystem' */
} DW_ETC_PID_T;

/* Parameters (auto storage) */
struct P_ETC_PID_T_ {
  int32_T Integrator_gainval;          /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S3>/Integrator'
                                        */
  uint16_T Delay_DelayLength;          /* Computed Parameter: Delay_DelayLength
                                        * Referenced by: '<S3>/Delay'
                                        */
  t_Percent2 Out_Y0;                   /* Computed Parameter: Out_Y0
                                        * Referenced by: '<S2>/Out'
                                        */
  t_Percent2 Integrator_IC;            /* Computed Parameter: Integrator_IC
                                        * Referenced by: '<S3>/Integrator'
                                        */
  t_Percent2 Gain_Gain;                /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S4>/Gain'
                                        */
  t_Percent2 Constant_Value;           /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S3>/Constant'
                                        */
  int16_T Delay_InitialCondition;      /* Computed Parameter: Delay_InitialCondition
                                        * Referenced by: '<S3>/Delay'
                                        */
  t_Percent2 Kp_PosNeg_Threshold;      /* Computed Parameter: Kp_PosNeg_Threshold
                                        * Referenced by: '<S3>/Kp_PosNeg'
                                        */
  t_Percent2 Constant_Value_k;         /* Computed Parameter: Constant_Value_k
                                        * Referenced by: '<S1>/Constant'
                                        */
  int16_T Gain_Gain_n;                 /* Computed Parameter: Gain_Gain_n
                                        * Referenced by: '<S3>/Gain'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_ETC_PID_T {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern P_ETC_PID_T ETC_PID_P;

/* Block states (auto storage) */
extern DW_ETC_PID_T ETC_PID_DW;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern t_Percent2 ETC_m_pc_GovDvt;     /* '<S1>/Sum2' */
extern t_Percent2 ETC_m_pc_KiPartClamp;/* '<S3>/Integrator' */
extern t_Percent2 ETC_m_pc_KiInputClmp;/* '<S3>/Integral Gain' */
extern t_Percent2 ETC_m_pc_KpPartClamp;/* '<S3>/Kp_PosNeg' */
extern t_Percent2 ETC_m_pc_OutClmpUnsat;/* '<S3>/Sum' */
extern boolean_T ETC_m_st_ClmpSt;      /* '<S4>/AND' */

/* Model entry point functions */
extern void ETC_PID_initialize(void);
extern void ETC_PID_step(void);
extern void ETC_PID_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ETC_PID_T *const ETC_PID_M;

/* Exported data declaration */

/* ConstVolatile memory section */
/* Declaration for custom storage class: ConstVolatile */
extern const volatile t_Percent2 ETC_C_pc_INegThr;
extern const volatile t_Percent2 ETC_C_pc_IPosThr;
extern const volatile t_Percent2 ETC_C_pc_PosSetMax;
extern const volatile t_Percent2 ETC_C_pc_PosSetMin;
extern const volatile boolean_T ETC_OE_st_ETCPwmDutySet;
extern const volatile boolean_T ETC_OE_st_ExPIDEn;
extern const volatile uint16_T ETC_OV_pc_ETCPwmDutySet;
extern const volatile t_Percent2 ETC_c_fac_Kd;
extern const volatile t_Percent2 ETC_c_fac_Ki;
extern const volatile t_Percent2 ETC_c_fac_Kp;
extern const volatile t_Percent2 ETC_c_fac_KpNeg;
extern const volatile t_Percent2 ETC_c_pc_PIDMax;
extern const volatile t_Percent2 ETC_c_pc_PIDMin;
extern const volatile t_Torque1 ETC_MAPY_trq_ETCVlvPcSet[11];
extern const volatile t_RotSpd1 ETC_MAPX_n_ETCVlvPcSetEngSpd[10];
extern const volatile int16_T ETC_MAP_pc_ETCVlvPcSet[10*11];

/* Declaration for custom storage class: Default */
extern t_Percent2 ETC_m_pc_ActualPos;
extern t_Percent2 ETC_m_pc_DutyOut;
extern t_Percent2 ETC_m_pc_ETCVlvSetPc;
extern t_Percent2 ETC_m_pc_ExtPIDout;
extern t_Percent2 ETC_m_pc_KdDiff;
extern t_Percent2 ETC_m_pc_KdDiffPerm;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ETC_PID'
 * '<S1>'   : 'ETC_PID/ETCPID'
 * '<S2>'   : 'ETC_PID/ETCPID/Subsystem'
 * '<S3>'   : 'ETC_PID/ETCPID/Subsystem/PIDCtl'
 * '<S4>'   : 'ETC_PID/ETCPID/Subsystem/PIDCtl/Clamping circuit'
 */
#endif                                 /* RTW_HEADER_ETC_PID_h_ */

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
