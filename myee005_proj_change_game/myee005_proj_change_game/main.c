/*
 * myee005_proj_change_game.c
 *
 * Created: 6/4/2019 11:01:31 PM
 * Author : myyee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8250000UL
#include "nokia5110.h"
#include "nokia5110.c"
#include <stdio.h>
#include "timer.h"
#include "bit.h"

//Global Variables
unsigned int init_player_posx = 40;
unsigned int init_player_posy = 21;
unsigned int player_posx = 40;
unsigned int player_posy = 21;
unsigned char player_score = 0x00;
unsigned char temp_score = 0x00;
unsigned int movement = 2;
unsigned int fast_movement = 4;
unsigned int receipe_game = 0;

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//LED Matrix
void transmit_data_C(unsigned char data) {
	int k;
	for (k = 0; k < 8 ; ++k) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> k) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTC |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
	/*
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
	PORTC = 0x00; */
}

void transmit_data_D(unsigned char data) {
	int k;
	for (k = 0; k < 8 ; ++k) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		//PORTD = 0x08;
		PORTD = SetBit(PORTD,3,1);
		PORTD = SetBit(PORTD, 0, 0);
		PORTD = SetBit(PORTD, 1, 0);
		PORTD = SetBit(PORTD, 2, 0);
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> k) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTD |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}

void fish_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char fish_col[8] = {0x00, 0x31, 0x4B, 0xAD, 0x85, 0x4B, 0x31, 0x00};
	
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(fish_col[j]);
	}
}

void steak_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char steak_col[8] = {0x00, 0x70, 0x78, 0xEE, 0xE7, 0x78, 0x30, 0x00};
	
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(steak_col[j]);
	}
}

void cook_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char cook_col[8] = {0x00, 0xE1, 0x21, 0x3F, 0x00, 0x22, 0x77, 0xFF};
	
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(cook_col[j]);
	}
}

void knife_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char knife_col[8] = {0x00, 0x00, 0x00, 0xFF, 0x1E, 0x1C, 0x00, 0x00};
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(knife_col[j]);
	}
}

void clear_led() {
	unsigned char overall_row[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
	unsigned char clear_col[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	for (int j = 0; j < 8; ++j) {
		transmit_data_D(overall_row[j]);
		transmit_data_C(clear_col[j]);
	}
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

void Game_Map () {
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
}

enum Player_MoveSM_States {init, wait, up, down, right, left};
	int Player_MoveSM (int state) {
		switch (state) {
			case init:
				if (receipe_game == 1 || receipe_game == 2 || receipe_game == 3) {
					state = wait;
				}
				else {
					state = init;
				}
				break;
			case wait:
				if (find_direction_from_xy(x,y) == 1) {
					state = up;
				}
				else if (find_direction_from_xy(x,y) == 3) {
					state = down;
				}
				else if (find_direction_from_xy(x,y) == 2) {
					state = right;
				}
				else if (find_direction_from_xy(x,y) == 4) {
					state = left;
				}
				else if (receipe_game == 0) {
					state = init;
				}
				else {
					state = wait;
				}
				break;
			case up:
				if (find_direction_from_xy(x,y) == 1) {
					state = up;
				}
				else if (receipe_game == 0) {
					state = init;
				}
				else {
					state = wait;
				}
				break;
				case right:
				if (find_direction_from_xy(x,y) == 2) {
					state = right;
				}
				else if (receipe_game == 0) {
					state = init;
				}
				else {
					state = wait;
				}
				break;
			case down:
				if (find_direction_from_xy(x,y) == 3) {
					state = down;
				}
				else if (receipe_game == 0) {
					state = init;
				}
				else {
					state = wait;
				}
				break;
			case left:
				if (find_direction_from_xy(x,y) == 4) {
					state = left;
				}
				else if (receipe_game == 0) {
					state = init;
				}
				else {
					state = wait;
				}
				break;
			default:
				state = init;
				break;
		}
		switch (state) {
			case init:
				break;
			case wait:
				nokia_lcd_clear();
				Game_Map();
				Player_scored();
				nokia_lcd_set_cursor(player_posx, player_posy);
				nokia_lcd_write_string("o", 1);
				nokia_lcd_render();
				break;
			case up:
				if ((player_posy >2 && player_posx >= 12 && player_posx <= 67) || (player_posy > 13 && (player_posx < 12 || player_posx > 67))) {
					player_posy = player_posy - movement;
				}
				nokia_lcd_clear();
				Game_Map();
				Player_scored();
				nokia_lcd_set_cursor(player_posx, player_posy);
				nokia_lcd_write_string("o", 1);
				nokia_lcd_render();
				break;
			case right:
				if ((player_posx < 78 && player_posy >= 12 && player_posy <= 30) || (player_posx < 66 && (player_posy < 12 || player_posy > 30))) {
					player_posx = player_posx + movement;
				}
				nokia_lcd_clear();
				Game_Map();
				Player_scored();
				nokia_lcd_set_cursor(player_posx, player_posy);
				nokia_lcd_write_string("o", 1);
				nokia_lcd_render();
				break;
			case down:
				if ((player_posy < 40 && player_posx >= 12 && player_posx <= 67) || (player_posy < 28 && (player_posx < 12 || player_posx > 67))) {
					player_posy = player_posy + movement;
				}
				nokia_lcd_clear();
				Game_Map();
				Player_scored();
				nokia_lcd_set_cursor(player_posx, player_posy);
				nokia_lcd_write_string("o", 1);
				nokia_lcd_render();
				break;
			case left:
				if ((player_posx > 2 && player_posy >= 12 && player_posy <= 30) || (player_posx > 13 && (player_posy < 12 || player_posy > 30)) ) {
					player_posx = player_posx - movement;
				}
				nokia_lcd_clear();
				Game_Map();
				Player_scored();
				nokia_lcd_set_cursor(player_posx, player_posy);
				nokia_lcd_write_string("o", 1);
				nokia_lcd_render();
				break;
			default:
				break;
		}
		return state;
	};
	
//Sushi Game
unsigned int sushi_points = 0;
unsigned int rice_points = 0;
unsigned int fish_light = 0;

enum Sushi_SM_States {sushi_init, sushi_wait, sushi_collect, rice_collect, sushi_serve, rice_serve, update_sushi_score};
	int Sushi_SM (int state) {
		switch(state) {
			case sushi_init:
				if (receipe_game == 1) {
					state = sushi_wait;
					nokia_lcd_clear();
					Game_Map();
					Player_scored();
					nokia_lcd_render();
				}
				else {
					state = sushi_init;
				}
				break;
			case sushi_wait:
				if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8) || (player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
					state = sushi_collect;
				}
				else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82) || (player_posx == 12 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
					state = rice_collect;
				}
				else if (rice_points >= 1 && sushi_points >= 1) {
					state = update_sushi_score;
				}
				else if (receipe_game == 0) {
					state = sushi_init;
				}
				else {
					state = sushi_wait;
				}
				break;
			case sushi_collect:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = sushi_serve;
				}
				else if (receipe_game == 0) {
					state = sushi_init;
				}
				else {
					state = sushi_collect;
				}
				break;
			case rice_collect:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = rice_serve;
				}
				else if (receipe_game == 0) {
					state = sushi_init;
				}
				else {
					state = rice_collect;
				}
				break;
			case sushi_serve:
				if (receipe_game == 0) {
					state = sushi_init;
				}
				else {
					state = sushi_wait;
				}
				break;
			case rice_serve:
				if (receipe_game == 0) {
					state = sushi_init;
				}
				else {
					state = sushi_wait;
				}
				break;
			case update_sushi_score:
				rice_points = rice_points - 1;
				sushi_points = sushi_points - 1;
				state = sushi_wait;
				break;
			default:
				state = sushi_wait;
				break;

		}
		switch (state) {
			case sushi_init:
				break;
			case sushi_wait:
				break;
			case sushi_collect:
				fish_led();
				break;
			case rice_collect:
				fish_light = 1;
				break;
			case sushi_serve:
				clear_led();
				player_score += 2;
				sushi_points += 1;
				break;
			case rice_serve:
				
				player_score += 1;
				rice_points += 1;
				break;
			case update_sushi_score:
				player_score += 3;
				break;
			default:
				break;
		}
		return state;
	};
	
//Steak Game
unsigned int cooking_steak = 0;
unsigned int chopping_steak = 0;
unsigned int steak_points = 0;
unsigned int steak_cook_points = 0;
unsigned int steak_chop_points = 0;

enum Steak_SM_States {steak_init, steak_wait, steak_collect, steak_chop, steak_cook, steak_serve, steak_done, steak_complete, steak_cooked_serve, steak_chopped_serve, update_steak_score};
	int Steak_SM(int state) {
		switch (state) {
			case steak_init:
				if (receipe_game == 2) {
					state = steak_wait;
					nokia_lcd_clear();
					Game_Map();
					Player_scored();
					nokia_lcd_render();
				}
				else {
					state = steak_init;
				}
				break;
			case steak_wait:
				if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
					state = steak_collect;
				}
				else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
					state = steak_cook;
				}
				else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3)) {
					state = steak_chop;
				}
				else if (steak_points >= 1 && steak_chop_points >= 1 && steak_cook_points >= 1) {
					state = update_steak_score;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_wait;
				}
				break;
			case steak_collect:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = steak_serve;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_collect;
				}
				break;
			case steak_cook:
				if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && cooking_steak < 40) {
					state = steak_cook;
					cooking_steak += 1;
				}
				else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && cooking_steak >= 40) {
					
					state = steak_done;
					cooking_steak = 0;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_cook;
				}
				break;
			case steak_chop:
				if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_steak < 20) {
					state = steak_chop;
					chopping_steak += 1;
				}
				else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_steak >= 20) {
					
					state = steak_complete;
					chopping_steak = 0;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_chop;
				}
				break;
			case steak_serve:
				state = steak_wait;
				break;
			case steak_done:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = steak_cooked_serve;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_done;
				}
				break;
			case steak_complete:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = steak_chopped_serve;
				}
				else if (receipe_game == 0) {
					state = steak_init;
				}
				else {
					state = steak_complete;
				}
				break;
			case steak_cooked_serve:
				state = steak_wait;
				break;
			case steak_chopped_serve:
				state = steak_wait;
				break;
			case update_steak_score:
				steak_points = steak_points - 1;
				steak_cook_points = steak_cook_points - 1;
				steak_chop_points = steak_chop_points - 1;
				state = steak_wait;
				break;
			default:
				break;
		}
		switch (state) {
			case steak_init:
				break;
			case steak_wait:
				break;
			case steak_collect:
				
				break;
			case steak_cook:
				break;
			case steak_chop:
				break;
			case steak_serve:
				
				player_score += 1;
				steak_points += 1;
				break;
			case steak_done:
				break;
			case steak_complete:
				break;
			case steak_cooked_serve:
				
				player_score += 3;
				steak_cook_points += 1;
				break;
			case steak_chopped_serve:
				
				player_score += 2;
				steak_chop_points += 1;
				break;
			case update_steak_score:
				player_score += 5;
				break;
			default:
				break;
		}
		return state;
	};

//Pizza Game
unsigned int ovening_pizza = 0;
unsigned int chopping_pizza = 0;
unsigned int pizza_points = 0;
unsigned int pizza_oven_points = 0;
unsigned int pizza_chop_points = 0;
unsigned int tomato_points = 0;

enum pizza_SM_States {pizza_init, pizza_wait, pizza_collect, pizza_chop, pizza_oven, tomato_collect, pizza_serve, tomato_serve, pizza_done, pizza_complete, pizza_ovened_serve, pizza_chopped_serve, update_pizza_score};
	int pizza_SM (int state) {
		switch (state) {
			case pizza_init:
				if (receipe_game == 3) {
					state = pizza_wait;
					nokia_lcd_clear();
					Game_Map();
					Player_scored();
					nokia_lcd_render();
				}
				else {
					state = pizza_init;
				}
				break;
			case pizza_wait:
				if (((player_posx == 12 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
					state = pizza_collect;
				}
				else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3)) {
					state = pizza_oven;
				}
				else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3)) {
					state = pizza_chop;
				}
				else if (((player_posx == 12 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 1 && player_posx <= 8)) && GetBit(~PINA, 3)) {
					state = tomato_collect;
				}
				else if (pizza_points >= 1 && pizza_chop_points >= 1 && pizza_oven_points >= 1 && tomato_points >= 1) {
					state = update_pizza_score;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else { 
					state = pizza_wait;
				}
				break;
			case pizza_collect:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = pizza_serve;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = pizza_collect;
				}
				break;
			case pizza_oven:
				if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && ovening_pizza < 40) {
					state = pizza_oven;
					ovening_pizza += 1;
				}
				else if (((player_posx == 66 && player_posy >= 1 && player_posy <= 9) || (player_posy == 13 && player_posx >= 69 && player_posx <= 82)) && GetBit(~PINA, 3) && ovening_pizza >= 40) {
					
					state = pizza_done;
					ovening_pizza = 0; 
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = pizza_oven;
				}
				break;
			case pizza_chop:
				if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_pizza < 20) {
					state = pizza_chop;
					chopping_pizza += 1;
				}
				else if (((player_posx == 66 && player_posy >= 33 && player_posy <= 42) || (player_posy == 29 && player_posx >= 70 && player_posx <= 82)) && GetBit(~PINA, 3) && chopping_pizza >= 20) {
					
					state = pizza_complete;
					chopping_pizza = 0;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = pizza_chop;
				}
				break;
			case tomato_collect:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = tomato_serve;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = tomato_collect;
				}
				break;
			case pizza_serve:
				state = pizza_wait;
				break;
			case tomato_serve:
				state = pizza_wait;
				break;
			case pizza_done:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = pizza_ovened_serve;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = pizza_done;
				}
				break;
			case pizza_complete:
				if ((player_posy == 41 && player_posx >= 30 && player_posx <= 50) && GetBit(~PINA, 3)) {
					state = pizza_chopped_serve;
				}
				else if (receipe_game == 0) {
					state = pizza_init;
				}
				else {
					state = pizza_complete;
				}
				break;
			case pizza_ovened_serve:
				state = pizza_wait;
				break;
			case pizza_chopped_serve:
				state = pizza_wait;
				break;
			case update_pizza_score:
				pizza_points = pizza_points - 1;
				pizza_oven_points = pizza_oven_points - 1;
				pizza_chop_points = pizza_chop_points - 1;
				tomato_points = tomato_points - 1;
				state = pizza_wait;
				break;
			default:
				break;
		}
		switch (state) {
			case pizza_init:
				break;
			case pizza_wait:
				break;
			case pizza_collect:
				
				break;
			case pizza_oven:
				break;
			case pizza_chop:
				break;
			case tomato_collect:
				
				break;
			case pizza_serve:
				
				player_score += 1;
				pizza_points += 1;
				break;
			case tomato_serve:
				
				player_score += 1;
				tomato_points += 1;
				break;
			case pizza_done:
				break;
			case pizza_complete:
				break;
			case pizza_ovened_serve:
				
				player_score += 3;
				pizza_oven_points += 1;
				break;
			case pizza_chopped_serve:
				
				player_score += 2;
				pizza_chop_points += 1;
				break;
			case update_pizza_score:
				player_score += 5;
				break;
			default:
				break;
		}
		return state;
	};

void Player_scored() {
	if (player_score >= 10) {
		temp_score = temp_score + (player_score / 10);
	}
	player_score = player_score % 10;
	nokia_lcd_set_cursor(19, 1);
	nokia_lcd_write_string("score:", 1);
	nokia_lcd_write_char(temp_score + '0', 1);
	nokia_lcd_write_char(player_score + '0', 1);
	nokia_lcd_render();
}

//Faster Movement
void move_fast() {
	if (GetBit(~PINA, 2)) {
		movement = fast_movement;
	}
	else {
		movement = 2;
	}
}

//Timer
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

enum FinalGame_SM_States {start, game_menu, sushi_game, steak_game, pizza_game, game_score};
	int FinalGame_SM(int state) {
		switch (state) {
			case start:
				state = game_menu;
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
					receipe_game = 1;
					state = sushi_game;
				}
				else if (find_direction_from_xy(x,y) == 2) {
					receipe_game = 2;
					state = steak_game;
				}
				else if (find_direction_from_xy(x,y) == 4) {
					receipe_game = 3;
					state = pizza_game;
				}
				else {
					state = game_menu;
				}
				break;
			case sushi_game:
				Timer_LED();
				if (game_time >= 600) {
					game_time = 0;
					state = game_score;
				}
				else {
					state = sushi_game;
				}
				break;
			case steak_game:
				Timer_LED();
				if (game_time >= 600) {
					game_time = 0;
					state = game_score;
				}
				else {
					state = steak_game;
				}
				break;
			case pizza_game:
				Timer_LED();
				if (game_time >= 600) {
					game_time = 0;
					state = game_score;
				}
				else {
					state = pizza_game;
				}
				break;
			case game_score:
				if (game_time < 100) {
					nokia_lcd_clear();
					nokia_lcd_set_cursor(12, 8);
					nokia_lcd_write_string("Score:", 2);
					nokia_lcd_set_cursor(33, 25);
					nokia_lcd_write_char(temp_score + '0', 2);
					nokia_lcd_write_char(player_score + '0', 2);
					nokia_lcd_render();
				}
				else if (game_time >= 100) {
					game_time = 0;
					state = game_menu; 
				}
				break;
			default:
				break;
		}
		switch (state) {
			case start:
				break;
			case game_menu:
				temp_score = 0;
				player_score = 0;
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
				clear_led();
				player_posx = init_player_posx;
				player_posy = init_player_posy;
				receipe_game = 0;
				break;
			default:
				break;
		}
		return state;
	};

int main(void) {
   	DDRB = 0xFF; PORTB = 0x00;
   	DDRA = 0xF0; PORTA = 0x0F;
   	DDRC = 0xFF; PORTC = 0x00;
   	DDRD = 0xFF; PORTD = 0x00;
	
	//Period for all tasks
	unsigned long int Player_MoveSM_calc = 5;
	unsigned long int Sushi_SM_calc = 5;
	unsigned long int Steak_SM_calc = 5;
	unsigned long int pizza_SM_calc = 5;
	unsigned long int FinalGame_SM_calc = 5;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(Player_MoveSM_calc, Sushi_SM_calc);
	tmpGCD = findGCD(tmpGCD, Steak_SM_calc);
	tmpGCD = findGCD(tmpGCD, pizza_SM_calc);
	tmpGCD = findGCD(tmpGCD, FinalGame_SM_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int Player_MoveSM_period = Player_MoveSM_calc/GCD;
	unsigned long int Sushi_SM_period = Sushi_SM_calc/GCD;
	unsigned long int Steak_SM_period = Steak_SM_calc/GCD;
	unsigned long int pizza_SM_period = pizza_SM_calc/GCD;
	unsigned long int FinalGame_SM_period = FinalGame_SM_calc/GCD;
	
	//Declare an array of tasks
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4 , &task5 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	// Task 1
	task1.state = init;//Task initial state.
	task1.period = Player_MoveSM_period;//Task Period.
	task1.elapsedTime = Player_MoveSM_period;//Task current elapsed time.
	task1.TickFct = &Player_MoveSM;//Function pointer for the tick.
	
	// Task 2
	task2.state = sushi_init;//Task initial state.
	task2.period = Sushi_SM_period;//Task Period.
	task2.elapsedTime = Sushi_SM_period;//Task current elapsed time.
	task2.TickFct = &Sushi_SM;//Function pointer for the tick.

	// Task 3
	task3.state = steak_init;//Task initial state.
	task3.period = Steak_SM_period;//Task Period.
	task3.elapsedTime = Steak_SM_period; // Task current elasped time.
	task3.TickFct = &Steak_SM; // Function pointer for the tick.
	
	// Task 4
	task4.state = pizza_init;//Task initial state.
	task4.period = pizza_SM_period;//Task Period.
	task4.elapsedTime = pizza_SM_period; // Task current elasped time.
	task4.TickFct = &pizza_SM; // Function pointer for the tick.
	
	// Task 5
	task5.state = start;//Task initial state.
	task5.period = FinalGame_SM_period;//Task Period.
	task5.elapsedTime = FinalGame_SM_period; // Task current elasped time.
	task5.TickFct = &FinalGame_SM; // Function pointer for the tick.
	
	TimerSet(GCD);
	TimerOn();
	
	InitADC();

	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);

	
	unsigned short i; // Scheduler for-loop iterator
    while (1) {
		x = ReadADC(0);
		y = ReadADC(1);
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
		if (GetBit(~PINA,2)) {
			movement = fast_movement;
		}
		else {
			movement = 2;
		}
		
		while(!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}

