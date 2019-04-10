/*
 * myee005_lab02_part3.c
 *
 * Created: 4/4/2019 7:31:02 PM
 * Author : myyee
 */ 

#include <avr/io.h>

int main(void){
	DDRA = 0x00;
	DDRC = 0xFF;
	PORTA = 0xFF;
	PORTC = 0x00;
	unsigned char tempA = 0x00;
	unsigned char cntavail = 0x00;


	while(1){
		cntavail = 0x00;
		tempA = PINA & 0x0F;
		
		for (unsigned int i = 0; i < 4; ++i) {
			if ((tempA & 0x01) == 0x00) {
				++cntavail;
			}
			tempA = tempA / 2;
		}
		if (cntavail == 0x00) {
			cntavail = cntavail + 0x80;
		}
		PORTC = cntavail;
	}

	return 0;
}

