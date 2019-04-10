/*
 * myee005_lab02_part1.c
 *
 * Created: 4/4/2019 5:30:54 PM
 * Author : myyee
 */ 

#include <avr/io.h>
int main(void){
	DDRA = 0x00; 
	PORTA = 0xFF;
	DDRB = 0xFF; 
	PORTB = 0x00;
	unsigned char PA;
	unsigned char PB;

	while(1){
		
		PA = PINA & 0x03; //0000 0011
		
		if (PA == 0x01) {
			PB = 0x01;
		}
		
		else {
			PB = 0x00;
		}
		
		PORTB = PB;
	}

	return 0;
}

