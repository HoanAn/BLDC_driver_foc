#ifndef __CONTRONLLER_H
#define __CONTRONLLER_H

#ifdef __cplusplus
 extern "C" {
#endif
////Include////
#include <stdint.h>
#include "dsp/fast_math_functions.h"
//////////////////////////

////Define///

/////////////

/////Variable//////
extern int TIM1_Period;
/////Function/////
uint8_t Space_vector_Modulation(float alpha, float beta, uint32_t* T_on_A, uint32_t* T_on_B,uint32_t* T_on_C);
void vector_rotate(float mag, float* angle, int period_ms, int Timer_time, float* alpha, float* beta);
//////////////////////	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__CONTRONLLER_H */