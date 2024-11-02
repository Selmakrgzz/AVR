/*
 * main.c
 *
 * Created: 10/16/2024 4:12:43 PM
 *  Author: Bruker
 */ 

#include <avr/io.h>

int main(void)
{
	//Start here
	//void start
	
	//DIR EKSEMPEL: 00000000
	//Kan sette 00000000 = PIN6_bm => 00000011
	//Kan sette PIN7_bm = 00000001
	//DIRCLR gjør det omvendte av DIRSET
	//Er den 1, så gir den strøm.
	
	//Jobber med PORTC registeret
	//bm betyr bit mask.
	//Klargjør til å brukes pinnen 6
	PORTC.DIRSET = PIN6_bm; //Setter en bit mask inn i DIRSET. PIN6 kobla til lyset
	//Vil ha noe fra pinnen 6
	PORTC.OUTSET = PIN6_bm; //Kobla til knappen
	
	//Skrur til en input
	PORTC.DIRCLR = PIN7_bm;
	//Uten pullup, så er spenningen flytende. Med den er den definert. 
	PORTC.PIN7CTRL = PORT_PULLUPEN_bm;
    while(1)
    {
		//void loop
        //TODO:: Please write your application code 
		//Skal ikke ha break i loopen, pga den skal kjøre hele tiden
		
		//Sjekker om PIN7 er lav. Er den lav, så er den trykka inn
		//Når den er trykka inn, så sendes all spenning til ground
		/*if (!(PORTC.IN & PIN7_bm))
		{
			//Da er lyset på
			//Pinnen er høy
			PORTC.OUTCLR = PIN6_bm;
		} else {
			//Lyset er av
			//Setter pinnen høy
			PORTC.OUTSET = PIN6_bm;
		}***/
		
		if(!(PORTC.IN & PIN7_bm)){
			PORTC.OUTTGL = PIN6_bm;
			while(!(PORTC.IN & PIN7_bm)){}
		}
    }
}