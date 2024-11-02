/*
 * main.c
 *
 * Created: 10/17/2024 4:12:19 PM
 *  Author: Bruker
 */ 
#define F_CPU 4000000
#include <avr/io.h>
#include <avr/interrupt.h>

// Forteller kompileren at den ikke skal overse. RESPECT!
volatile uint16_t counter;

void LED_setup(){
	PORTC.DIRSET = PIN6_bm;
	PORTC.OUTSET = PIN6_bm;
}

void TCB0_setup(){
	// Capture/Compare register value
	TCB0.CCMP = 50000; //Klokkesyklusen. Skjer hver 0.025 sekund
	TCB0.CTRLB = TCB_CNTMODE_INT_gc;
	TCB0.INTCTRL = TCB_CAPT_bm; //Skrudd på interrupt
	TCB0.CTRLA = TCB_CLKSEL_DIV1_gc | TCB_ENABLE_bm;
}

//Interrupt funker gjennom ISR
int main(void)
{
	
	LED_setup();
	TCB0_setup();
	sei(); //Set interrupt 
    while(1)
    {
        //TODO:: Please write your application code 
		
		
    }
}

ISR(TCB0_INT_vect){
	// 2 000 000 / 50 000 = 40
	//Vi vil at lyset skal lyse hver sekund. Derfor teller vi antall ganger interrupt skjer i counter,
	//Hvis den er lik 40, så har det gått 1 sek, og da lyser LEDen
	if (counter == 40){
		//Slår av og på LED-lyset (toggle)
		PORTC.OUTTGL = PIN6_bm;
		counter = 0; //Husk å resette counter for hver gang
	}
	
	//Når en interrupt skjer, så settes et interrupt-flag i TCB0 sin interrupt-flags register
	TCB0.INTFLAGS  = TCB_CAPT_bm;
	
	//Teller antall ganger interrupt oppstår
	counter++;
}