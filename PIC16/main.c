#include "config.h"							//configuration words
#include "gpio.h"                           //we use gpio functions
#include "delay.h"                          //we use software delays
#include "led4_pins.h"						//4-digit led driver
#include "tmr0.h"							//we use tmr0
#include "uart.h"							//we use uart tx/rx

//hardware configuration
#define LEDx4_BPS			UART_BR_9600	//set the display's baud rate
//end hardware configuration

//isr
void interrupt isr(void) {
	if (T0IF) tmr0_isr();
}
	
//global defines

//global variables

//process the ch received
char process(char ch) {
	static uint8_t rbuf[4] = {'0', '0', '0', '0'};
	char tmp;
	
	switch (ch) {
	case '\n':								//'\n' or '\r\ causes the display to update
	case '\r':
		//anyone of those two characters causes the display to update
		//rbuf is ascii, lram is numerical
		lRAM[0]=rbuf[0] - '0';
		lRAM[1]=rbuf[1] - '0';
		lRAM[2]=rbuf[2] - '0';
		lRAM[3]=rbuf[3] - '0';
		//for debugging only
		//uart1_put(rbuf[0]); //rbuf[0]='0';
		//uart1_put(rbuf[1]); //rbuf[1]='0';
		//uart1_put(rbuf[2]); //rbuf[2]='0';
		//uart1_put(rbuf[3]); //rbuf[3]='0';
		//uart1_puts("display updated\n\r");
		tmp = '\n';							//flash out the next device anc cause the display to update
		break;
	//valid chars are: '0'..'9'
	//you can expand the valid data set to your liking here
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		//circular buffer - last 4 digits effective
		tmp = rbuf[0];						//move the oldest char to the next device
		rbuf[0]=rbuf[1];
		rbuf[1]=rbuf[2];
		rbuf[2]=rbuf[3];
		rbuf[3]=ch;
		break;
	default: tmp = '0'; break;				//default char is '0';
	}
	return tmp;
}


int main(void) {
	uint16_t count;
	uint16_t tmp;
	char ch;
	
	mcu_init();							    //initialize the mcu at 16Mhz
	led_init();								//reset the led
	tmr0_init(TMR0_PS_32x);					//reset tmr0 -> need to maintain a minimum refresh rate of 25Hz
	tmr0_act(led_display);					//install the handler
	uart_init(LEDx4_BPS);					//reset uart to desired baud rate
	ei();									//enable interrupts
	while (1) {
#if 1										//for receiver and transmitter
		//if uart data is available
		if (uart_available()) {				//if data is available
			ch = uart_get();				//receive the data
			//uart2_put(ch);				//transmit it to the next device in chain
			uart_put(process(ch));			//process the data received and send out only the valid char
		}

#else										//for transmitter only - for debugging, to simulate an input
		uart_puts((char *)"12345678\n\r");
		delayms(100);						//waste sometime
#endif
	}
}

