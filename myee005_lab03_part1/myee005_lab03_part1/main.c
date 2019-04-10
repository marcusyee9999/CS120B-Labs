/*
 * myee005_lab03_part1.c
 *
 * Created: 4/8/2019 10:08:32 PM
 * Author : myyee
 *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 1
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00;
	DDRB = 0x00;
    DDRC = 0xFF;
    PORTA = 0xFF;
	PORTB = 0xFF;
    PORTC = 0x00;
	unsigned char tempInA;
	unsigned char tempInB;
	unsigned char cntVal;
	
    while (1) {
		cntVal = 0x00;
		tempInA = PINA & 0xFF;
		tempInB = PINB & 0xFF;
		
		for (unsigned int i = 0; i < 8; ++i) {
			if ((tempInA & 0x01) == 0x01) {
				++cntVal;
			}
			if ((tempInB & 0x01) == 0x01) {
				++cntVal;
			}
			tempInA = tempInA / 2;
			tempInB = tempInB / 2;
		}
		
		PORTC = cntVal;
		
    }
}

