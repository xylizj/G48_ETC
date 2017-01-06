/**
 *******************************************************************************
 **  FILE INFORMATION:
 **  Filename:           ETC_PID.c
 **  File Creation Date: 20-Dec-2016
 **
 **  ABSTRACT:
 **
 **
 **  NOTES:
 **
 **
 **  MODEL INFORMATION:
 **  Model Name:         ETC_PID
 **  Model Description:
 **  Model Version:      1.76
 **  Model Author:       Administrator - Fri Oct 28 15:53:01 2016
 **
 **  MODIFICATION HISTORY:
 **  Model at Code Generation: Administrator - Tue Dec 20 10:02:59 2016
 **
 **  Last Saved Modification:  Administrator - Tue Dec 20 10:02:26 2016
 **
 **
 *******************************************************************************
 **/
#include "ETC_PID.h"
#include "ETC_PID_private.h"

/*  Defines */

/*  Data Types */

/**************************** GLOBAL DATA *************************************/
/*  Definitions */

/* Exported block signals */
t_Percent2 ETC_m_pc_GovDvt;            /* '<S1>/Sum2' */
t_Percent2 ETC_m_pc_KiPartClamp;       /* '<S3>/Integrator' */
t_Percent2 ETC_m_pc_KiInputClmp;       /* '<S3>/Integral Gain' */
t_Percent2 ETC_m_pc_KpPartClamp;       /* '<S3>/Kp_PosNeg' */
t_Percent2 ETC_m_pc_OutClmpUnsat;      /* '<S3>/Sum' */
boolean_T ETC_m_st_ClmpSt;             /* '<S4>/AND' */

/* Block states (auto storage) */
DW_ETC_PID_T ETC_PID_DW;

/* Real-time model */
RT_MODEL_ETC_PID_T ETC_PID_M_;
RT_MODEL_ETC_PID_T *const ETC_PID_M = &ETC_PID_M_;

/* Exported data definition */

/* ConstVolatile memory section */
/* Definition for custom storage class: ConstVolatile */


/* Definition for custom storage class: Default */
t_Percent2 ETC_m_pc_ActualPos;
t_Percent2 ETC_m_pc_DutyOut;
t_Percent2 ETC_m_pc_ETCVlvSetPc;
t_Percent2 ETC_m_pc_ExtPIDout;
t_Percent2 ETC_m_pc_KdDiff;
t_Percent2 ETC_m_pc_KdDiffPerm;

/*  Declarations  */

/***************************** FILE SCOPE DATA ********************************/

/*************************** FUNCTIONS ****************************************/
void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo)
{
  uint32_T absIn0;
  uint32_T absIn1;
  uint32_T in0Lo;
  uint32_T in0Hi;
  uint32_T in1Hi;
  uint32_T productHiLo;
  uint32_T productLoHi;
  absIn0 = (uint32_T)(in0 < 0L ? -in0 : in0);
  absIn1 = (uint32_T)(in1 < 0L ? -in1 : in1);
  in0Hi = absIn0 >> 16UL;
  in0Lo = absIn0 & 65535UL;
  in1Hi = absIn1 >> 16UL;
  absIn0 = absIn1 & 65535UL;
  productHiLo = in0Hi * absIn0;
  productLoHi = in0Lo * in1Hi;
  absIn0 *= in0Lo;
  absIn1 = 0UL;
  in0Lo = (productLoHi << 16UL) + absIn0;
  if (in0Lo < absIn0) {
    absIn1 = 1UL;
  }

  absIn0 = in0Lo;
  in0Lo += productHiLo << 16UL;
  if (in0Lo < absIn0) {
    absIn1++;
  }

  absIn0 = (((productLoHi >> 16UL) + (productHiLo >> 16UL)) + in0Hi * in1Hi) +
    absIn1;
  if (!((in0 == 0L) || ((in1 == 0L) || ((in0 > 0L) == (in1 > 0L))))) {
    absIn0 = ~absIn0;
    in0Lo = ~in0Lo;
    in0Lo++;
    if (in0Lo == 0UL) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = in0Lo;
}

int32_T mul_s32_s32_s32_sr45_sat(int32_T a, int32_T b)
{
  int32_T result;
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  u32_clo = (uint32_T)((int32_T)u32_chi >> 13L);
  u32_chi = (int32_T)u32_chi >= 0L ? 0UL : MAX_uint32_T;
  if (((int32_T)u32_chi > 0L) || ((u32_chi == 0UL) && (u32_clo >= 2147483648UL)))
  {
    result = MAX_int32_T;
  } else if (((int32_T)u32_chi < -1L) || (((int32_T)u32_chi == -1L) && (u32_clo <
    2147483648UL))) {
    result = MIN_int32_T;
  } else {
    result = (int32_T)u32_clo;
  }

  return result;
}

/* Model step function */
void ETC_PID_step(void)
{
  t_Percent2 rtb_DataTypeConv2;
  int32_T tmp;
  int32_T tmp_0;
  int16_T u;
  int16_T rtb_DataTypeConv2_0;

  /* Saturate: '<S1>/Saturation' incorporates:
   *  Inport: '<Root>/ETC_m_pc_ETCVlvSetPc'
   */
  if (ETC_m_pc_ETCVlvSetPc > ETC_C_pc_PosSetMax) {
    rtb_DataTypeConv2 = ETC_C_pc_PosSetMax;
  } else if (ETC_m_pc_ETCVlvSetPc < ETC_C_pc_PosSetMin) {
    rtb_DataTypeConv2 = ETC_C_pc_PosSetMin;
  } else {
    rtb_DataTypeConv2 = ETC_m_pc_ETCVlvSetPc;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Sum: '<S1>/Sum2' incorporates:
   *  Inport: '<Root>/ETC_m_pc_ActualPos'
   */
  ETC_m_pc_GovDvt = rtb_DataTypeConv2 - ETC_m_pc_ActualPos;

  /* Outputs for Enabled SubSystem: '<S1>/Subsystem' incorporates:
   *  EnablePort: '<S2>/Enable'
   */
  /* Constant: '<S1>/Constant3' */
  if (ETC_OE_st_ExPIDEn) {
    if (!ETC_PID_DW.Subsystem_MODE) {
      /* InitializeConditions for DiscreteIntegrator: '<S3>/Integrator' */
      ETC_PID_DW.Integrator_DSTATE = ETC_PID_P.Integrator_IC;

      /* InitializeConditions for Delay: '<S3>/Delay' */
      ETC_PID_DW.Delay_DSTATE = ETC_PID_P.Delay_InitialCondition;
      ETC_PID_DW.Subsystem_MODE = true;
    }

    /* DiscreteIntegrator: '<S3>/Integrator' */
    ETC_m_pc_KiPartClamp = ETC_PID_DW.Integrator_DSTATE;

    /* DeadZone: '<S4>/DeadZone' */
    if (ETC_m_pc_KiPartClamp > ETC_C_pc_IPosThr) {
      tmp_0 = ETC_m_pc_KiPartClamp - (int32_T)ETC_C_pc_IPosThr;
      if (tmp_0 > 32767L) {
        tmp_0 = 32767L;
      } else {
        if (tmp_0 < -32768L) {
          tmp_0 = -32768L;
        }
      }

      rtb_DataTypeConv2 = (int16_T)tmp_0;
    } else if (ETC_m_pc_KiPartClamp >= ETC_C_pc_INegThr) {
      rtb_DataTypeConv2 = 0;
    } else {
      tmp_0 = ETC_m_pc_KiPartClamp - (int32_T)ETC_C_pc_INegThr;
      if (tmp_0 > 32767L) {
        tmp_0 = 32767L;
      } else {
        if (tmp_0 < -32768L) {
          tmp_0 = -32768L;
        }
      }

      rtb_DataTypeConv2 = (int16_T)tmp_0;
    }

    /* End of DeadZone: '<S4>/DeadZone' */

    /* Gain: '<S3>/Integral Gain' */
    tmp_0 = (int32_T)ETC_c_fac_Ki * ETC_m_pc_GovDvt >> 8;
    if (tmp_0 > 32767L) {
      tmp_0 = 32767L;
    } else {
      if (tmp_0 < -32768L) {
        tmp_0 = -32768L;
      }
    }

    ETC_m_pc_KiInputClmp = (int16_T)tmp_0;

    /* End of Gain: '<S3>/Integral Gain' */

    /* Gain: '<S4>/Gain' */
    tmp_0 = (int32_T)ETC_PID_P.Gain_Gain * ETC_m_pc_KiPartClamp >> 8;
    if (tmp_0 > 32767L) {
      tmp_0 = 32767L;
    } else {
      if (tmp_0 < -32768L) {
        tmp_0 = -32768L;
      }
    }

    /* DataTypeConversion: '<S4>/DataTypeConv2' */
    u = ETC_m_pc_KiInputClmp >> 8;

    /* Signum: '<S4>/SignPreSat' */
    if (rtb_DataTypeConv2 < 0) {
      rtb_DataTypeConv2_0 = -1;
    } else if (rtb_DataTypeConv2 > 0) {
      rtb_DataTypeConv2_0 = 1;
    } else {
      rtb_DataTypeConv2_0 = 0;
    }

    /* End of Signum: '<S4>/SignPreSat' */

    /* Signum: '<S4>/SignPreIntegrator' */
    if (u < 0) {
      u = -1;
    } else if (u > 0) {
      u = 1;
    } else {
      u = 0;
    }

    /* Logic: '<S4>/AND' incorporates:
     *  Gain: '<S4>/Gain'
     *  RelationalOperator: '<S4>/Equal'
     *  RelationalOperator: '<S4>/NotEqual'
     *  Signum: '<S4>/SignPreIntegrator'
     */
    ETC_m_st_ClmpSt = (((int16_T)tmp_0 != rtb_DataTypeConv2) &&
                       (rtb_DataTypeConv2_0 == u));

    /* Gain: '<S3>/Derivative Gain' */
    tmp_0 = (int32_T)ETC_c_fac_Kd * ETC_m_pc_GovDvt >> 8;
    if (tmp_0 > 32767L) {
      tmp_0 = 32767L;
    } else {
      if (tmp_0 < -32768L) {
        tmp_0 = -32768L;
      }
    }

    /* Sum: '<S3>/Sum1' incorporates:
     *  Delay: '<S3>/Delay'
     *  Gain: '<S3>/Derivative Gain'
     */
    ETC_m_pc_KdDiff = (int16_T)tmp_0 - ETC_PID_DW.Delay_DSTATE;

    /* Gain: '<S3>/Gain' */
    ETC_m_pc_KdDiffPerm = (int16_T)(ETC_PID_P.Gain_Gain_n * (int32_T)
      ETC_m_pc_KdDiff >> 7);

    /* Switch: '<S3>/Kp_PosNeg' incorporates:
     *  Gain: '<S3>/Proportional Gain Negative'
     *  Gain: '<S3>/Proportional Gain Postive'
     */
    if (ETC_m_pc_GovDvt > ETC_PID_P.Kp_PosNeg_Threshold) {
      /* Gain: '<S3>/Proportional Gain Postive' */
      tmp = (int32_T)ETC_c_fac_Kp * ETC_m_pc_GovDvt >> 8;
      if (tmp > 32767L) {
        tmp = 32767L;
      } else {
        if (tmp < -32768L) {
          tmp = -32768L;
        }
      }

      ETC_m_pc_KpPartClamp = (int16_T)tmp;
    } else {
      /* Gain: '<S3>/Proportional Gain Negative' */
      tmp = (int32_T)ETC_c_fac_KpNeg * ETC_m_pc_GovDvt >> 8;
      if (tmp > 32767L) {
        tmp = 32767L;
      } else {
        if (tmp < -32768L) {
          tmp = -32768L;
        }
      }

      ETC_m_pc_KpPartClamp = (int16_T)tmp;
    }

    /* End of Switch: '<S3>/Kp_PosNeg' */

    /* Sum: '<S3>/Sum' */
    tmp = (int32_T)ETC_m_pc_KpPartClamp + ETC_m_pc_KiPartClamp;
    if (tmp > 32767L) {
      tmp = 32767L;
    } else {
      if (tmp < -32768L) {
        tmp = -32768L;
      }
    }

    tmp += (int32_T)ETC_m_pc_KdDiffPerm;
    if (tmp > 32767L) {
      tmp = 32767L;
    } else {
      if (tmp < -32768L) {
        tmp = -32768L;
      }
    }

    ETC_m_pc_OutClmpUnsat = (int16_T)tmp;

    /* End of Sum: '<S3>/Sum' */

    /* Saturate: '<S3>/Saturation' */
    if (ETC_m_pc_OutClmpUnsat > ETC_c_pc_PIDMax) {
      ETC_m_pc_ExtPIDout = ETC_c_pc_PIDMax;
    } else if (ETC_m_pc_OutClmpUnsat < ETC_c_pc_PIDMin) {
      ETC_m_pc_ExtPIDout = ETC_c_pc_PIDMin;
    } else {
      ETC_m_pc_ExtPIDout = ETC_m_pc_OutClmpUnsat;
    }

    /* End of Saturate: '<S3>/Saturation' */

    /* Switch: '<S3>/Switch' incorporates:
     *  Constant: '<S3>/Constant'
     */
    if (ETC_m_st_ClmpSt) {
      tmp = (int32_T)ETC_PID_P.Constant_Value << 7;
    } else {
      tmp = (int32_T)ETC_m_pc_KiInputClmp << 7;
    }

    /* End of Switch: '<S3>/Switch' */

    /* Update for DiscreteIntegrator: '<S3>/Integrator' */
    tmp = mul_s32_s32_s32_sr45_sat(ETC_PID_P.Integrator_gainval, tmp);
    if (tmp > 32767L) {
      tmp = 32767L;
    } else {
      if (tmp < -32768L) {
        tmp = -32768L;
      }
    }

    tmp += ETC_PID_DW.Integrator_DSTATE;
    if (tmp > 32767L) {
      tmp = 32767L;
    } else {
      if (tmp < -32768L) {
        tmp = -32768L;
      }
    }

    ETC_PID_DW.Integrator_DSTATE = (int16_T)tmp;

    /* End of Update for DiscreteIntegrator: '<S3>/Integrator' */

    /* Update for Delay: '<S3>/Delay' incorporates:
     *  Gain: '<S3>/Derivative Gain'
     */
    ETC_PID_DW.Delay_DSTATE = (int16_T)tmp_0;
  } else {
    if (ETC_PID_DW.Subsystem_MODE) {
      /* Disable for Outport: '<S2>/Out' */
      ETC_m_pc_ExtPIDout = ETC_PID_P.Out_Y0;
      ETC_PID_DW.Subsystem_MODE = false;
    }
  }

  /* End of Constant: '<S1>/Constant3' */
  /* End of Outputs for SubSystem: '<S1>/Subsystem' */

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Constant: '<S1>/Constant2'
   */
  if (ETC_OE_st_ETCPwmDutySet) {
    ETC_m_pc_DutyOut = (int16_T)((uint32_T)ETC_OV_pc_ETCPwmDutySet * 41943UL >>
      14);
  } else {
    ETC_m_pc_DutyOut = ETC_m_pc_ExtPIDout;
  }

  /* End of Switch: '<S1>/Switch1' */
}

/* Model initialize function */
void ETC_PID_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(ETC_PID_M, (NULL));

  /* block I/O */

  /* exported global signals */
  ETC_m_pc_GovDvt = 0;
  ETC_m_pc_KiPartClamp = 0;
  ETC_m_pc_KiInputClmp = 0;
  ETC_m_pc_KpPartClamp = 0;
  ETC_m_pc_OutClmpUnsat = 0;
  ETC_m_st_ClmpSt = false;

  /* custom signals */
  ETC_m_pc_DutyOut = 0;
  ETC_m_pc_KdDiff = 0;
  ETC_m_pc_KdDiffPerm = 0;
  ETC_m_pc_ExtPIDout = 0;

  /* states (dwork) */
  (void) memset((void *)&ETC_PID_DW, 0,
                sizeof(DW_ETC_PID_T));

  /* external inputs */
  ETC_m_pc_ETCVlvSetPc = 0;
  ETC_m_pc_ActualPos = 0;

  /* InitializeConditions for Enabled SubSystem: '<S1>/Subsystem' */
  /* InitializeConditions for DiscreteIntegrator: '<S3>/Integrator' */
  ETC_PID_DW.Integrator_DSTATE = ETC_PID_P.Integrator_IC;

  /* InitializeConditions for Delay: '<S3>/Delay' */
  ETC_PID_DW.Delay_DSTATE = ETC_PID_P.Delay_InitialCondition;

  /* End of InitializeConditions for SubSystem: '<S1>/Subsystem' */

  /* Start for Enabled SubSystem: '<S1>/Subsystem' */
  /* VirtualOutportStart for Outport: '<S2>/Out' */
  ETC_m_pc_ExtPIDout = ETC_PID_P.Out_Y0;

  /* End of Start for SubSystem: '<S1>/Subsystem' */
}

/* Model terminate function */
void ETC_PID_terminate(void)
{
  /* (no terminate code required) */
}

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
