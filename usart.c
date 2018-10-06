/* 
 * File:   usart.c
 * Author: Tetsuya Idota
 *
 * Contains functions for performing USART communication
 * on the Seeduino
 *
 * Updated on (Date): 2-19-2013
 */

#include "usart.h"

static FILE my_std_in = FDEV_SETUP_STREAM( NULL, usart0rx, _FDEV_SETUP_READ);
static FILE my_std_out = FDEV_SETUP_STREAM( usart0tx, NULL, _FDEV_SETUP_WRITE);

////////////////////
// Function : configure_usart
// Author: Tetsuya Idota
//
// Configures the Seeduino registers for USART communication
////////////////////

void configure_usart( usart_config *Ux ){
	//calculate baud prescale
	uint32_t baud_prescale = F_CPU/(Ux->baudrate*16UL)-1;
	
	//setting adjustment for flags
	//	(the values are changed to the bits setting, and invalid values are modified to the default settings)
	//data bits
	if(5 <= Ux->databits || Ux->databits <= 8)
		Ux->databits = Ux->databits - 5; // case of 5-8 bits
	else if(Ux->databits == 9)
		Ux->databits = 7;	//case of 9 bits
	else
		Ux->databits = 3; //if invalid value
	//paritymode
	if(Ux->paritymode > 3)
		Ux->paritymode = 0; //if invalid value
	//stopbits
	if(Ux->stopbits < 1 || 2 < Ux->stopbits)
		Ux->stopbits = 1; //if invalid value
	//modeselect
	if(Ux->modesel > 3)
		Ux->modesel = 0; //if invalid value

	//setup
	switch(Ux->number){
		case 0:
			UCSR0B |= 0x18; //RXEN TXEN
			UCSR0B |= ((Ux->databits & 0x04) << UCSZ02); //data bits
			UCSR0C |= ((Ux->databits & 0x03) << UCSZ00); //data bits
			UCSR0C |= (Ux->paritymode << UPM00); // parity mode
			UCSR0C |= (Ux->stopbits << USBS0); //stopbits
			UCSR0C |= (Ux->modesel << UMSEL00); //mode select
			UBRR0L = baud_prescale;
			UBRR0H = (baud_prescale >> 8);
			break;
		case 1:
			UCSR1B |= 0x18; //RXEN TXEN
			UCSR1B |= ((Ux->databits & 0x04) << UCSZ12); //data bits
			UCSR1C |= ((Ux->databits & 0x03) << UCSZ10); //data bits
			UCSR1C |= (Ux->paritymode << UPM10); // parity mode
			UCSR1C |= (Ux->stopbits << USBS1); //stopbits
			UCSR1C |= (Ux->modesel << UMSEL10); //mode select
			UBRR1L = baud_prescale;
			UBRR1H = (baud_prescale >> 8);
			break;
		case 2:
			UCSR2B |= 0x18; //RXEN TXEN
			UCSR2B |= ((Ux->databits & 0x04) << UCSZ22); //data bits
			UCSR2C |= ((Ux->databits & 0x03) << UCSZ20); //data bits
			UCSR2C |= (Ux->paritymode << UPM20); // parity mode
			UCSR2C |= (Ux->stopbits << USBS2); //stopbits
			UCSR2C |= (Ux->modesel << UMSEL20); //mode select
			UBRR2L = baud_prescale;
			UBRR2H = (baud_prescale >> 8);
			break;
		case 3:
			UCSR3B |= 0x18; //RXEN TXEN
			UCSR3B |= ((Ux->databits & 0x04) << UCSZ32); //data bits
			UCSR3C |= ((Ux->databits & 0x03) << UCSZ30); //data bits
			UCSR3C |= (Ux->paritymode << UPM30); // parity mode
			UCSR3C |= (Ux->stopbits << USBS3); //stopbits
			UCSR3C |= (Ux->modesel << UMSEL30); //mode select
			UBRR3L = baud_prescale;
			UBRR3H = (baud_prescale >> 8);
			break;
		default:
			break;
	
	}
}

////////////////////
// Function : ioinit
// Author: Tetsuya Idota and Brian Fehrman
//
// ioinit will setup the USARTs for communication and 
// reconfigure stdout and stdin to call our functions so
// that we can c functions, such as printf, for serial communication.
////////////////////
void init_usart()
{
	//Initialize USART0
	usart_config ucon_0;
	ucon_0.number = 0;
	ucon_0.baudrate = 9600;
	ucon_0.databits = 8; //8bits
	ucon_0.paritymode = 0; //disabled
	ucon_0.stopbits = 1; //1 bit
	ucon_0.modesel = 0; //Asynchronous
	configure_usart(&ucon_0);
	
/*
	//Initialize USART1
	usart_config ucon_1;
	ucon_1.number = 1;
	ucon_1.baudrate = 57600;
	ucon_1.databits = 8; //8bits
	ucon_1.paritymode = 0; //disabled
	ucon_1.stopbits = 1; //1 bit
	ucon_1.modesel = 0; //Asynchronous
	configure_usart(&ucon_1);
*/
	
	//Reconfigure stdout and stdin to call our functions
	stdout = &my_std_out;
	stdin = &my_std_in;
}

////////////////////
// Function : usartNtx
// Author: Tetsuya Idota
//
// The following functions transmit a byte on the specified
// USART channel.
// USART0 is optimized for the communication with the terminal
////////////////////
int usart0tx( char data, FILE* stream ){ // "char" instead for "uint8_t" to be compatible to the put function in FILE
	
	//handle newline with a recursive call
	if( ( char ) data == '\n' )
	{
		usart0tx( '\r', stream );
	}
	while((UCSR0A & (1<<UDRE0)) == 0){}
	UDR0 = data;
	
	return 0;
}
void usart1tx( uint8_t data ){
	while((UCSR1A & (1<<UDRE1)) == 0){}
	UDR1 = data;
}
void usart2tx( uint8_t data ){
	while((UCSR2A & (1<<UDRE2)) == 0){}
	UDR2 = data;
}
void usart3tx( uint8_t data ){
	while((UCSR3A & (1<<UDRE3)) == 0){}
	UDR3 = data;
}

////////////////////
// Function : usartNrx
// Author: Tetsuya Idota
//
// The following functions read a byte on the specified
// USART channel.
// USART0 is optimized for the communication with the terminal
////////////////////
int usart0rx(FILE *stream){ // "int" instead for "uint8_t" to be compatible to the get function in FILE
	while((UCSR0A & (1<<RXC0)) == 0){}
	return UDR0;
}
uint8_t usart1rx(){
	while((UCSR1A & (1<<RXC1)) == 0){}
	return UDR1;
}
uint8_t usart2rx(){
	while((UCSR2A & (1<<RXC2)) == 0){}
	return UDR2;
}
uint8_t usart3rx(){
	while((UCSR3A & (1<<RXC3)) == 0){}
	return UDR3;
}
