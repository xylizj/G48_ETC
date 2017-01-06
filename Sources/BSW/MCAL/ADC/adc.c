#include "adc.h"

/*global declare*/
bool convert_complete = TRUE;
adc_signal_t cur_convert = ADC_CHANNELS_COUNT;
adc_config_t ai_config_array[ADC_CHANNELS_COUNT] = 
{
    {AN_CHAN_0, 0},
};

/*
 * Function:ADC_Init
 * Param<void>:
 * Return<void>:
 * REQ IDs:TPMS_SW_BASE_0088,TPMS_SW_BASE_0089,TPMS_SW_BASE_0090,TPMS_SW_BASE_0091,TPMS_SW_BASE_0092
 *         TPMS_SW_BASE_0093,TPMS_SW_BASE_0094,TPMS_SW_BASE_0095,TPMS_SW_BASE_0096,TPMS_SW_BASE_0097
 * Discription:
 * Note:
*/
void ADC_Init(void)
{
    int16_t i16_ch_index = 0;
    uint8_t u8_channel = (uint8_t)ai_config_array[0].channel; 
    
    ATDCTL3_DJM = 1;       /*Result right justified*/
    ATDCTL3_FIFO = 1;      /*Fisrt-in-first-out mode*/
    ATDCTL3_S8C = 0;       /*1 conversion per sequence*/
    ATDCTL3_S4C = 0;
    ATDCTL3_S2C = 0;
    ATDCTL3_S1C = 1;
    ATDCTL1_SRES = 0x01;   /*10 bit resolution*/
    ATDDIEN = 0xFFFF;      /*Set All GPIO as Digital Input/Output for next initial*/
    ATDSTAT0_SCF = 1;      /*Clear sequence complete flag*/
#if ADC_CHANNELS_COUNT > 1
    ATDCTL2_ASCIE = 1;     /*Enable ATD sequence complete interrupt*/
#endif
    for (i16_ch_index = 0; i16_ch_index < ADC_CHANNELS_COUNT; i16_ch_index++)
    {
        u8_channel = (uint8_t)ai_config_array[i16_ch_index].channel; 
        ATDDIEN &= CLEAR_MASK(u8_channel);
    }
#if ADC_CHANNELS_COUNT == 1 /*If only one channel used trig-cvt*/
    ATDCTL5_Cx = (uint8_t)ai_config_array[0].channel;
#endif

}


/*
 * Function:ADC_Convert
 * Param<adc_signal_t channel_index>:
 * Return<io_err_t>:
 * REQ IDs:TPMS_SW_BASE_0098
 * Discription:
 * Note: 
*/
static io_err_t ADC_Convert(adc_signal_t channel_index)
{
    adc_channel_t   channel        = ai_config_array[channel_index].channel; 

    ATDCTL5_Cx = (uint8_t)channel;  /*Start ATD to corresponding channel*/ 
    return IO_ERR_OK;
} 

/*
 * Function:ADC_SingleChannelTrigger
 * Param<adc_signal_t channel_index>:
 * Return<io_err_t>:
 * REQ IDs:TPMS_SW_BASE_0455
 * Discription:
 * Note: 
*/
io_err_t ADC_SingleChannelTrigger(adc_signal_t channel_index)
{
    if (TRUE == convert_complete)
    {
        convert_complete = FALSE;
        cur_convert      = ADC_CHANNELS_COUNT;
  	  
        return ADC_Convert(channel_index);
    }
    else
    {
        return IO_ERR_OTHER_FAILURE;
    }
}

#if ADC_CHANNELS_COUNT > 1
/*
 * Function:ADC_SingleSequenceTrigger
 * Param<void>:
 * Return<io_err_t>:
 * REQ IDs:TPMS_SW_BASE_0099,TPMS_SW_BASE_0100
 * Discription:
 * Note: 
*/
io_err_t ADC_SingleSequenceTrigger(void) 
{
    if (TRUE == convert_complete)
    {
        convert_complete = FALSE;
        cur_convert = (adc_signal_t)(0);
        return ADC_Convert(cur_convert);
    }
    else
    {
        return IO_ERR_OTHER_FAILURE;
    }
}
#endif /*End of ADC_CHANNELS_COUNT > 1*/
/*
 * Function:ADC_ReadChannel
 * Param<adc_signal_t channel_index>:
 * Param<uint16_t* convert_value>:
 * Return<io_err_t>:
 * REQ IDs:TPMS_SW_BASE_0103,TPMS_SW_BASE_0104,TPMS_SW_BASE_0105
 * Discription:
 * Note: 
*/
#pragma INLINE
io_err_t ADC_ReadChannel(adc_signal_t channel_index,uint16_t *p_convert_value) 
{
    adc_channel_t   channel        = ai_config_array[channel_index].channel; 


    if(p_convert_value == 0) 
    {
        return IO_ERR_PARAMETER_ERROR;
    }
    else
    {
#if ADC_CHANNELS_COUNT > 1
        *p_convert_value = ai_config_array[channel_index].cvt_value;
        return IO_ERR_OK;
#else
        if (1 == ATDSTAT0_SCF)
        {
            convert_complete = TRUE;
            ATDSTAT0_SCF = 1; /*Clear Sequnce Complete Flag(unnecessary)*/
            *p_convert_value = ATDDR0;
            ATDCTL5_Cx = (uint8_t)ai_config_array[0].channel;
            return IO_ERR_OK;
        }
        else
        {
            return IO_ERR_BUSY;
        }
#endif /*End of ADC_CHANNELS_COUNT > 1*/
    } 
}


/*static bool PowerMonitor(void)
{
  uint16_t batVoltValue;
  static uint8_t offDebCnt;
  
  if(   IO_ERR_OK==ADC_ReadChannel(AN_CHAN_0, &batVoltValue)
     && batVoltValue<BAT_THREHOLD_ADC_VALUE)
  {     
    offDebCnt++;
  }
  else
  {
    offDebCnt = 0;
  }

  if(offDebCnt>=3)
  {
    offDebCnt = 0;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}*/