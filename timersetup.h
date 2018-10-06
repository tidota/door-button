/* 
 * File: timersetup.h
 * Author: Tetsuya Idota and Brian Fehrman
 * Description: function prototypes to setup timer
 * Created on (date): 3-11-2013
 * Updated on (date): 3-19-2013
 */

#ifndef _TIMERSETUP_H
#define _TIMERSETUP_H

#include <avr/interrupt.h>
#include <inttypes.h>

////////////////////
// Function : TimerInit()
// Purpose : Initialize and setup timerX
// Parameter :
//    index: index for timerX(0-5)
//    wgmod: Wave form generation mode
//    clksel: clock select
//    period: value which the counter counts up
//    commatA/B/C: compare match
/////////////////////
void TimerInit(uint8_t index, uint8_t wgmod, uint8_t clksel, uint16_t period, uint8_t commatA, uint8_t commatB, uint8_t commatC);

#endif
