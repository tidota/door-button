/* 
 * File: pwmsetup.h
 * Author: Tetsuya Idota and Brian Fehrman
 * Description: function prototypes to setup pwm
 * Created on 3-12-2013
 * Updated on 4-20-2013
 * Updated on 3-19-2014 : setPwm_duty2() and InitPwm2() added
 */

#ifndef _PWMSETUP_H
#define _PWMSETUP_H

#include <avr/interrupt.h>
#include <inttypes.h>
#include "timersetup.h"

//Definition//
#define PWM_PERIOD 0.02
#define PWM_MAX_RATE (0.0024/PWM_PERIOD)
#define PWM_MID_RATE (0.0015/PWM_PERIOD)
#define PWM_MIN_RATE (0.0006/PWM_PERIOD)

// this is the macro to set degrees for the servo motor
//       the input is in the rage of 0 to 180
//       the result is the duty and it can be assigned to the variable "duty" directly
#define setDegServo(indx,deg) (SetPwm_duty(indx, (PWM_MAX_RATE - PWM_MIN_RATE)*(float)(deg)/180.0 + PWM_MIN_RATE))
// double signal version (for use of both OCnB and OCnC)
#define setDegServo2(itimer,isig,deg) (SetPwm_duty2(itimer, isig, (PWM_MAX_RATE - PWM_MIN_RATE)*(float)(deg)/180.0 + PWM_MIN_RATE))

extern float period;

// === prototypes === //

////////////////////
// Function : setPwm_duty()
// Purpose : initialize Pwm_duty
// Parameter : index, setting rate (0 to 1)
// Return: current rate (0 to 1) (*setting rate may not affect if it is out of the range)
/////////////////////
float SetPwm_duty(uint8_t index, float rate);

////////////////////
// Function : initPwm()
// Purpose : initialize PWM
// Parameter : index, rate [0,1] (%), max_period to inturrupt for timer
/////////////////////
void InitPwm(uint8_t index, float rate, float max_period);

////////////////////
// Function : setPwm_duty2()
// Purpose : initialize Pwm_duty (one timer can provide 2 signals OCnB + OCnC)
//           it applies only 16-bit timers (1, 3, 4, and 5)
// Parameter : index of timer, index of sig (0 for B, 1 for C), setting rate (0 to 1)
// Return: current rate (0 to 1) (*setting rate may not affect if it is out of the range)
/////////////////////
float SetPwm_duty2(uint8_t itimer, uint8_t isig, float rate);

////////////////////
// Function : initPwm2()
// Purpose : initialize PWM (one timer can provide 2 signals OCnB + OCnC)
//           it applies only 16-bit timers (1, 3, 4, and 5)
// Parameter : index, rate [0,1] (%), max_period to inturrupt for timer
/////////////////////
void InitPwm2(uint8_t index, float rate, float max_period);

#endif
