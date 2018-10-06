/* 
 * File: timersetup.c
 * Author: Tetsuya Idota and Brian Fehrman
 * Description: Contains function to setup timer
 * Created on (date): 3-11-2013
 * Updated on (date): 3-19-2013
 */

#include "timersetup.h"

////////////////////
// Function : TimerInit()
/////////////////////
void TimerInit(uint8_t index, uint8_t wgmod, uint8_t clksel, uint16_t period, uint8_t commatA, uint8_t commatB, uint8_t commatC){
	// parameter check //
	if(5 < index || 15 < wgmod || 7 < clksel || 3 < commatA || 3 < commatB || 3 < commatC)
		return; 

	sei(); //Enable global interrupts//

	switch(index){
		case 0:
			//WGM0 2:0 & CS0 2:0 & COM0A/B 1:0 //
			TCCR0A = (wgmod & 0x03);
			TCCR0B = ((wgmod & 0x04) << 1) + clksel;
			TCCR0A |= (commatA << COM0A0);
			TCCR0A |= (commatB << COM0B0);
	
			//Initialize TCNT0//
			if(wgmod == 0) //normal mode
				TCNT0 = 255 - period;
			else if(wgmod == 1){ //Phase correct
				TCNT0 = 255 - period;
			}else if(wgmod == 2){ //CTC mode
				TCNT0 = 0;
				OCR0A = period;
			}else if(wgmod == 3){ //Fast PWM
				TCNT0 = 255 - period;
			}else if(wgmod == 5){ //Phase correct
				TCNT0 = 0;
				OCR0A = period;
			}else if(wgmod == 7){ //Fast PWM
				TCNT0 = 0;
				OCR0A = period;
			}

			//Enable interrupt//
			// * if TOP is 0xFF, 
			// * ISR is required to initialize TCNTx with 255 - period.
			if(wgmod == 0) //normal mode
				TIMSK0 = 0x01;
			else if(wgmod == 1) //Phase correct
				TIMSK0 = 0x01;
			else if(wgmod == 2) //CTC mode
				TIMSK0 = 0x02;
			else if(wgmod == 3) //Fast PWM
				TIMSK0 = 0x01;
			break;
		case 1:
			//WGM1 2:0 & CS1 2:0 & COM1A/B/C 1:0 //
			TCCR1A = (wgmod & 0x03);
			TCCR1B = ((wgmod & 0x0C)<<1) + clksel;
			TCCR1A |= (commatA << COM1A0);
			TCCR1A |= (commatB << COM1B0);
			TCCR1A |= (commatC << COM1C0);
	
			//Initialize TCNT1//
			if(wgmod == 0){ //normal mode
				TCNT1H = 255 - ((period & 0xFF00) >> 8);
				TCNT1L = 255 - (period & 0x00FF);
			}else if(wgmod == 4){ //CTC mode
				TCNT1H = 0;
				TCNT1L = 0;
				OCR1AH = (period >> 8);
				OCR1AL = period;
			}else if(wgmod == 11){ //Phase correct
				TCNT1H = 0;
				TCNT1L = 0;
				OCR1AH = (period >> 8);
				OCR1AL = period;
			}else if(wgmod == 15){ //Fast PWM
				TCNT1H = 0;
				TCNT1L = 0;
				OCR1AH = (period >> 8);
				OCR1AL = period;
			}

			//Enable interrupt//
			if(wgmod == 0) //normal mode
				TIMSK1 = 0x01;
			else if(wgmod == 4) //CTC mode
				TIMSK1 = 0x02;
			else if(wgmod == 15) //Fast PWM
				TIMSK1 = 0x02;
			break;
		case 2:
			//WGM2 2:0 & CS2 2:0 & COM2A 1:0 //
			TCCR2A = (wgmod & 0x03);
			TCCR2B = ((wgmod & 0x04) << 1) + clksel;
			TCCR2A |= (commatA << COM2A0);
			TCCR2A |= (commatB << COM2B0);
	
			//Initialize TCNT2//
			if(wgmod == 0) //normal mode
				TCNT2 = 255 - period;
			else if(wgmod == 1){ //Phase correct
				TCNT2 = 255 - period;
			}else if(wgmod == 2){ //CTC mode
				TCNT2 = 0;
				OCR2A = period;
			}else if(wgmod == 3){ //Fast PWM
				TCNT2 = 255 - period;
			}else if(wgmod == 5){ //Phase correct
				TCNT2 = 0;
				OCR2A = period;
			}else if(wgmod == 7){ //Fast PWM
				TCNT2 = 0;
				OCR2A = period;
			}

			//Enable interrupt//
			// * if TOP is 0xFF, 
			// * ISR is required to initialize TCNTx with 255 - period.
			if(wgmod == 0) //normal mode
				TIMSK2 = 0x01;
			else if(wgmod == 2) //CTC mode
				TIMSK2 = 0x02;
			else if(wgmod == 3) //Fast PWM
				TIMSK2 = 0x01;
			break;
		case 3:
			//WGM3 2:0 & CS3 2:0 & COM3A/B/C 1:0 //
			TCCR3A = (wgmod & 0x03);
			TCCR3B = ((wgmod & 0x0C)<<1) + clksel;
			TCCR3A |= (commatA << COM3A0);
			TCCR3A |= (commatB << COM3B0);
			TCCR3A |= (commatC << COM3C0);
	
			//Initialize TCNT1//
			if(wgmod == 0){ //normal mode
				TCNT3H = 255 - ((period & 0xFF00) >> 8);
				TCNT3L = 255 - (period & 0x00FF);
			}else if(wgmod == 4){ //CTC mode
				TCNT3H = 0;
				TCNT3L = 0;
				OCR3AH = (period >> 8);
				OCR3AL = period;
			}else if(wgmod == 11){ //Phase correct
				TCNT3H = 0;
				TCNT3L = 0;
				OCR3AH = (period >> 8);
				OCR3AL = period;
			}else if(wgmod == 15){ //Fast PWM
				TCNT3H = 0;
				TCNT3L = 0;
				OCR3AH = (period >> 8);
				OCR3AL = period;
			}

			//Enable interrupt//
			if(wgmod == 0) //normal mode
				TIMSK3 = 0x01;
			else if(wgmod == 4) //CTC mode
				TIMSK3 = 0x02;
			else if(wgmod == 15) //Fast PWM
				TIMSK3 = 0x02;
			break;
		case 4:
			//WGM4 2:0 & CS4 2:0 & COM4A/B/C 1:0 //
			TCCR4A = (wgmod & 0x03);
			TCCR4B = ((wgmod & 0x0C)<<1) + clksel;
			TCCR4A |= (commatA << COM4A0);
			TCCR4A |= (commatB << COM4B0);
			TCCR4A |= (commatC << COM4C0);
	
			//Initialize TCNT1//
			if(wgmod == 0){ //normal mode
				TCNT4H = 255 - ((period & 0xFF00) >> 8);
				TCNT4L = 255 - (period & 0x00FF);
			}else if(wgmod == 4){ //CTC mode
				TCNT4H = 0;
				TCNT4L = 0;
				OCR4AH = (period >> 8);
				OCR4AL = period;
			}else if(wgmod == 11){ //Phase correct
				TCNT3H = 0;
				TCNT3L = 0;
				OCR3AH = (period >> 8);
				OCR3AL = period;
			}else if(wgmod == 15){ //Fast PWM
				TCNT4H = 0;
				TCNT4L = 0;
				OCR4AH = (period >> 8);
				OCR4AL = period;
			}

			//Enable interrupt//
			if(wgmod == 0) //normal mode
				TIMSK4 = 0x01;
			else if(wgmod == 4) //CTC mode
				TIMSK4 = 0x02;
			else if(wgmod == 15) //Fast PWM
				TIMSK4 = 0x02;
			break;
		case 5:
			//WGM5 2:0 & CS5 2:0 & COM5A/B/C 1:0 //
			TCCR5A = (wgmod & 0x03);
			TCCR5B = ((wgmod & 0x0C)<<1) + clksel;
			TCCR5A |= (commatA << COM5A0);
			TCCR5A |= (commatB << COM5B0);
			TCCR5A |= (commatC << COM5C0);
	
			//Initialize TCNT1//
			if(wgmod == 0){ //normal mode
				TCNT5H = 255 - ((period & 0xFF00) >> 8);
				TCNT5L = 255 - (period & 0x00FF);
			}else if(wgmod == 4){ //CTC mode
				TCNT5H = 0;
				TCNT5L = 0;
				OCR5AH = (period >> 8);
				OCR5AL = period;
			}else if(wgmod == 11){ //Phase correct
				TCNT5H = 0;
				TCNT5L = 0;
				OCR5AH = (period >> 8);
				OCR5AL = period;
			}else if(wgmod == 15){ //Fast PWM
				TCNT5H = 0;
				TCNT5L = 0;
				OCR5AH = (period >> 8);
				OCR5AL = period;
			}

			//Enable interrupt//
			if(wgmod == 0) //normal mode
				TIMSK5 = 0x01;
			else if(wgmod == 4) //CTC mode
				TIMSK5 = 0x02;
			else if(wgmod == 15) //Fast PWM
				TIMSK5 = 0x02;
			break;
		default:
			break;
	}
}

