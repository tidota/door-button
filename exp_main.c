#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "usart.h"
#include "stagectrl.h"
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
uint8_t rx_snr(char *buff){
	int i = 0, j = 0;
	while(i < MAX_RX_SNR){
		buff[i] = usart1rx();
		i++;
	}
	if(buff[0] != 'R' || buff[5] != 0x0D)
		return 0; //failed
	buff[0] = '0';
	buff[5] = 0x00;
	// check each character and count the 0's before data
	i = 0;
	while(i < MAX_RX_SNR - 1){
		if(buff[i] != '0')
			break;
		if(buff[i] < '0' || '9' < buff[i])
			return 0; //failed
		i++;
	}
	// shift data to left
	j = 0;
	while(i + j < MAX_RX_SNR - 1){
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
void tx_usb(const char *data){
	int i =0;
	while(data[i] != 0x00){
		printf("%c",data[i]);
		i++;
	}
	printf("%c\n",0x03);
}
void rx_usb(char *com){ //* if there is no command, the function will wait forever.
	int i =0;
	char buff =0x03;
	scanf("%c",&buff);
	while(buff != 0x03){
		com[i] = buff;
		scanf("%c",&buff);
		i++;
	}
	com[i] = 0x00;
}

// === format of command from PC ===
// measurement: "p<pitch>y<yaw>"  e.g., "p0800y0900" for 80.0 deg of pitch and 90.0 deg of yaw
// initialize:  "I"
//
// the ranges:
//   pitch: 0-90
//   yaw: 0-360

// === format of data to PC ===
// data: <pitch><tab><yaw><tab><4 digits of number>
// OK: "OK"
// error: "E-"<message>
// *OK is to be used to respond for the initialization command "I"

// function to create error message
//   body (main string to store the result)
//   append (string to add to body)
//   strt (position that append starts)
//   lim (limit length of body)
//   example: char body[32]="error:",char append[8]="message",strt=6,lim=32  => body="error:message"
void mkerr(char* body, char* append, int strt, int lim){
	int i = 0;
	while(strt+i < lim-1){
		body[strt+i] = append[i];
		if(append[i] == '\0')
			break;
		i++;
	}
	body[strt+i] = '\0';
}

// === conversions of angles for servos ===
// pitch,yaw => tilting servo (0 to 180)
#define Tlt(p,y) (((y) <= 180)? (p): (180-(p)))
// pitch,yaw => panning servo (0 to 180)
#define Pan(p,y) (((y) <= 180)? (y): ((y) - 180))


int main(void){	

	// variables for receiving data from sonar
	char r_snr[30]; // buffer of string

	// variables for communication with PC
	char r_usb[30]; // string from PC
	char t_usb[30]; // string to PC
	double pitch, yaw;
	char errinv[32] = "E-Invalid Command:"; //error message (18 characters)
	const int lerrinv = 18;
	char errrng[32] = "E-Out of Range:";    //error message (15 characters)
	const int lerrrng = 15;

	double tlt, pan, tlt_old, pan_old;

	// === USB settings (for com. with PC) === //
	init_usart();

	// === sonar settings === //
	initSonar();

	// === PWM settings === //
	initStage();

	// default position //
	tlt = 0; pan = 0;
	setDegTlt(tlt);
	setDegPan(pan);
	tlt_old = tlt;
	pan_old = pan;
	_delay_ms(2000);

	//=== Main Loop ===//
	while(1){
		// reset the buffer
		r_usb[0] = 0; r_usb[4] = 0;
		// wait for the command from PC //
		rx_usb(r_usb);

		// check the command
		if(r_usb[0] == 'I'){ //initialization command
			// set the servos at the initial positions
			setDegTlt(30); setDegPan(30);
			_delay_ms(2000);
			setDegTlt(0); setDegPan(0);
			_delay_ms(500);
			tlt_old = 0; pan_old = 0;
			// response to PC
			tx_usb("OK");
			continue;
		}
		if(r_usb[0] == 'p'){
			pitch = (r_usb[1] - '0')*100;
			pitch += (r_usb[2] - '0')*10;
			pitch += (r_usb[3] - '0');
			pitch += (r_usb[4] - '0')/10.0;
		}else{
			mkerr(errinv,r_usb,lerrinv,32);
			tx_usb(errinv);
			continue;
		}
		if(r_usb[5] == 'y'){
			yaw = (r_usb[6] - '0')*100;
			yaw += (r_usb[7] - '0')*10;
			yaw += (r_usb[8] - '0');
			yaw += (r_usb[9] - '0')/10.0;
		}else{
			mkerr(errinv,r_usb,lerrinv,32);
			tx_usb(errinv);
			continue;
		}
		if(pitch < 0 || 90 < pitch || yaw < 0 || 360 < yaw){
			mkerr(errrng,r_usb,lerrrng,32);
			tx_usb(errrng);
			continue;
		}

		// move servos //
		tlt = Tlt(pitch,yaw); pan = Pan(pitch,yaw); // get tlt and pan angles
		setDegTlt(tlt); setDegPan(pan);             // control servos
		if(abs(tlt - tlt_old) > abs(pan - pan_old)) // wait for the time corresponding the angle they moved
			delay_ms((int)(abs(tlt - tlt_old)*10));
		else
			delay_ms((int)(abs(pan - pan_old)*10));
		tlt_old = tlt; pan_old = pan;               // update the variables

		// command the sonar to measure //
		measure();

		// receive data
		if(rx_snr(r_snr) == 0)
			continue; // restart the loop if the return value is 0
	
		// send the result to PC
		sprintf(t_usb,"%d.%d\t%d.%d\t%s",(int)pitch,(int)(pitch*10.0)%10,(int)yaw,(int)(yaw*10.0)%10,r_snr);
		tx_usb(t_usb);
	}

	return 1;
} //main//


