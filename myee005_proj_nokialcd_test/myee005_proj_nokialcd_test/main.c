/*
 * myee005_proj_nokialcd_test.c
 *
 * Created: 6/1/2019 10:09:06 PM
 * Author : myyee
 */ 

#define F_CPU 8250000UL
#include <avr/io.h>
#include "nokia5110.h"
#include "nokia5110.c"
//#include "nokia5110_chars.h"

enum Game_Map_States {init, start_map}state;
void Game_MapSM() {
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

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
//	nokia_lcd_set_cursor(1, 10);
//	nokia_lcd_write_string("hello", 3);
//	nokia_lcd_render();
	while (1){
		Game_MapSM();
		
	}
}

