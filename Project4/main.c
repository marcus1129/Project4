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
#include <string.h>

volatile int gate1 = 0;
volatile int gate = 0;
int gate2 = 0;
volatile int bufferIndex = 0;
char buffer[100];
char RX = ' ';
int carriageReturn = 0;

void initExternalInterrupt(){
	DDRE = 0x00; //E4
	PORTE = 0xFF;
	EICRB = 0b00000010;
	EIMSK = 0b00010000;
	sei();
}

void init(){
	int ubrr = 51;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

void transChar(char transData){
	while(gate){}
	UDR0 = transData;
	gate = 1;
}

void transStr(char* transDataStr, int endLine){
	for(int i = 0; transDataStr[i] != '\0'; i++){
		transChar(transDataStr[i]);
	}
	if(endLine){
		transChar('\r');
		transChar('\n');
	}
}

void receiveChar(){
	RX = UDR0;
	buffer[bufferIndex] = RX;
	bufferIndex += 1;
	if(RX == '\r'){
		carriageReturn = 1;
	}
	else if(carriageReturn && RX == '\n'){
		gate2 = 1;
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
	transStr("To set the clock, enter a time in the format: \"hh:mm:ss\" and press the button.", 1);
	_delay_ms(1000);
    while (1){
		CLOCK.clock_makeTimeStr();
		if(gate2){
			carriageReturn = 0;
			gate2 = 0;
			buffer[bufferIndex] = '\0';
			bufferIndex = 0;
		}
		for(int i = 0; i < 8; i++){
			sendCharXY(CLOCK.timeStr[i], 0, i);
		}
		if(gate1){
			CLOCK.clock_updateClock(buffer);
			gate1 = 0;
		}
		if(CLOCK.interruptFlag){
			clear_display();
			transStr("Klokken er: ", 0);
			transStr(CLOCK.timeStr, 1);
			CLOCK.interruptFlag = 0;
		}
    }
}

ISR(INT4_vect){
	gate1 = 1;
}

ISR(USART0_RX_vect){
	receiveChar();
};

ISR(USART0_TX_vect){
	gate = 0;
}