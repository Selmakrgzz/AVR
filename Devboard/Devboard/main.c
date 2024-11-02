/*
 * main.c
 *
 * Created: 11/2/2024 8:54:56 AM
 *  Author: Bruker
 */ 

#include <avr/io.h>

int main(void)
{
	PORTA.DIRCLR = PIN6_bm; // Initialiserer PA6 som input
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}