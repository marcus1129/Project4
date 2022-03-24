/* 
 * Project4.c
 *
 * Created: 3/10/2022 3:05:52 PM
 * Author : Marcus Korre
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "clock.h"
#include <util/delay.h>

int main(void){
	_i2c_address = 0X78;
	I2C_Init();
	InitializeDisplay();
	print_fonts();
	clear_display();
	
	CLOCK.clock_init();
	CLOCK.clock_enableTimerInterrupt(2);
	UART.UART_init(ASYNC, 19200, 1);
	
	char temp[10] = {' '};
    while (1){
		UART.UART_transmitChar('s');
		
		sprintf(temp, "%d", CLOCK.hour);
		sendStrXY(temp, 0, 0);
		sendCharXY(':', 0, 2);
		sprintf(temp, "%d", CLOCK.minute);
		sendStrXY(temp, 0, 3);
		sendCharXY(':', 0, 5);
		sprintf(temp, "%d", CLOCK.second);
		sendStrXY(temp, 0, 6);
		if(CLOCK.interruptFlag){
			clear_display();
			CLOCK.interruptFlag = 0;
		}
		
		
		
		/*CLOCK.clock_increment();
		sprintf(temp1, "%d", CLOCK.second);
		sprintf(temp2, "%d", CLOCK.minute);
		sprintf(temp3, "%d", CLOCK.hour);
		sendStrXY(UART.RX, 0, 0);
		sendCharXY(':', 0, 2);
		sendStrXY(UART.RX, 0, 3);
		sendCharXY(':', 0, 5);
		sendStrXY(UART.RX, 0, 6);
		_delay_ms(1000);*/
    }
}

