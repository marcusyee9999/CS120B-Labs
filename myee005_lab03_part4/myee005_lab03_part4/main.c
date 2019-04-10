/*
 * myee005_lab03_part4.c
 *
 * Created: 4/9/2019 11:09:19 AM
 * Author : myyee
 *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 4
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

int main(void)
{
    	DDRA = 0x00;
		DDRB = 0xFF;
    	DDRC = 0xFF;
    	PORTA = 0xFF;
		PORTB = 0x00;
    	PORTC = 0x00;
		unsigned char tempA;
		unsigned char tempB;
		unsigned char tempC;

    while (1) {
		tempA = PINA;
		tempB = 0x00;
		tempC = 0x00;

		for (unsigned int i = 4; i < 8; i++ ) {
			if ((GetBit(tempA, i - 4))) {
				tempC = SetBit(tempC, i, 1);
			}
			else {
				tempC = SetBit(tempC, i, 0);
			}
		}
		
		for (unsigned int j = 0; j < 4; j++) {
			if (GetBit(tempA, j + 4)) {
				tempB = SetBit(tempB, j, 1);
			}
			else {
				tempB = SetBit(tempB, j, 0);
			}
		}
	
		PORTB = tempB;
		PORTC = tempC;

	}

}