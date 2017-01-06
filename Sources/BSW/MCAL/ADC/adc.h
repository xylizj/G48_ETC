#ifndef __ADC_H__
#define __ADC_H__

#include "typedefs.h"
#include "derivative.h"


#define ADC_CHANNELS_COUNT 1


#if ADC_CHANNELS_COUNT

typedef enum __ADC_CHANNEL_T__
{
    AN_CHAN_0 = 0,
    AN_CHANNEL_1,
    AN_CHANNEL_2,
    AN_CHANNEL_3,
    AN_CHANNEL_4,
    AN_CHANNEL_5,
    AN_CHANNEL_6,
    AN_CHANNEL_7,
    ADC_CHANNEL_MAX
}adc_channel_t;

typedef enum __ADC_SIGNAL_T__
{
   ADC_SIGNAL_RSSI = 0,
   ADC_SIGNALS_COUNT
}adc_signal_t;

typedef struct __adc_config_type__
{ 
   adc_channel_t channel;
   uint16_t      cvt_value;
}adc_config_t;/*REQ IDs: TPMS_SW_BASE_0090*/



#define SET_MASK(bit) (1ul << (bit))
#define CLEAR_MASK(bit)   (~(1ul << (bit)))




#define T15_THREHOLD_VOLT       5//unit V
#define T15_THREHOLD_ADC_VALUE  ((1024*T15_THREHOLD_VOLT/11)/5)  //resolution is 10bits //AD采集的参考电压5V

#define BAT_THREHOLD_VOLT       5//unit V
#define BAT_THREHOLD_ADC_VALUE  ((1024*BAT_THREHOLD_VOLT/11)/5)  //resolution is 10bits //AD采集的参考电压5V







extern void ADC_Init(void);
extern io_err_t ADC_SingleChannelTrigger(adc_signal_t channel_index);
extern io_err_t ADC_ReadChannel(adc_signal_t channel_index,uint16_t* convert_value);


#endif /*ADC_CHANNELS_COUNT*/
#endif /*End of __ADC_H__*/

/*--------EOF--------*/
