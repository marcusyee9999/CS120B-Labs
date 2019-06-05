/*
 * myee005_proj_ledmatrix_test.c
 *
 * Created: 5/30/2019 2:38:22 PM
 * Author : myyee
 */ 

#include <avr/io.h>
#include "timer.h"

void transmit_data_C(unsigned char data) {
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTC |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
	
	//reset for new
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x00);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTC |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

void transmit_data_D(unsigned char data) {
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x08;
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTD |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}

enum SM_States{sm_display};
int LED_SM(int state) {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
//	unsigned char knife_col[8] = {0x00, 0x00, 0x00, 0xFF, 0x1E, 0x1C, 0x00, 0x00};
	unsigned char cook_col[8] = {0x00, 0xE1, 0x21, 0x3F, 0x00, 0x22, 0x77, 0xFF};
//	unsigned char fish_col[8] = {0x00, 0x31, 0x4B, 0xAD, 0x85, 0x4B, 0x31, 0x00};
//	unsigned char steak_col[8] = {0x00, 0x70, 0x78, 0xEE, 0xE7, 0x78, 0x30, 0x00};
	
	switch (state) {
		case sm_display:
			state = sm_display;
			break;
		default:
			break;
	}
	
	switch (state) {
		case sm_display:
			for (int j = 0; j < 8; ++j) {
				transmit_data_D(overall_row[j]);
				transmit_data_C(cook_col[j]);
			}
			break;
		default:
			break;
	}
	
	return state;
}; 

enum matrixTest_states {sm1_display} state;	// make LED matrix light each one up individually
int Matrix_test(int state){
	static unsigned char column_val = 0x01;
	static unsigned char column_sel = 0x7F;
	
	switch (state) {
		case sm1_display:
		break;
		default:
		state = sm1_display;
		break;
	}
	switch (state) {
		case sm1_display:
		if (column_sel == 0xFE && column_val == 0x80) {
			column_sel = 0x7F;
			column_val = 0x01;
		}
		else if ( column_sel == 0xFE) {
			column_sel = 0x7F;
			column_val = column_val << 1;
		}
		else {
			column_sel = (column_sel >> 1) | 0x80;
		}
		break;
		default:
		break;
	}
	transmit_data_C(column_val);
	transmit_data_D(column_sel);
	return state;
}

void fish_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char fish_col[8] = {0x00, 0x31, 0x4B, 0xAD, 0x85, 0x4B, 0x31, 0x00};
	
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(fish_col[j]);
	}
}

int main(void)
{
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    
	TimerSet(5);
	TimerOn();
//	transmit_data_C(0x01);
//	transmit_data_D(0x01);
	
	while (1) {
		//Matrix_test(state);
		//LED_SM(state);
		LED_SM(state);
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

