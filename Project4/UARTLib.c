#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver
#include <util/delay.h>

int initASYNC0(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR0L = (unsigned char)(UART.ubrr>>8);
	UBRR0H = (unsigned char)UART.ubrr;
	UCSR0A |= (1<<RXC0);
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0)|(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	return 1;
};

int initASYNC1(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR1L = (unsigned char)(UART.ubrr>>8);
	UBRR1H = (unsigned char)UART.ubrr;
	UCSR1A |= (1<<RXC1);
	UCSR1B |= (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1)|(1<<UCSZ12);
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
	return 1;
};

int initASYNC2(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UBRR2L = (unsigned char)(UART.ubrr>>8);
	UBRR2H = (unsigned char)UART.ubrr;
	UCSR2A |= (1<<RXC2);
	UCSR2B |= (1<<RXCIE2)|(1<<RXEN2)|(1<<TXEN2)|(1<<UCSZ22);
	UCSR2C |= (1<<UCSZ21)|(1<<UCSZ20);
	return 1;
};

int initASYNC3(int rate){ //UARTBaudRegList is a list of register, see define.c and define.h
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

int init(int mode, int baudRate, int UARTId){
	if(UARTId > 3){
		return 0;
	}
	sei();
	UART.UARTReg = UARTId;
	int (*initAsyncFuncs[4])(int rate) = {initASYNC0, initASYNC1, initASYNC2, initASYNC3};
	(*initAsyncFuncs[UARTId])(baudRate);
	return 1;
}

int transmitChar0(char transmitionData){
	//while(~(UCSR0A & (1<<UDRE0))){}
	UCSR0B &= ~(1<<TXB80);
	if(transmitionData & 0x0100){
		UCSR0B |= (1<<TXB80);
	}
	UDR0 = transmitionData;
	return 1;
};

int transmitChar1(char transmitionData){
	//while(~(UCSR1A & (1<<UDRE1))){}
	UCSR1B &= ~(1<<TXB81);
	if(transmitionData & 0x0100){
		UCSR1B |= (1<<TXB81);
	}
	UDR1 = transmitionData;
	return 1;
};

int transmitChar2(char transmitionData){
	//while(~(UCSR2A & (1<<UDRE2))){}
	UCSR2B &= ~(1<<TXB82);
	if(transmitionData & 0x0100){
		UCSR2B |= (1<<TXB82);
	}
	UDR2 = transmitionData;
	return 1;
};

int transmitChar3(char transmitionData){
	//while(~(UCSR3A & (1<<UDRE3))){}
	UCSR3B &= ~(1<<TXB83);
	if(transmitionData & 0x0100){
		UCSR3B |= (1<<TXB83);
	}
	UDR3 = transmitionData;
	return 1;
};

int transmitChar(char transmitionData){
	int (*transmitCharFuncs[4])(char data) = {transmitChar0, transmitChar1, transmitChar2, transmitChar3};
	(*transmitCharFuncs[UART.UARTReg])(transmitionData);
	return 1;
}

int receiveChar0(){
	UART.RXh = UCSR0B;
	UART.RXl = UDR0;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int receiveChar1(){
	UART.RXh = UCSR1B;
	UART.RXl = UDR1;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int receiveChar2(){
	UART.RXh = UCSR2B;
	UART.RXl = UDR2;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int receiveChar3(){
	UART.RXh = UCSR3B;
	UART.RXl = UDR3;
	UART.RXh = (UART.RXh >> 1) & 0x01;
	UART.RX = ((UART.RXh << 8) | UART.RXl);
	return 1;
}

int receiveChar(){
	int(*receiveCharFuncs[4])() = {receiveChar0, receiveChar1, receiveChar2, receiveChar3};
	(*receiveCharFuncs[UART.UARTReg])();
	return 1;
}

struct uart UART = {
	.init = init,
	.transmitChar = transmitChar,
};

ISR(USART0_RX_vect){
	receiveChar();
};

ISR(USART1_RX_vect){
	receiveChar();
};

ISR(USART2_RX_vect){
	receiveChar();
};

ISR(USART3_RX_vect){
	receiveChar();
};