/*
 * myee005_proj_pizzagame_test.c
 *
 * Created: 6/3/2019 9:46:34 PM
 * Author : myyee
 */ 

#include <avr/io.h>

unsigned int ovening_pizza = 0;
unsigned int chopping_pizza = 0;
unsigned int pizza_points = 0;
unsigned int pizza_oven_points = 0;
unsigned int pizza_chop_points = 0;

enum pizza_SM_States {pizza_init, pizza_wait, pizza_collect, pizza_chop, pizza_oven, pizza_serve, pizza_done, pizza_complete, pizza_ovened_serve, pizza_chopped_serve, update_pizza_score}pizza_state;
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
		else if (pizza_points >= 1 && pizza_chop_points >= 1 && pizza_oven_points >= 1) {
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
		case pizza_serve:
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
		case pizza_serve:
		PORTD = SetBit(PORTD, 6, 0);
		player_score += 1;
		pizza_points += 1;
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


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

