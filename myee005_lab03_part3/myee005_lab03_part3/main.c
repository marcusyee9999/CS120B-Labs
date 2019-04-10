/*
 * myee005_lab03_part3.c
 *
 * Created: 4/8/2019 11:14:56 PM
 * Author : myyee
 *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 3
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
    DDRC = 0xFF;
    PORTA = 0xFF;
    PORTC = 0x00;
    unsigned char tempA;
    unsigned char tmpVal = 0x00;
    
    while (1) {
	    tempA = PINA & 0x0F;
	    
	    if (tempA == 0x01 || tempA == 0x02) {
		    tmpVal = 0x60;
	    }
	    if (tempA == 0x03 || tempA == 0x04) {
		    tmpVal = 0x70;
	    }
	    if (tempA == 0x05 || tempA == 0x06) {
		    tmpVal = 0x38;
	    }
	    if (tempA == 0x07 || tempA == 0x08 || tempA == 0x09) {
		    tmpVal = 0x3C;
	    }
	    if (tempA == 0x0A || tempA == 0x0B || tempA == 0x0C) {
		    tmpVal = 0x3E;
	    }
	    if (tempA == 0x0D || tempA == 0x0E || tempA == 0x0F) {
		    tmpVal = 0x3F;
	    }

		tmpVal = tmpVal & 0x7F; 

		if (GetBit(PINA, 4) && GetBit(PINA, 5) && !GetBit(PINA,6)) {
			tmpVal = SetBit(tmpVal, 7, 1);
		}
		else {
			tmpVal = SetBit(tmpVal, 7, 0);
		}
	    
	    PORTC = tmpVal;
    }
return 0;
}

