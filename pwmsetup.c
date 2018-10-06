/* 
 * File: pwmsetup.c
 * Author: Tetsuya Idota and Brian Fehrman
 * Description: Contains function to setup pwm
 * Created on 3-12-2013
 * Updated on 3-19-2014 : setPwm_duty2() and InitPwm2() added
 */

#include "pwmsetup.h"

float period; //value to set into OCRxA

////////////////////
// Function : setPwm_duty()
/////////////////////
float SetPwm_duty(uint8_t index, float rate){
	//range check
	if(rate > PWM_MAX_RATE * 1.1)
		rate = PWM_MAX_RATE * 1.1;
	else if(rate < PWM_MIN_RATE * 0.9)
		rate = PWM_MIN_RATE * 0.9;

	uint16_t buff;
	//set the value
	switch(index){
		case 0:
			OCR0B = (uint8_t)(period*rate+0.5);
			break;
		case 1:
			buff = (uint16_t)(period*rate+0.5);
			OCR1BH = (buff >> 8);
			OCR1BL = buff;
			break;
		case 2:
			OCR2B = (uint8_t)(period*rate+0.5);
			break;
		case 3:
			buff = (uint16_t)(period*rate+0.5);
			OCR3BH = (buff >> 8);
			OCR3BL = buff;
			break;
		case 4:
			buff = (uint16_t)(period*rate+0.5);
			OCR4BH = (buff >> 8);
			OCR4BL = buff;
			break;
		case 5:
			buff = (uint16_t)(period*rate+0.5);
			OCR5BH = (buff >> 8);
			OCR5BL = buff;
			break;
		default:
			break;
	}
	return rate;
}

////////////////////
// Function : initPwm()
/////////////////////
void InitPwm(uint8_t index, float rate, float max_period){
	//period
	period = max_period;

	//setup for PWM//
	SetPwm_duty(index, rate);
	

	//initialize timer//
	switch(index){
		case 0:
			TimerInit(index,5,5,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 1:
			TimerInit(index,11,2,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 2:
			TimerInit(index,5,7,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 3:
			TimerInit(index,11,2,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 4:
			TimerInit(index,11,2,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 5:
			TimerInit(index,11,2,period,2,2,0); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		default:
			break;
	}
	
}

////////////////////
// Function : setPwm_duty2()
/////////////////////
float SetPwm_duty2(uint8_t itimer, uint8_t isig, float rate){
	//range check
	//if(rate > PWM_MAX_RATE * 2.0)
	//	rate = PWM_MAX_RATE * 2.0;
	//else if(rate < PWM_MIN_RATE * 0.5)
	//	rate = PWM_MIN_RATE * 0.5;

	uint16_t buff = (uint16_t)(period*rate+0.5);
	//set the value
	switch(itimer){
		case 1:
			if(isig==0){
				OCR1BH = (buff >> 8);
				OCR1BL = buff;
			}else if(isig==1){
				OCR1CH = (buff >> 8);
				OCR1CL = buff;
			}
			break;
		case 3:
			if(isig==0){
				OCR3BH = (buff >> 8);
				OCR3BL = buff;
			}else if(isig==1){
				OCR3CH = (buff >> 8);
				OCR3CL = buff;
			}
			break;
		case 4:
			if(isig==0){
				OCR4BH = (buff >> 8);
				OCR4BL = buff;
			}else if(isig==1){
				OCR4CH = (buff >> 8);
				OCR4CL = buff;
			}
			break;
		case 5:
			if(isig==0){
				OCR5BH = (buff >> 8);
				OCR5BL = buff;
			}else if(isig==1){
				OCR5CH = (buff >> 8);
				OCR5CL = buff;
			}
			break;
		default:
			break;
	}
	return rate;
}

////////////////////
// Function : initPwm2()
/////////////////////
void InitPwm2(uint8_t index, float rate, float max_period){
	//period
	period = max_period;

	//setup for PWM//
	SetPwm_duty(index, rate);
	

	//initialize timer//
	switch(index){
		case 1:
			TimerInit(index,11,2,period,2,2,2); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 3:
			TimerInit(index,11,2,period,2,2,2); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 4:
			TimerInit(index,11,2,period,2,2,2); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		case 5:
			TimerInit(index,11,2,period,2,2,2); //index, phase correct PWM, clksel, 250 counts to interrupt
			break;
		default:
			break;
	}
}
