/*
 * PWM.h
 *
 *  Created on: Oct 18, 2016
 *      Author: Administrator
 */

#ifndef PWM_H_
#define PWM_H_

void PWM0_Init(void);
void PWM0_SetPeriod(unsigned char PerValue);
extern void PWM0_SetDuty(unsigned char DutyValue);

void Pwm01DrvDuty(unsigned int DutyValue);


#endif /* PWM_H_ */
