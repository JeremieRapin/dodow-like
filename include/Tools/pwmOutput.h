/**
 * file: pwmOutput.h
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#ifndef _PWM_OUTPUT_
  #define _PWM_OUTPUT_

#include "Object/color.h"

void pwmOutput_Init();

void pwmOutput_Cleanup();

void pwmOutput_Stop();

void pwmOutput_Start();

void pwmOutput_Set(eColor_t color, int value);

#endif // _PWM_OUTPUT_
