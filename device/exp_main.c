/* ============================================================================
 * File: 
 * Author: Tetsuya Idota
 * Created: October 6, 2018
 *
 * Desciption:
 * The main code to control the microcontroller.
============================================================================ */

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "usart.h"
#include "sonar.h"

/* ============================================================================
   # of digits which sonar data contains
   R<4 digits of range>0x0D
============================================================================ */
#define RNG_DIG 4

void initPORTA();
int buttonPressed();
void flashLED(int);
uint8_t getMeasurement(char *);
void delay_ms(uint16_t);

/* ============================================================================
 * Function: main
 *
 * Desciption:
 * It first initializes connections with the sonar and PC, and the pin
 * settings.
 * In the while loop, it constantly checks the button status. Once the button
 * is pressed, it takes sonar measurements, sends a command to beep to the PC
 * through USB, and flashes the LED ligtht.
 * If the sonar results indicate the person is not present (1000+ mm of range),
 * it sends a command to send an email.
============================================================================ */

int main(void)
{    
    init_usart();

    initSonar();

    initPORTA();

    while (1)
    {
        if (buttonPressed())
        {
            char r_snr[10];
            int range;

            //printf("beep\n");

            getMeasurement(r_snr);
            sscanf(r_snr, "%d", &range);
            if (range >= 1000)
            {
                //printf("email\n");
            }

            flashLED(500);
        }
        else
        {
            _delay_ms(100);
        }
    }

    return 1;
}

/* ============================================================================
 * Function: initPORTA
 *
 * Desciption:
 * Initializes port A for the button (input at A4) and LED (output at A0).
============================================================================ */

void initPORTA()
{
    DDRA = 0x0F;
    PORTA = 0xF0;
}

/* ============================================================================
 * Function: buttonPressed
 *
 * Desciption:
 * Returns 1 if the button is pressed. Otherwise, 0.
 *
 * Note: when it is not pressed, the pin is high. Otherwise, it is low.
============================================================================ */

int buttonPressed()
{
    int pressed = 0;

    if (PINA & 0x10 == 0)
        pressed = 1;

    return pressed;
}

/* ============================================================================
 * Function: flashLED
 *
 * Desciption:
 * flashes the LED during the time specified by the parameter in msec.
============================================================================ */

void flashLED(int duration)
{
    int elapsed = 0;
    while (elapsed < duration)
    {
        PORTA |= 0x01;
        delay_ms(100);
        PORTA &= 0xFE;
        delay_ms(100);
        elapsed += 200;
    }
}

/* ============================================================================
 * Function: getMeasurement
 *
 * Desciption:
 * takes sonar measurement until it gets reasonable data, and stores the
 * retults in the character array specified by the parameter.
============================================================================ */

uint8_t getMeasurement(char *buff)
{
    while(1)
    {
        int done = 1;

        measure();

        while(usart1rx() != 'R'){}

        int i = 0;
        while(i < RNG_DIG)
        {
            buff[i] = usart1rx();
            if (buff[i] < '0' || '9' < buff[i])
                done = 0;
	    i++;
        }
        usart1rx();

        if(done)
            break;
    }

    buff[RNG_DIG] = '\0';

    return 1;
}

/* ============================================================================
 * Function: delay_ms
 *
 * Desciption:
 * It waits for the time given by the parameter.
============================================================================ */

void delay_ms(uint16_t cnt_ms)
{
    while(cnt_ms--)
    {
        _delay_ms(1);
    }
}

