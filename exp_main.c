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

// update: 140410-0815: debugged the communication
// update: 140413-0926: adjusted for the pc program

// variable time to delay
void delay_ms(uint16_t cnt_ms) {
    while(cnt_ms--) {
        _delay_ms(1);
    }
}

// === format of the data from sonar ===
// R<4 digits of range>
#define MAX_RX_SNR 6
uint8_t rx_snr(char *buff)
{
    int i = 0, j = 0;
    while(i < MAX_RX_SNR)
    {
        buff[i] = usart1rx();
        i++;
    }

    if(buff[0] != 'R' || buff[5] != 0x0D)
        return 0; //failed

    buff[0] = '0';
    buff[5] = 0x00;

    // check each character and count the 0's before data
    i = 0;
    while(i < MAX_RX_SNR - 1)
    {
        if(buff[i] != '0')
            break;
        if(buff[i] < '0' || '9' < buff[i])
            return 0; //failed
        i++;
    }

    // shift data to left
    j = 0;
    while(i + j < MAX_RX_SNR - 1)
    {
        buff[j] = buff[i+j];
        j++;
    }

    buff[j] = 0x00;

    return 1; //OK
}

// === format of the end character in every communciation ===
// to make sure that the end character is exactly 1 byte,
// the character is 0x03 (End of Text) instead of the carriage return (\n \r\n and so on)
// the end of "data" is supposed to be 0
void tx_usb(const char *data)
{
    int i =0;
    while(data[i] != 0x00)
    {
        printf("%c",data[i]);
        i++;
    }
    printf("%c\n",0x03);
}

// === format of data to PC ===
// data: <pitch><tab><yaw><tab><4 digits of number>
// OK: "OK"
// error: "E-"<message>
// *OK is to be used to respond for the initialization command "I"

// === conversions of angles for servos ===
// pitch,yaw => tilting servo (0 to 180)
#define Tlt(p,y) (((y) <= 180)? (p): (180-(p)))
// pitch,yaw => panning servo (0 to 180)
#define Pan(p,y) (((y) <= 180)? (y): ((y) - 180))


int main(void)
{    
    // variables for receiving data from sonar
    char r_snr[30]; // buffer of string

    // variables for communication with PC
    char t_usb[30]; // string to PC

    // === USB settings (for com. with PC) === //
    init_usart();

    // === sonar settings === //
    initSonar();

    //=== Main Loop ===//
    while(1)
    {
        // command the sonar to measure //
        measure();

        // receive data
        if(rx_snr(r_snr) == 0)
            continue; // restart the loop if the return value is 0
    
        // send the result to PC
        sprintf(t_usb, "%s\n", r_snr);
        tx_usb(t_usb);

        _delay_ms(500);
    }

    return 1;
} //main//


