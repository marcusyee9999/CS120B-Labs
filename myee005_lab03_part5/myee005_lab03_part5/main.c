/*
 * myee005_lab03_part5.c
 *
 * Created: 4/9/2019 2:42:10 PM
 * Author : myyee
 *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu 
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 5
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
    DDRD = 0x00;
	DDRB = 0x0E;
    PORTD = 0xFF;
    PORTB = 0x01;
	unsigned char tempD;
	unsigned char tempB;
	unsigned char out_B;

    while (1) {
		tempB = PINB;
		tempD = PIND;
		out_B = 0x00;
		tempD = tempD * 2;
		tempD = SetBit(tempD, 0, GetBit(tempB, 0));
		
		if (tempD <= 69 && tempD >= 5) {
			out_B = SetBit(out_B, 2, 1);
		}
		else if (tempD >= 70) {
			out_B = SetBit(out_B, 1, 1);
		}
		
		PORTB = out_B;
    }
}

