#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "define.h"

int initASYNC(int rate, int UARTReg, int UARTBaudRegList[6][4][2]){
	UART.ubrr = F_CPU/(16*rate)-1;
	UARTBaudRegList[0][UARTReg][0] = (unsigned char)(UART.ubrr>>8); //List of registers, see init() for the list;
	UARTBaudRegList[0][UARTReg][1] = (unsigned char)UART.ubrr;
	UARTBaudRegList[2][UARTReg][0] = (1<<UARTBaudRegList[4][UARTReg][0])|(1<<UARTBaudRegList[5][UARTReg][0]);
	return 1;
};

int initASYNC2X(int rate, int UARTReg, int UARTBaudRegList[6][4][2]){
	UART.ubrr = F_CPU/(8*rate)-1;
	return 1;
};

int initMASTER(int rate, int UARTReg, int UARTBaudRegList[6][4][2]){
	return 1;
};

int initSLAVE(int rate, int UARTReg, int UARTBaudRegList[6][4][2]){
	return 1;
};

int init(int mode, int baudRate, int UARTId){
	if(UARTId > 3){
		return 0;
	}
	int (*initFuncs[4])(int rate, int UARTReg, int UARTBaudRegList[6][4][2]) = {initASYNC, initASYNC2X, initMASTER, initSLAVE};
	if((*initFuncs[mode])(baudRate, UARTId, UARTBaudRateRegList)){
		
	}
	return 1;
}

void transmit(){
	
};

struct uart UART = {
	.init = init,
	.transmit = transmit
};