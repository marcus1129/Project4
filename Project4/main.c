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
//#include "UARTLib.h"
#include "clock.h"
#include <util/delay.h>

volatile int gate1 = 0;
volatile int gate = 0;
volatile int bufferIndex = 0;
char RX = ' ';

void initExternalInterrupt(){
	DDRE = 0x00; //E4
	PORTE = 0xFF;
	EICRB = 0b00000010;
	EIMSK = 0b00010000;
	sei();
}

void init(){
	int ubrr = F_CPU/(16*19200)-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

int receiveChar(){
	/*UART.RXh = UCSR0B;
	UART.RXl = UDR0;
	UART.RXh = (UART.RXh >> 1) & 0x01;*/
	RX = /*((UART.RXh << 8) | UART.RXl)*/UDR0;
	return 1;
}

void trans(){
	if(!gate){
		UDR0 = 0x31;
		gate = 1;
	}
}

int main(void){
	_i2c_address = 0X78;
	I2C_Init();
	InitializeDisplay();
	print_fonts();
	clear_display();
	init();
	CLOCK.clock_init();
	CLOCK.clock_enableTimerInterrupt(0);
	//UART_init(ASYNC, 19200, 0);
	initExternalInterrupt();
	//UART_transmitStr("To set the clock, enter a time in the format: \"hh:mm:ss\" and press the button.");
	_delay_ms(1000);
    while (1){
		trans();
		CLOCK.clock_makeTimeStr();
		//UART.UART_transmitStr(CLOCK.timeStr);
		/*for(int i = 0; i < 8; i++){
			UART_transmitChar('w');
			sendCharXY(CLOCK.timeStr[i], 0, i);
		}*/
		if(gate1){
			//CLOCK.clock_updateClock(timeVal);
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

ISR(USART0_RX_vect){
	receiveChar();
	bufferIndex += 1;
};

ISR(USART0_TX_vect){
	gate = 0;
}