/*
 * main.c
 *
 * Created: 2/1/2025 12:15:39 PM
 *  Author: Bruker
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE 115200
#define F_CPU 3333333
#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
uint8_t number = 0;

void setup_usart(){
	PORTB.DIRSET = PIN2_bm; // set high
	USART0.BAUD = (uint8_t) USART_BAUD_RATE(BAUD_RATE); // set baud rate
	USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm; // enable the transmitter and receiver
	USART0.CTRLA = USART_RXCIE_bm; // set unread data from receiver in control register A
}

void setup_tcb(){
	uint8_t TOP = 255; // set top value when the interrupts hits
	TCB0.CCMP = TOP; // something
	TCB0.CTRLB = TCB_CNTMODE_SINGLE_gc; // something (for å ikke overskrive noe bruk =|)
	TCB0.INTCTRL = TCB_CAPT_bm; // starts the interrupt
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc; // clock speed
	TCB0.CTRLA = TCB_ENABLE_bm; // enable interrupt
}


int main(void)
{
	PORTA.DIRSET = PIN5_bm;
	PORTA.OUTSET = PIN5_bm;
	PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
	
    while(1)
    {
        //TODO:: Please write your application code 
		setup_usart();
		setup_tcb();
		sei();
		
		// check if button is pressed
		if(!(PORTA.IN & PIN7_bm)) {
			USART0.TXDATAL = number; // load transmit buffer with the data we want to send
		}
		
    }
}

ISR(TCB0_INT_vect) {
	// initilize the interrupt
	TCB0.INTFLAGS = TCB_CAPT_bm;
	TCB0.CNT = 0;
	TCB0.CTRLA = TCB_ENABLE_bm;
}

ISR(USART0_RXC_vect) {
	number = USART0.RXDATAL;
	//PORTA.OUTTGL = PIN5_bm;

	if (number < 255) { // only increment if the number is less than 255
		number++;
		USART0.TXDATAL = number;
	} else { // if the number is greater than 255, then set led on
		PORTA.OUTSET = PIN7_bm;
		number = 0;
	}
}