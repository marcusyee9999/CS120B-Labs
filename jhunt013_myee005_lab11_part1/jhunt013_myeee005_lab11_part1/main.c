/*
 * jhunt013_myeee005_lab11_part1.c
 *
 * Created: 5/15/2019 7:41:05 PM
 * Author : myyee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
#include <stdio.h>
#include "bit.h"
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"

enum SM1_States{SM1_output}SM1_State;
unsigned char tempB = 0x00;

int SMTick1(int state){
	unsigned char x;
	x = GetKeypadKey();
	switch(state){
		case SM1_output:
		switch (x) {
			case '\0':
			tempB = 0x1F;
			break; // All 5 LEDs on
			case '1':
			tempB = 0x01;
			break; // hex equivalent
			case '2':
			tempB = 0x02;
			break;
			case '3':
			tempB = 0x03;
			break;
			case '4':
			tempB = 0x04;
			break;
			case '5':
			tempB = 0x05;
			break;
			case '6':
			tempB = 0x06;
			break;
			case '7':
			tempB = 0x07;
			break;
			case '8':
			tempB = 0x08;
			break;
			case '9':
			tempB = 0x09;
			break;
			case 'A':
			tempB = 0x0A;
			break;
			case 'B':
			tempB = 0x0B;
			break;
			case 'C':
			tempB = 0x0C;
			break;
			case 'D':
			tempB = 0x0D;
			break;
			case '*':
			tempB = 0x0E;
			break;
			case '0':
			tempB = 0x00;
			break;
			case '#':
			tempB = 0x0F;
			break;
			default:
			tempB = 0x1B;
			break; // Should never occur. Middle LED off.
		}
		state = SM1_output;
		PORTB=tempB;
		break;
	}
	return state;
}


int main()
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;

	unsigned long int SMTick1_calc = 20;
	unsigned long int tempGCD = 10;

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;

	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.


	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}
