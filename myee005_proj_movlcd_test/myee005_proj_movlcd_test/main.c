/*
 * myee005_proj_movlcd_test.c
 *
 * Created: 6/2/2019 3:27:31 PM
 * Author : myyee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8250000UL
#include <avr/io.h>
#include "nokia5110.h"
#include "nokia5110.c"
//#include "nokia5110_chars.h"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned int player_posx = 35;
unsigned int player_posy = 1;

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
	return(1); // direction is up

	else if(x>765 && y>255 && y<765)
	return(2); //direction is right

	else if(x>255 && x<765 && y<255)
	return(3); //direction is down

	else if(x<255 && y>255 && y<765)
	return(4); // direction is left

	else {return (0);}
	//	return(0); //default state
}

enum Game_Map_States {init, start_map}Game_Map_state;
void Game_MapSM(int state) {
	switch(state) {
		case init:
		state = start_map;
		break;
		case start_map:
		state = start_map;
		break;
		default:
		state = start_map;
		break;
	}
	
	switch(state) {
		case init:
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
		nokia_lcd_set_cursor(25,41);
		nokia_lcd_write_string("|____|", 1);
		nokia_lcd_render();
		break;
		default:
		break;
	}
}

enum Player_Move_States {init2, wait, up, right, down, left}Player_Move_state;
void Player_MoveSM(int state) {
		switch (state) {
			case init:
			state = wait;
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
			else {
				state = wait;
			}
			break;
			case up:
			if (find_direction_from_xy(x,y) == 1) {
				state = up;
			}
			else {
				state = wait;
			}
			break;
			case right:
			if (find_direction_from_xy(x,y) == 2) {
				state = right;
			}
			else {
				state = wait;
			}
			break;
			case down:
			if (find_direction_from_xy(x,y) == 3) {
				state = down;
			}
			else {
				state = wait;
			}
			break;
			case left:
			if (find_direction_from_xy(x,y) == 4) {
				state = left;
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
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
			case up:
			player_posy = player_posy - 5;
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
			case right:
			player_posx = player_posx + 5;
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
			case down:
			player_posy = player_posy + 5;
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
			case left:
			player_posx = player_posx - 5;
			nokia_lcd_set_cursor(player_posx, player_posy);
			nokia_lcd_write_string("o", 1);
			break;
			default:
			break;
		}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	const unsigned long period = 100;
	Game_Map_state = init;
	Player_Move_state = init2;
	
	InitADC();
	
	TimerSet(period);
	TimerOn();
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
	//Game_MapSM(Game_Map_state);
	
	while (1){
		x = ReadADC(0);
		y = ReadADC(1);
		nokia_lcd_clear();
		Player_MoveSM(Player_Move_state);
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

