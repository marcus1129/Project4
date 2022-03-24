#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver
#include <util/delay.h>

int UART_initASYNC0(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR0L = (unsigned char)(UART.ubrr>>8);
	UBRR0H = (unsigned char)UART.ubrr;
	UCSR0A |= (1<<RXC0);
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0)|(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	return 1;
};

int UART_initASYNC1(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR1L = (unsigned char)(UART.ubrr>>8);
	UBRR1H = (unsigned char)UART.ubrr;
	UCSR1A |= (1<<RXC1);
	UCSR1B |= (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1)|(1<<UCSZ12);
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
	return 1;
};

int UART_initASYNC2(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR2L = (unsigned char)(UART.ubrr>>8);
	UBRR2H = (unsigned char)UART.ubrr;
	UCSR2A |= (1<<RXC2);
	UCSR2B |= (1<<RXCIE2)|(1<<RXEN2)|(1<<TXEN2)|(1<<UCSZ22);
	UCSR2C |= (1<<UCSZ21)|(1<<UCSZ20);
	return 1;
};

int UART_initASYNC3(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR3L = (unsigned char)(UART.ubrr>>8);
	UBRR3H = (unsigned char)UART.ubrr;
	UCSR3A |= (1<<RXC1);
	UCSR3B |= (1<<RXCIE3)|(1<<RXEN3)|(1<<TXEN3)|(1<<UCSZ32);
	UCSR3C |= (1<<UCSZ31)|(1<<UCSZ30);
	return 1;
};

/*int initASYNC2X(int rate, int UARTReg, int UARTBaudRegList[13][4][3]){
	UART.ubrr = F_CPU/(8*rate)-1;
	return 1;
};

int initMASTER(int rate, int UARTReg, int UARTBaudRegList[13][4][3]){
	return 1;
};

int initSLAVE(int rate, int UARTReg, int UARTBaudRegList[13][4][3]){
	return 1;
};*/

int UART_init(int mode, int baudRate, int UARTId){
	if(UARTId > 3){
		return 0;
	}
	sei();
	UART.UARTReg = UARTId;
	int (*UART_initAsyncFuncs[4])(int rate) = {UART_initASYNC0, UART_initASYNC1, UART_initASYNC2, UART_initASYNC3};
	(*UART_initAsyncFuncs[UARTId])(baudRate);
	return 1;
}

int UART_transmitChar0(char transmitionData){
	//while(~(UCSR0A & (1<<UDRE0))){}
	UCSR0B &= ~(1<<TXB80);
	if(transmitionData & 0x0100){
		UCSR0B |= (1<<TXB80);
	}
	UDR0 = transmitionData;
	return 1;
};

int UART_transmitChar1(char transmitionData){
	//while(~(UCSR1A & (1<<UDRE1))){}
	UCSR1B &= ~(1<<TXB81);
	if(transmitionData & 0x0100){
		UCSR1B |= (1<<TXB81);
	}
	UDR1 = transmitionData;
	return 1;
};

int UART_transmitChar2(char transmitionData){
	//while(~(UCSR2A & (1<<UDRE2))){}
	UCSR2B &= ~(1<<TXB82);
	if(transmitionData & 0x0100){
		UCSR2B |= (1<<TXB82);
	}
	UDR2 = transmitionData;
	return 1;
};

int UART_transmitChar3(char transmitionData){
	//while(~(UCSR3A & (1<<UDRE3))){}
	UCSR3B &= ~(1<<TXB83);
	if(transmitionData & 0x0100){
		UCSR3B |= (1<<TXB83);
	}
	UDR3 = transmitionData;
	return 1;
};

int UART_transmitChar(char transmitionData){
	int (*UART_transmitCharFuncs[4])(char data) = {UART_transmitChar0, UART_transmitChar1, UART_transmitChar2, UART_transmitChar3};
	(*UART_transmitCharFuncs[UART.UARTReg])(transmitionData);
	return 1;
}

int UART_receiveChar0(){
	UART.RXh = UCSR0B;
	UART.RXl = UDR0;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int UART_receiveChar1(){
	UART.RXh = UCSR1B;
	UART.RXl = UDR1;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int UART_receiveChar2(){
	UART.RXh = UCSR2B;
	UART.RXl = UDR2;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int UART_receiveChar3(){
	UART.RXh = UCSR3B;
	UART.RXl = UDR3;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int UART_receiveChar(){
	int(*UART_receiveCharFuncs[4])() = {UART_receiveChar0, UART_receiveChar1, UART_receiveChar2, UART_receiveChar3};
	(*UART_receiveCharFuncs[UART.UARTReg])();
	return 1;
}

struct uart UART = {
	.UART_init = UART_init,
	.UART_transmitChar = UART_transmitChar,
};

ISR(USART0_RX_vect){
	UART_receiveChar();
};

ISR(USART1_RX_vect){
	UART_receiveChar();
};

ISR(USART2_RX_vect){
	UART_receiveChar();
};

ISR(USART3_RX_vect){
	UART_receiveChar();
};