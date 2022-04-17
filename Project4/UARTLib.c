#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UARTLib.h"
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver
#include <util/delay.h>

volatile int gate = 0;
volatile int bufferIndex = 0;
int ubrr;
int UARTReg;
char buffer[10];
char timeVal[10];
char RXh;
char RXl;
char RX;

int UART_initASYNC0(int rate){ 
	ubrr = F_CPU/(16*rate)-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	return 1;
};

int UART_initASYNC1(int rate){ 
	ubrr = F_CPU/(16*rate)-1;
	UBRR1L = (unsigned char)(ubrr>>8);
	UBRR1H = (unsigned char)ubrr;
	UCSR1A |= (1<<RXC1);
	UCSR1B |= (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1)|(1<<UCSZ12);
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
	return 1;
};

int UART_initASYNC2(int rate){
	ubrr = F_CPU/(16*rate)-1;
	UBRR2L = (unsigned char)(ubrr>>8);
	UBRR2H = (unsigned char)ubrr;
	UCSR2A |= (1<<RXC2);
	UCSR2B |= (1<<RXCIE2)|(1<<RXEN2)|(1<<TXEN2)|(1<<UCSZ22);
	UCSR2C |= (1<<UCSZ21)|(1<<UCSZ20);
	return 1;
};

int UART_initASYNC3(int rate){
	ubrr = F_CPU/(16*rate)-1;
	UBRR3L = (unsigned char)(ubrr>>8);
	UBRR3H = (unsigned char)ubrr;
	UCSR3A |= (1<<RXC1);
	UCSR3B |= (1<<RXCIE3)|(1<<RXEN3)|(1<<TXEN3)|(1<<UCSZ32);
	UCSR3C |= (1<<UCSZ31)|(1<<UCSZ30);
	return 1;
};

int UART_init(int mode, int baudRate, int UARTId){
	if(UARTId > 3){
		return 0;
	}
	sei();
	UARTReg = UARTId;
	/*int (*UART_initAsyncFuncs[4])(int rate) = {UART_initASYNC0, UART_initASYNC1, UART_initASYNC2, UART_initASYNC3};
	(*UART_initAsyncFuncs[UARTId])(baudRate);*/
	UART_initASYNC0(baudRate);
	bufferIndex = 0;
	strcpy(timeVal, "");
	strcpy(buffer, "");
	return 1;
}

int UART_transmitChar0(char transmitionData){
	UDR0 = 0x31;
	if(!gate){
		UDR0 = 0x31;
		gate = 1;
	}
	return 1;
};

int UART_transmitChar1(char transmitionData){
	UCSR1B &= ~(1<<TXB81);
	if(transmitionData & 0x0100){
		UCSR1B |= (1<<TXB81);
	}
	UDR1 = transmitionData;
	return 1;
};

int UART_transmitChar2(char transmitionData){
	UCSR2B &= ~(1<<TXB82);
	if(transmitionData & 0x0100){
		UCSR2B |= (1<<TXB82);
	}
	UDR2 = transmitionData;
	return 1;
};

int UART_transmitChar3(char transmitionData){
	UCSR3B &= ~(1<<TXB83);
	if(transmitionData & 0x0100){
		UCSR3B |= (1<<TXB83);
	}
	UDR3 = transmitionData;
	return 1;
};

int UART_transmitChar(char transmitionData){
	/*int (*UART_transmitCharFuncs[4])(char data) = {UART_transmitChar0, UART_transmitChar1, UART_transmitChar2, UART_transmitChar3};
	(*UART_transmitCharFuncs[UART.UARTReg])(transmitionData);*/
	UART_transmitChar0('s');
	return 1;
}

int UART_transmitStr(char* transmitionData){
	int i = 0;
	while(transmitionData[i] != '\0'){
		UART_transmitChar(transmitionData[i]);
		i++;
	}
	return 1;
}

int UART_receiveChar0(){
	/*UART.RXh = UCSR0B;
	UART.RXl = UDR0;
	UART.RXh = (UART.RXh >> 1) & 0x01;*/
	RX = /*((UART.RXh << 8) | UART.RXl)*/UDR0;
	return 1;
}

int UART_receiveChar1(){
	RXh = UCSR1B;
	RXl = UDR1;
	RXh = (RXh >> 1) & 0x01;
	RX = ((RXh << 8) | RXl);
	return 1;
}

int UART_receiveChar2(){
	RXh = UCSR2B;
	RXl = UDR2;
	RXh = (RXh >> 1) & 0x01;
	RX = ((RXh << 8) | RXl);
	return 1;
}

int UART_receiveChar3(){
	RXh = UCSR3B;
	RXl = UDR3;
	RXh = (RXh >> 1) & 0x01;
	RX = ((RXh << 8) | RXl);
	return 1;
}

int UART_receiveChar(){
	int(*UART_receiveCharFuncs[4])() = {UART_receiveChar0, UART_receiveChar1, UART_receiveChar2, UART_receiveChar3};
	(*UART_receiveCharFuncs[UARTReg])();
	buffer[bufferIndex] = RX;
	if(bufferIndex == 10){
		strncpy(timeVal, buffer, 10);
		bufferIndex = 0;
	}

	return 1;
}

/*struct uart UART = {
	.UART_init = UART_init,
	.UART_transmitChar = UART_transmitChar,
	.UART_transmitStr = UART_transmitStr
};*/

ISR(USART0_RX_vect){
	UART_receiveChar();
	bufferIndex += 1;
};

ISR(USART1_RX_vect){
	UART_receiveChar();
	bufferIndex += 1;
};

ISR(USART2_RX_vect){
	UART_receiveChar();
	bufferIndex += 1;
};

ISR(USART3_RX_vect){
	UART_receiveChar();
	bufferIndex += 1;
};

ISR(USART0_TX_vect){
	gate = 0;
}