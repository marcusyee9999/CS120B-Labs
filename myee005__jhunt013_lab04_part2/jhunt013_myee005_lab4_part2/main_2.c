/*
 * jhunt013_myee005_lab4_part2.c
 *
 * Created: 4/10/2019 8:28:54 PM
 * Author : jenny
 */ 

#include <avr/io.h>

enum States{init, wait, reset, release_1, increment, release_2, decrement, release_3}state;

void CntSM(){

	switch(state) {	//transitions
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
			state == decrement;
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
		if (PINA != 0x03) {
			state = wait;
		}
		else {
			state = release_1;
		}
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
		if (PINA != 0x01) {
			state = wait;
		}
		else {
			state = release_2;
		}
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
		if (PINA != 0x02) {
			state = wait;
		}
		else {
			state = release_3;
		}
		break;
		
		default:
			state = init;
			break;
	}

	switch(state){	//actions
		case init:
			PORTC = 0x07;
			break;
		case wait:
			PORTC = PORTC;
			break;
		case reset:
			PORTC = 0x00;
			break;
		case release_1:
			PORTC = PORTC;
			break;
		case increment:
			if (PORTC < 9) {
				++PORTC;
			}
			else {
				PORTC = PORTC;
			}
			break;
		case release_2:
			PORTC = PORTC;
			break;
		case decrement:
			if (PORTC > 0) {
				--PORTC;
			}
			else {
				PORTC = PORTC;
			}
			break;
		case release_3:
			PORTC = PORTC;
			break;
		default:
			PORTC = 0x07;
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

