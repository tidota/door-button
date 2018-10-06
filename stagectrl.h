// stagectrl.h
//
// this file defines functionality for control of the stage (servos of the pan and tilt kit)
// control of each servo is calibrated for its error respectively.


#ifndef _STAGECTRL_H
#define _STAGECTRL_H

#include "pwmsetup.h"

// interval in degrees of servo //
#define DEGINT 5
// the minimum degrees of servo //
#define DEGMIN 0
// the maximum degrees of servo //
#define DEGMAX 180

// initialization of servos
void initStage();

// note:
// timer3 is used for control of servos
//  output D2(OC3B) is for tilting servo
//  output D3(OC3C) is for panning servo
//  the controlling function is calibrated for each servo

// controlling function for tilting servo
float setDegTlt(float deg);

// controlling funcion for panning servo
float setDegPan(float deg);

#endif // _STAGECTRL_H
