#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
////Include////
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_it.h"	 
//#include "fast_math_functions.h"
//////////////////////////

////Define///
#define Sample_period 1000 //1s
#define Start_up_change_step 2000-1
#define First_PWM_width 950
#define Min_PWM_width 170
#define Max_PWM_width 1050	 
#define TIM3_clock_start_up 100000 //(kHz)
#define TIM3_clock 500000 //(kHz)
#define TIM14_clock_start_up 1000000 //(Hz)
#define TIM14_clock 1000 //(Hz)
#define Sample_time 100 //(means 100ms for timer 14)	 
#define Z_cross_startup 50	 
//#define  PSEUDO_COMMUTATION
/////constant////////

/////Variable//////
extern int TIM1_Period;
extern int Frequency;	 
struct return4{
	float var1;
	float var2;
	float var3;
	uint8_t  var4;
};
	
/////Function/////
void Timer1_PWM_config(void);
void Timer1_PWM_Pin_Config(void);
void Timer3_sample_config(void);
void Sample_indicator_config(void);
void Timer14_sample_config(void);

//////////////////////	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__PWM_H */