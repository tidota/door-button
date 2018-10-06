#include "sonar.h"


// initialize the settings for sonar
void initSonar(){

	// IO for sonar //
	DDRB = 0xFF; //output: PORTB7 - D13
	PORTB = 0x7F;

	//=== USART settings (for sonar) ===//
	usart_config ucon;
	ucon.number = 1;
	ucon.baudrate = 9600;
	ucon.databits = 8; //8bits
	ucon.paritymode = 0; //disabled
	ucon.stopbits = 1; //1 bit
	ucon.modesel = 0; //Asynchronous
	configure_usart(&ucon);
	
}


// command to take measurement
void measure(){
		PORTB |= 0x80;
		_delay_ms(25);
		PORTB &= 0x7F;
}
