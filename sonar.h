// sonar.h
//
// this file defineds the functionality for setting of the sonar.
// it includes the setting of USART which is used for communication with the sensor.

// output to sonar: PORTB7 - D13
// input from sonar: usart1

#ifndef _SONAR_H
#define _SONAR_H

#include "usart.h"

// initialize the settings for sonar
void initSonar();

// command to take measurement
void measure();


#endif // _SONAR_H
