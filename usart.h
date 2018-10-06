/* 
 * File:   usart.h
 * Author: Tetsuya Idota
 *
 * Header file for usart.c
 */

#ifndef USART_H
#define	USART_H

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

// Refer to http://www.nongnu.org/avr-libc/user-manual/group__util__setbaud.html

// Feel free to modify this struct to include parameters you would like to be
// able to control
typedef struct tagusart_config
{
	uint8_t number;
	//float baudrate;     // or uint32_t if you prefer
	uint32_t baudrate;
	uint8_t databits;     // just specify the number of bits (5,6,7,8, or 9)
	uint8_t paritymode;
		//0 Disabled
		//1 Reserved
		//2 Enabled, Even Parity
		//3 Enabled, Odd Parity
		//other Disabled
	uint8_t stopbits; // just specify the number of bits (1 or 2)
	uint8_t modesel;
		//0 Asynchronous USART
		//1 Synchronous USART
		//2 (Reserved)
		//3 Master SPI (MSPIM)
		//other Asynchronous USART
    
	// fill this in with whatever features you want to use.
} usart_config;

void configure_usart( usart_config *Ux );
void init_usart();

// Use something like these . or
int usart0tx( char data, FILE* stream );
void usart1tx( uint8_t data );
void usart2tx( uint8_t data );
void usart3tx( uint8_t data );
int usart0rx(FILE* stream);
uint8_t usart1rx();
uint8_t usart2rx();
uint8_t usart3rx();
// use something like these
void usartNtx( uint8_t N, uint8_t data );
uint8_t usartNrx( uint8_t N );
// or both


// Possible to use macros, too, instead of either...
#define TXUSARTn(d,n) while ( !( UCSR##n##A & (1<<UDRE##n)) ); UDR##n = d

#endif	/* USART_H */

