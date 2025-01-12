/*
 * main.c
 *
 * Created: 11/2/2024 8:54:56 AM
 *  Author: Selma
 */ 

#include <avr/io.h>

int main(void)
{
	PORTA.DIRCLR = PIN6_bm; // Set's PA6 as input - Button pressed down
	PORTA.DIRSET = PIN5_bm; // Set's PA5 as output - The LED should light up
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm; // Need a defined condition when it's not activated
	
    while(1)
    {
        //TODO:: Please write your application code 
		if (!(PORTA.IN & PIN6_bm)) {
			PORTA.OUTSET = PIN5_bm;
		} else {
			PORTA.OUTCLR = PIN5_bm;
		}
		
    }
}