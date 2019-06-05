/*
  *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
  *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
  *	Lab Section: 028
  *	Assignment: Lab #4  Exercise # 2
  *	I acknowledge all content contained herein, excluding template or example
  *	code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, init, wait, reset, release_1, increment, release_2, decrement, release_3}state;

void CntSM(){

	switch(state) {	//transitions
		case start:
			state = init;
			break;
		case init:
			state = wait;
			break;
		case wait:
			if (PINA == 0x03) {
				state = reset;
			}
			else if (PINA == 0x01) {
				state = increment;
			}
			else if (PINA == 0x02) {
				state = decrement;
			}
			else {
				state = wait;
			}
			break;
		case reset:
			if(PINA == 0x03){
				state = reset;
			}
			else {
				state = release_1;
			}
			break;
		case release_1:
			state = wait;
		break;
		
		case increment:
		if(PINA == 0x01){
			state = increment;
		}
		else {
			state = release_2;
		}
		break;
		case release_2:
			state = wait;
		break;
		
		case decrement:
		if(PINA == 0x02){
			state = decrement;
		}
		else {
			state = release_3;
		}
		break;
		case release_3:
		state = wait;
		break;
		
		default:
			state = init;
			break;
	}

	switch(state){	//actions
		case start:
			PORTC = 0x00;
			break;
		case init:
			PORTC = 0x07;
			break;
		case wait:
			if (PORTC >= 9) {
				PORTC = 0x09;
			}
			else if (PORTC <= 0) {
				PORTC = 0x00;
			}
			else {
				PORTC = PORTC;
			}
			break;
		case reset:
			PORTC = PORTC;
			break;
		case release_1:
			PORTC = 0x00;
			break;
		case increment:
			PORTC = PORTC;
			break;
		case release_2:
			PORTC = PORTC + 1;
			break;
		case decrement:
			PORTC = PORTC;
			break;
		case release_3:
			PORTC = PORTC - 1;
			break;
		default:
			PORTC = 0x00;
			break;
		
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    while (1) 
    {
		CntSM();
    }
}

