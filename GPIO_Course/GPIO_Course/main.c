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
	//DIRCLR gj�r det omvendte av DIRSET
	//Er den 1, s� gir den str�m.
	
	//Jobber med PORTC registeret
	//bm betyr bit mask.
	//Klargj�r til � brukes pinnen 6
	PORTC.DIRSET = PIN6_bm; //Setter en bit mask inn i DIRSET. PIN6 kobla til lyset
	//Vil ha noe fra pinnen 6
	PORTC.OUTSET = PIN6_bm; //Kobla til knappen
	
	//Skrur til en input
	PORTC.DIRCLR = PIN7_bm;
	//Uten pullup, s� er spenningen flytende. Med den er den definert. 
	PORTC.PIN7CTRL = PORT_PULLUPEN_bm;
    while(1)
    {
		//void loop
        //TODO:: Please write your application code 
		//Skal ikke ha break i loopen, pga den skal kj�re hele tiden
		
		//Sjekker om PIN7 er lav. Er den lav, s� er den trykka inn
		//N�r den er trykka inn, s� sendes all spenning til ground
		/*if (!(PORTC.IN & PIN7_bm))
		{
			//Da er lyset p�
			//Pinnen er h�y
			PORTC.OUTCLR = PIN6_bm;
		} else {
			//Lyset er av
			//Setter pinnen h�y
			PORTC.OUTSET = PIN6_bm;
		}***/
		
		if(!(PORTC.IN & PIN7_bm)){
			PORTC.OUTTGL = PIN6_bm;
			while(!(PORTC.IN & PIN7_bm)){}
		}
    }
}