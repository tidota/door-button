#include "stagectrl.h"


// initialization of servos
void initStage(){
	// IO setting
	DDRE = 0xFF; // set as output
	PORTE = 0xFF; // set as 1's

	//* port for sensor is set in the function
	//    using timer3, PMW duty(center), max period
	InitPwm2(3, (PWM_MAX_RATE + PWM_MIN_RATE)/2, 20000.0);
	
	//set 90 deg for both servos	
	//setDegTlt(90);
	//setDegPan(90);
}


// note:
// timer3 is used for control of servos
//  output D2(OC3B) is for tilting servo
//  output D3(OC3C) is for panning servo
//  the controlling function is calibrated for each servo

// controlling function for tilting servo
float setDegTlt(float deg){
	return setDegServo2(3,0,deg);
}
// controlling funcion for panning servo
float setDegPan(float deg){
	return setDegServo2(3,1,deg);
}

