#ifndef UARTLIB_H_
#define UARTLIB_H_

#define ASYNC 0
#define ASYNC2X 1
#define USARTMASTER 2
#define USARTSLAVE 3
#define F_CPU 16000000UL

struct uart{
	int ubrr;
	int UARTReg;
	int (*init)(int mode, int baudRate, int UARTId);
	void (*transmit)(char TransmitionData);
};

extern struct uart UART;



#endif /* UARTLIB_H_ */