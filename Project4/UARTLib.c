#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "define.h"

int initASYNC(int rate, int UARTReg, int UARTBaudRegList[9][4][3]){ //UARTBaudRegList is a list of register, see define.c and define.h
	UART.ubrr = F_CPU/(16*rate)-1;
	UARTBaudRegList[UBBR][UARTReg][L] = (unsigned char)(UART.ubrr>>8); 
	UARTBaudRegList[UBBR][UARTReg][H] = (unsigned char)UART.ubrr; // Sets baud rate
	UARTBaudRegList[UCSRB][UARTReg][0] = (1<<UARTBaudRegList[TXEN][UARTReg][0])|(1<<UARTBaudRegList[RXEN][UARTReg][0]); //Enables transmit and recieve bit in register
	UARTBaudRegList[UCSRC][UARTReg][0] = (1<<UARTBaudRegList[UCSZ][UARTReg][0])|(1<<UARTBaudRegList[UCSZ][UARTReg][1])|(1<<UARTBaudRegList[UCSZ][UARTReg][2]); //Sets data length
	return 1;
};

int initASYNC2X(int rate, int UARTReg, int UARTBaudRegList[9][4][3]){
	UART.ubrr = F_CPU/(8*rate)-1;
	return 1;
};

int initMASTER(int rate, int UARTReg, int UARTBaudRegList[9][4][3]){
	return 1;
};

int initSLAVE(int rate, int UARTReg, int UARTBaudRegList[9][4][3]){
	return 1;
};

int init(int mode, int baudRate, int UARTId){
	if(UARTId > 3){
		return 0;
	}
	importRegs();
	UART.UARTReg = UARTId;
	int (*initFuncs[4])(int rate, int UARTReg, int UARTBaudRegList[9][4][3]) = {initASYNC, initASYNC2X, initMASTER, initSLAVE};
	if((*initFuncs[mode])(baudRate, UARTId, UARTBaudRateRegList)){
		
	}
	return 1;
}

void transmit(char TransmitionData){
	while(!(UARTBaudRateRegList[UCSRA][UART.UARTReg][0] & 0b00100000)){} //Waits for transmition end
	UARTBaudRateRegList[UCSRB][UART.UARTReg][0] &= ~(1<<UARTBaudRateRegList[TXB8][UART.UARTReg][0]);
	if(TransmitionData & 0x0100){
		UARTBaudRateRegList[UCSRB][UART.UARTReg][0] |= (1<<UARTBaudRateRegList[TXB8][UART.UARTReg][0]);
	}
	UARTBaudRateRegList[UDR][UART.UARTReg][0] = TransmitionData;
};

struct uart UART = {
	.init = init,
	.transmit = transmit
};