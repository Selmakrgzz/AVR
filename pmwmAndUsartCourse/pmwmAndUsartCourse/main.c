/*
 * main.c
 *
 * Created: 10/22/2024 1:40:59 PM
 *  Author: Bruker
 */ 

#define F_CPU 4000000
#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#define USART_DBG USART1 //Connected to the debugger
#define USART_DBG_BAUD_RATE 115200

#define TCB_LED TCB0 //PA2 Output



#include <avr/io.h>
#include <avr/interrupt.h>

/*----------Utility-Functions-----------*/

//Standard strlen function
//Returnerer lengen av str
uint8_t strlenu8(const char* str) {
	uint8_t len = 0;
	for (uint8_t i = 0; str[i] != '\0'; i++) {
		len++;
	}
	return len;
}

//Standard power function
//Regner potensen av et tall med heltall som basis og eksponent
uint8_t powu8(uint8_t base, uint8_t exponent) {
	uint8_t result = 1;
	for (uint8_t i = 0; i < exponent; i++) {
		result = result * base;
	}
	return result;
}

//Convert from string to uint8
//Konverterer char til uint8
uint8_t decimalStrToIntu8(const char* str, uint8_t size) {
	char rev[4]; //Max 3 digits for u8
	
	//Zero the buffer
	//Hver gang metoden kalles, så blir rev "renset"
	for (uint8_t i = 0; i < 4; i++) {
		rev[i] = 0;
	}

	//Reverse the string to get least significant digit
	//Legger til char karakterene baklengs i rev
	for (uint8_t i = 0; i < size; i++) {
		rev[i] = str[size-i-1];
	}
	
	//Sum up all the digits by powers of 10
	//Legger sammen tallene slik at vi får dem i riktig retning og plass
	uint8_t integer = 0;
	for (uint8_t i = 0; i < size; i++) {
		//Sjekker at det er et tall mellom 0 - 9
		if (rev[i] >= '0' && rev[i] <= '9') { //Check that it is a digit
			//Misuse ASCII to convert from char to number
			// '0' = 48
			// '0' - '0' = 0
			// '1' - '0' = 1
			integer = integer + (rev[i] - '0') * powu8(10, i); //Eks. 0 + 3 * 1 = 3 | 3 + 2 * 10 = 23 | 23 + 1 * 100 = 123
		}
		else {
			return 0; // Error, detected other characters than 0-9
		}
	}
	return integer;
}

/*----------Setup and Control-----------*/

//Volatile forteller kompilatoren at den alltid må lese variablen fra minnet, 
//og ikke bruke en optimalisert versjon
volatile uint8_t on_time = 128;

//For input processing
volatile char input_buffer[4];
volatile uint8_t index = 0;

//USART muliggjør seriell kommunikasjon mellom enheter

//Debug?
//Debug control register? s.231
/*void setupUsartDBG(){
	//TODO: 3
	//pc0 = output
	//se på register til usart s.366
	//rxdaral
	
	USART1.BAUD = USART_BAUD_RATE(USART_DBG_BAUD_RATE);
	//USART1.CTRLC
	PORTC_DIRSET = PIN0_bm;
	//Enabled receiver and transmitter
	USART1.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	USART1.CTRLA = USART_RXCIE_bm;

	
}*/

void setupADC(){
	PORTD.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc; //hvis en port skal analogt leses
	VREF.ADC0REF = VREF_REFSEL_VDD_gc; //1
	ADC0.CTRLA = ADC_RESSEL_12BIT_gc; //3
	ADC0.CTRLC = ADC_PRESC_DIV8_gc; //7
	ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc; //8
	ADC0.INTCTRL = ADC_RESRDY_bm; 
	ADC0.CTRLA |= ADC_ENABLE_bm; //11
}

void startCONV(){
	ADC0.COMMAND = ADC_STCONV_bm;
}

//PWM = Pulse Width Modulation
//Brukes for å kontrollere mengden strøm som går til en elektrisk enhet 
//ved å variere pulslengden i en fast tidsperiode
//s. 212
void setupPWM(){
	/*TCB0.CCMP = 255; //period register
	TCB0.CTRLB = 1; //counting clock ticks according to the prescaler setting
	TCB0.EVCTRL = 1; //events are counted instead of clock ticks
	return TCB0.CNT; //counter value */
	
	PORTA.DIRSET = PIN2_bm; //Setter den til output
	TCB0.CCMPL = 255; //period register
	TCB0.CCMPH = 128; //This value does not matter
	TCB0.CTRLB = TCB_CCMPEN_bm | TCB_CNTMODE_PW_gc;
	TCB0.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_DIV1_gc;
}

//Duty Cycle referer til hvor lenge strømmen er på ilø en enkel periode
//Bestemme hvor lenge strømmen er på
void PWMSetCompare(uint8_t new_compare){
	//TODO: 2
	TCB_LED.CCMP = new_compare;
}

/*----------Main Loop-----------*/

int main(void)
{
	//setupUsartDBG();
	setupADC();
	startCONV();
	setupPWM();
	sei();
	while(1)
	{
		PWMSetCompare(on_time);
	}
}

/*----------Interrupts-----------*/

//Timeren trigges når USART mottar ny data
//ISR er da klar for å håndtere dataen
//Når interrupten inntreffer, så stopper CPUen annet arbeid i hovedprogrammet midlertidig
//og hopper inn i IST for å håndtere nye dataen
/*ISR(USART1_RXC_vect){
	//TODO: 4
	char read = USART_DBG.RXDATAL;
	if (read == '\r'){
		on_time = decimalStrToIntu8((const char*) input_buffer, strlenu8((const char*) input_buffer));
		index = 0;
		input_buffer[0] = 0; input_buffer[1] = 0;
		input_buffer[2] = 0; input_buffer[3] = 0;
	} else {
		if (index < 3){
			input_buffer[index] = read;
			index++;
		}
	}
}*/

ISR(ADC0_RESRDY_vect){
	float res = (ADC0.RES/4096.0);
	uint8_t conv = (uint8_t)(res * 255) -69;
	
	PWMSetCompare(conv);
	setupADC();
	ADC0.INTFLAGS = ADC_RESRDY_bm;
}