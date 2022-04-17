/* 
 * Project4.c
 *
 * Created: 3/10/2022 3:05:52 PM
 * Author : Marcus Korre
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "clock.h"
#include <util/delay.h>

void initExternalInterrupt(){
	DDRE = 0x00; //E4
	PORTE = 0xFF;
	EICRB = 0b00000010;
	EIMSK = 0b00010000;
	sei();
}

volatile int gate1 = 0;

int main(void){
	_i2c_address = 0X78;
	I2C_Init();
	InitializeDisplay();
	print_fonts();
	clear_display();
	
	CLOCK.clock_init();
	CLOCK.clock_enableTimerInterrupt(0);
	UART.UART_init(ASYNC, 19200, 0);
	initExternalInterrupt();
	UART.UART_transmitStr("To set the clock, enter a time in the format: \"hh:mm:ss\" and press the button.");
	_delay_ms(1000);
    while (1){
		CLOCK.clock_makeTimeStr();
		//UART.UART_transmitStr(CLOCK.timeStr);
		for(int i = 0; i < 8; i++){
			UART.UART_transmitChar('w');
			sendCharXY(CLOCK.timeStr[i], 0, i);
		}
		if(gate1){
			CLOCK.clock_updateClock(UART.timeVal);
			gate1 = 0;
		}
		if(CLOCK.interruptFlag){
			clear_display();
			CLOCK.interruptFlag = 0;
		}
    }
}

ISR(INT4_vect){
	gate1 = 1;
}