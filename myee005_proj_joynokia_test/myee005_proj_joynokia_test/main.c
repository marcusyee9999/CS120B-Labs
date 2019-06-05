/*
 * myee005_proj_joynokia_test.c
 *
 * Created: 6/2/2019 4:35:27 PM
 * Author : myyee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8250000UL
#include "nokia5110.h"
#include "nokia5110.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned int player_posx = 40;
unsigned int player_posy = 21;
unsigned char player_score = 0x00;
unsigned char temp_score = 0x00;
unsigned int movement = 2;
unsigned int fast_movement = 4;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

//LED Matrix
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

void LED_SM() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	//	unsigned char knife_col[8] = {0x00, 0x00, 0x00, 0xFF, 0x1E, 0x1C, 0x00, 0x00};
	//	unsigned char cook_col[8] = {0x00, 0xE1, 0x21, 0x3F, 0x00, 0x22, 0x77, 0xFF};
	unsigned char fish_col[8] = {0x00, 0x31, 0x4B, 0xAD, 0x85, 0x4B, 0x31, 0x00};
	//unsigned char steak_col[8] = {0x00, 0x70, 0x78, 0xEE, 0xE7, 0x78, 0x30, 0x00};
		
		for (int j = 0; j < 8; ++j) {
			transmit_data_D(overall_row[j]);
			transmit_data_C(fish_col[j]);
		}
}
/*
enum SM_States{sm_display};
int LED_SM(int state) {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	//	unsigned char knife_col[8] = {0x00, 0x00, 0x00, 0xFF, 0x1E, 0x1C, 0x00, 0x00};
	//	unsigned char cook_col[8] = {0x00, 0xE1, 0x21, 0x3F, 0x00, 0x22, 0x77, 0xFF};
		unsigned char fish_col[8] = {0x00, 0x31, 0x4B, 0xAD, 0x85, 0x4B, 0x31, 0x00};
	//unsigned char steak_col[8] = {0x00, 0x70, 0x78, 0xEE, 0xE7, 0x78, 0x30, 0x00};
	
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
			transmit_data_C(fish_col[j]);
		}
		break;
		default:
		break;
	}
	
	return state;
};
*/

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


//joystick

unsigned short x, y;

void InitADC() {
	ADMUX=(1<<REFS0);                   //setting the reference of ADC such THAT Areff=Avcc
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
//used something new datatype her to check if it works or not, unsigned integer short can aolso be written as unint16_t

uint16_t ReadADC(uint8_t ch)
{	InitADC();
	ch=ch&0b00000111; //using this channel to be a extra safety net for the correct reading
	ADMUX|=ch; // read from the first three channels
	//conversion steps
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF); // got from one of the labs
	return(ADC);
}
//code for reading which direction is the joystick pointing to0
uint16_t find_direction_from_xy(uint16_t x,uint16_t y)
{
	//1-> up, 2-> right, 3-> down 4-> left
	if(x>255 && x<765 && y>765) // we know 1024 is the max value, these values are exact half of the max values which i had using the joystick
	return(2); // direction is up

	else if(x>765 && y>255 && y<765)
	return(3); //direction is right

	else if(x>255 && x<765 && y<255)
	return(4); //direction is down

	else if(x<255 && y>255 && y<765)
	return(1); // direction is left

	else {return (0);}
	//	return(0); //default state
}

enum Game_MapSM_States {map_init, start_map}Game_Map_state;
void Game_MapSM() {
	switch(Game_Map_state) {
		case map_init:
			Game_Map_state = start_map;
			break;
		case start_map:
			Game_Map_state = start_map;
			break;
		default:
			Game_Map_state = start_map;
			break;
	}
	
	switch(Game_Map_state) {
		case map_init:
			break;
		case start_map:
			nokia_lcd_set_cursor(1,1);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(1,8);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(72,1);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(72,8);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(1,34);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(1,41);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(72,34);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(72,41);
			nokia_lcd_write_string("xx", 1);
			nokia_lcd_set_cursor(31,41);
			nokia_lcd_write_string("____", 1);
			nokia_lcd_render();
			break;
		default:
			break;
	}
}

enum Player_MoveSM_States {init, wait, up, down, right, left}Player_Move_state;

void Player_MoveSM () {
	switch (Player_Move_state) {
		case init:
			Player_Move_state = wait;
			break;
		case wait:
			if (find_direction_from_xy(x,y) == 1) {
				Player_Move_state = up;
			}
			else if (find_direction_from_xy(x,y) == 3) {
				Player_Move_state = down;
			}
			else if (find_direction_from_xy(x,y) == 2) {
				Player_Move_state = right;
			}
			else if (find_direction_from_xy(x,y) == 4) {
				Player_Move_state = left;
			}
			else {
				Player_Move_state = wait;
			}
			break;
		case up:
			if (find_direction_from_xy(x,y) == 1) {
				Player_Move_state = up;
			}
			else {
				Player_Move_state = wait;
			}
			break;
			case right:
			if (find_direction_from_xy(x,y) == 2) {
				Player_Move_state = right;
			}
			else {
				Player_Move_state = wait;
			}
			break;
		case down:
			if (find_direction_from_xy(x,y) == 3) {
				Player_Move_state = down;
			}
			else {
				Player_Move_state = wait;
			}
			break;
		case left:
			if (find_direction_from_xy(x,y) == 4) {
				Player_Move_state = left;
			}
			else {
				Player_Move_state = wait;
			}
			break;
		default:
			Player_Move_state = init;
			break;
	}
	switch (Player_Move_state) {
		case init:
			break;
		case wait:
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
		case up:
			if ((player_posy >2 && player_posx >= 12 && player_posx <= 67) || (player_posy > 13 && (player_posx < 12 || player_posx > 67))) {
				player_posy = player_posy - movement;
			}
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
		case right:
			if ((player_posx < 78 && player_posy >= 12 && player_posy <= 30) || (player_posx < 66 && (player_posy < 12 || player_posy > 30))) {
				player_posx = player_posx + movement;
			}
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
		case down:
			if ((player_posy < 40 && player_posx >= 12 && player_posx <= 67) || (player_posy < 28 && (player_posx < 12 || player_posx > 67))) {
				player_posy = player_posy + movement;
			}
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
		case left:
			if ((player_posx > 2 && player_posy >= 12 && player_posy <= 30) || (player_posx > 13 && (player_posy < 12 || player_posy > 30)) ) {
				player_posx = player_posx - movement;
			}
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
		default:
			break;
	}
}

unsigned int sushi_points = 0;
unsigned int rice_points = 0;

enum Sushi_SM_States {sushi_init, sushi_wait, sushi_collect, rice_collect, sushi_serve, rice_serve, update_sushi_score}Sushi_state;
void Sushi_SM () {
	switch (Sushi_state) {
		case sushi_init:
			Sushi_state = sushi_wait;
			break;
		case sushi_wait:
			if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
				Sushi_state = sushi_collect;
			}
			else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
				Sushi_state = rice_collect;
			}
			else if (rice_points >= 1 && sushi_points >= 1) {
				Sushi_state = update_sushi_score;
			}
			else {
				Sushi_state = sushi_wait;
			}
			break;
		case sushi_collect:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				Sushi_state = sushi_serve;
			}
			else {
				Sushi_state = sushi_collect;
			}
			break;
		case rice_collect:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				Sushi_state = rice_serve;
			}
			else {
				Sushi_state = rice_collect;
			}
			break;
		case sushi_serve:
			Sushi_state = sushi_wait;
			break;
		case rice_serve:
			Sushi_state = sushi_wait;
			break;
		case update_sushi_score:
			rice_points = rice_points - 1;
			sushi_points = sushi_points - 1;
			Sushi_state = sushi_wait;
			break;
		default:
			Sushi_state = sushi_wait;
			break;
	}
	
	switch (Sushi_state) {
		case sushi_init:
			break;
		case sushi_wait:
			break;
		case sushi_collect:
			PORTD = SetBit(PORTD, 6, 1);
			break;
		case rice_collect:
			PORTD = SetBit(PORTD, 6, 1);
			break;
		case sushi_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 2;
			sushi_points += 1;
			break;
		case rice_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 1;
			rice_points += 1;
			break;
		case update_sushi_score:
			player_score += 3;
			break;
		default:
			break;
	}
}

unsigned int cooking_steak = 0;
unsigned int chopping_steak = 0;
unsigned int steak_points = 0;
unsigned int steak_cook_points = 0;
unsigned int steak_chop_points = 0;

enum Steak_SM_States {steak_init, steak_wait, steak_collect, steak_chop, steak_cook, steak_serve, steak_done, steak_complete, steak_cooked_serve, steak_chopped_serve, update_steak_score}Steak_state;
void Steak_SM() {
	switch (Steak_state) {
		case steak_init:
			Steak_state = steak_wait;
			break;
		case steak_wait:
			if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
				Steak_state = steak_collect;
			}
			else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
				Steak_state = steak_cook;
			}
			else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3)) {
				Steak_state = steak_chop;
			}
			else if (steak_points >= 1 && steak_chop_points >= 1 && steak_cook_points >= 1) {
				Steak_state = update_steak_score;
			}
			else {
				Steak_state = steak_wait;
			}
			break;
		case steak_collect:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				Steak_state = steak_serve;
			}
			else {
				Steak_state = steak_collect;
			}
			break;
		case steak_cook:
			if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && cooking_steak < 40) {
				Steak_state = steak_cook;
				cooking_steak += 1;
			}
			else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && cooking_steak >= 40) {
				PORTD = SetBit(PORTD, 5, 1);
				Steak_state = steak_done;
				cooking_steak = 0;
			}
			else {
				Steak_state = steak_cook;
			}
			break;
		case steak_chop:
			if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_steak < 20) {
				Steak_state = steak_chop;
				chopping_steak += 1;
			}
			else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_steak >= 20) {
				PORTD = SetBit(PORTD, 6, 1);
				Steak_state = steak_complete;
				chopping_steak = 0;
			}
			else {
				Steak_state = steak_chop;
			}
			break;
		case steak_serve:
			Steak_state = steak_wait;
			break;
		case steak_done:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				Steak_state = steak_cooked_serve;
			}
			else {
				Steak_state = steak_done;
			}
			break;
		case steak_complete:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				Steak_state = steak_chopped_serve;
			}
			else {
				Steak_state = steak_complete;
			}
			break;
		case steak_cooked_serve:
			Steak_state = steak_wait;
			break;
		case steak_chopped_serve:
			Steak_state = steak_wait;
			break;
		case update_steak_score:
			steak_points = steak_points - 1;
			steak_cook_points = steak_cook_points - 1;
			steak_chop_points = steak_chop_points - 1;
			Steak_state = steak_wait;
			break;
		default:
			break;
	}
	switch(Steak_state) {
		case steak_init:
			break;
		case steak_wait:
			break;
		case steak_collect:
			PORTD = SetBit(PORTD, 6, 1);
			break;
		case steak_cook:
			break;
		case steak_chop:
			break;
		case steak_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 1;
			steak_points += 1;
			break;
		case steak_done:
			break;
		case steak_complete:
			break;
		case steak_cooked_serve:
			PORTD = SetBit(PORTD, 5, 0);
			player_score += 3;
			steak_cook_points += 1;
			break;
		case steak_chopped_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 2;
			steak_chop_points += 1;
			break;
		case update_steak_score:
			player_score += 5;
			break;
		default:
			break;
	}
}

unsigned int ovening_pizza = 0;
unsigned int chopping_pizza = 0;
unsigned int pizza_points = 0;
unsigned int pizza_oven_points = 0;
unsigned int pizza_chop_points = 0;
unsigned int tomato_points = 0;

enum pizza_SM_States {pizza_init, pizza_wait, pizza_collect, pizza_chop, pizza_oven, tomato_collect, pizza_serve, tomato_serve, pizza_done, pizza_complete, pizza_ovened_serve, pizza_chopped_serve, update_pizza_score}pizza_state;
void pizza_SM() {
	switch (pizza_state) {
		case pizza_init:
			pizza_state = pizza_wait;
			break;
		case pizza_wait:
			if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
				pizza_state = pizza_collect;
			}
			else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
				pizza_state = pizza_oven;
			}
			else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3)) {
				pizza_state = pizza_chop;
			}
			else if (((player_posx == 12 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
				pizza_state = tomato_collect;
			}
			else if (pizza_points >= 1 && pizza_chop_points >= 1 && pizza_oven_points >= 1 && tomato_points >= 1) {
				pizza_state = update_pizza_score;
			}
			else { 
				pizza_state = pizza_wait;
			}
			break;
		case pizza_collect:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				pizza_state = pizza_serve;
			}
			else {
				pizza_state = pizza_collect;
			}
			break;
		case pizza_oven:
			if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && ovening_pizza < 40) {
				pizza_state = pizza_oven;
				ovening_pizza += 1;
			}
			else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && ovening_pizza >= 40) {
				PORTD = SetBit(PORTD, 5, 1);
				pizza_state = pizza_done;
				ovening_pizza = 0; 
			}
			else {
				pizza_state = pizza_oven;
			}
			break;
		case pizza_chop:
			if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_pizza < 20) {
				pizza_state = pizza_chop;
				chopping_pizza += 1;
			}
			else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_pizza >= 20) {
				PORTD = SetBit(PORTD, 6, 1);
				pizza_state = pizza_complete;
				chopping_pizza = 0;
			}
			else {
				pizza_state = pizza_chop;
			}
			break;
		case tomato_collect:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				pizza_state = tomato_serve;
			}
			else {
				pizza_state = tomato_collect;
			}
			break;
		case pizza_serve:
			pizza_state = pizza_wait;
			break;
		case tomato_serve:
			pizza_state = pizza_wait;
			break;
		case pizza_done:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				pizza_state = pizza_ovened_serve;
			}
			else {
				pizza_state = pizza_done;
			}
			break;
		case pizza_complete:
			if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
				pizza_state = pizza_chopped_serve;
			}
			else {
				pizza_state = pizza_complete;
			}
			break;
		case pizza_ovened_serve:
			pizza_state = pizza_wait;
			break;
		case pizza_chopped_serve:
			pizza_state = pizza_wait;
			break;
		case update_pizza_score:
			pizza_points = pizza_points - 1;
			pizza_oven_points = pizza_oven_points - 1;
			pizza_chop_points = pizza_chop_points - 1;
			tomato_points = tomato_points - 1;
			pizza_state = pizza_wait;
			break;
		default:
			break;
	}
	switch(pizza_state) {
		case pizza_init:
			break;
		case pizza_wait:
			break;
		case pizza_collect:
			PORTD = SetBit(PORTD, 6, 1);
			break;
		case pizza_oven:
			break;
		case pizza_chop:
			break;
		case tomato_collect:
			PORTD = SetBit(PORTD, 6, 1);
			break;
		case pizza_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 1;
			pizza_points += 1;
			break;
		case tomato_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 1;
			tomato_points += 1;
			break;
		case pizza_done:
			break;
		case pizza_complete:
			break;
		case pizza_ovened_serve:
			PORTD = SetBit(PORTD, 5, 0);
			player_score += 3;
			pizza_oven_points += 1;
			break;
		case pizza_chopped_serve:
			PORTD = SetBit(PORTD, 6, 0);
			player_score += 2;
			pizza_chop_points += 1;
			break;
		case update_pizza_score:
			player_score += 5;
			break;
		default:
			break;
	}
}

enum Player_ScoreSM_States {score_display}Score_state;
void Player_scoreSM() {
	switch (Score_state) {
		case score_display:
			if (player_score >= 10) {
				temp_score = temp_score + (player_score / 10);
			}
			player_score = player_score % 10;
			nokia_lcd_set_cursor(19, 1);
			nokia_lcd_write_string("score:", 1);
			nokia_lcd_write_char(temp_score + '0', 1);
			nokia_lcd_write_char(player_score + '0', 1);
			break;
		default:
			break;
	}
}

void move_fast() {
	if (GetBit(~PINA, 2)) {
		movement = fast_movement;
	}
	else {
		movement = 2;
	}
}

unsigned int game_time = 0;

void Timer_LED() {
	if (game_time > 0 && game_time < 200) {
		PORTA = SetBit(PORTA, 4, 1);
		PORTA = SetBit(PORTA, 5, 1);
		PORTA = SetBit(PORTA, 6, 1);
	}
	else if (game_time >= 200 && game_time < 400) {
		PORTA = SetBit(PORTA, 4, 1);
		PORTA = SetBit(PORTA, 5, 1);
		PORTA = SetBit(PORTA, 6, 0);
	}
	else if (game_time >= 400 && game_time < 600) {
		PORTA = SetBit(PORTA, 4, 1);
		PORTA = SetBit(PORTA, 5, 0);
		PORTA = SetBit(PORTA, 6, 0);
	}
	else {
		PORTA = SetBit(PORTA, 4, 0);
		PORTA = SetBit(PORTA, 5, 0);
		PORTA = SetBit(PORTA, 6, 0);
	}
}

enum FinalGame_SM_States {start, game_menu, sushi_game, steak_game, pizza_game, game_score}FinalGame_state;
void FinalGame_SM() {
	switch (FinalGame_state) {
		case start:
			FinalGame_state = game_menu;
			break;
		case game_menu:
			nokia_lcd_clear();
			nokia_lcd_set_cursor(20, 1);
			nokia_lcd_write_string("Menu", 2);
			nokia_lcd_set_cursor(20, 20);
			nokia_lcd_write_string("^ sushi", 1);
			nokia_lcd_set_cursor(20, 30);
			nokia_lcd_write_string("> steak", 1);
			nokia_lcd_set_cursor(20, 40);
			nokia_lcd_write_string("< pizza", 1);
			nokia_lcd_render();
			if (find_direction_from_xy(x,y) == 1) {
				FinalGame_state = sushi_game;
			}
			else if (find_direction_from_xy(x,y) == 2) {
				FinalGame_state = steak_game;
			}
			else if (find_direction_from_xy(x,y) == 4) {
				FinalGame_state = pizza_game;
			}
			else {
				FinalGame_state = game_menu;
			}
			break;
		case sushi_game:
			Timer_LED();
			if (game_time < 600) {
				nokia_lcd_clear();
				Game_MapSM();
				Player_scoreSM();
				Player_MoveSM();
				Sushi_SM();
				nokia_lcd_render();
			}
			else if (game_time >= 600) {
				game_time = 0;
				FinalGame_state = game_score;
			}
			break;
		case steak_game:
			Timer_LED();
			if (game_time < 600) {
				nokia_lcd_clear();
				Game_MapSM();
				Player_scoreSM();
				Player_MoveSM();
				Steak_SM();
				nokia_lcd_render();
			}
			else if (game_time >= 600) {
				game_time = 0;
				FinalGame_state = game_score;
			}
			break;
		case pizza_game:
			Timer_LED();
			if (game_time < 600) {
				nokia_lcd_clear();
				Game_MapSM();
				Player_scoreSM();
				Player_MoveSM();
				pizza_SM();
				nokia_lcd_render();
			}
			else if (game_time >= 600) {
				game_time = 0;
				FinalGame_state = game_score;
			}
			break;
		case game_score:
			if (game_time < 100) {
				nokia_lcd_clear();
				nokia_lcd_set_cursor(12, 10);
				nokia_lcd_write_string("Score:", 2);
				nokia_lcd_set_cursor(33, 25);
				nokia_lcd_write_char(temp_score + '0', 2);
				nokia_lcd_write_char(player_score + '0', 2);
				nokia_lcd_render();
			}
			else if (game_time >= 100) {
				game_time = 0;
				FinalGame_state = game_menu;
			}
			break;
		default:
			break;
	}
	switch (FinalGame_state) {
		case start:
			break;
		case game_menu:
			break;
		case sushi_game:
			game_time++;
			break;
		case steak_game:
			game_time++;
			break;
		case pizza_game:
			game_time++;
			break;
		case game_score:
			game_time++;
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xF0; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	const unsigned long period = 50;
	
	InitADC();
	
	TimerSet(period);
	TimerOn();
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
	
	while (1) {
		x = ReadADC(0);
		y = ReadADC(1);
		
		FinalGame_SM();
		move_fast();
		
		while(!TimerFlag){}
		TimerFlag = 0;

	}
}

