#ifndef UARTLIB_H_
#define UARTLIB_H_

#define ASYNC 0
#define ASYNC2X 1
#define USARTMASTER 2
#define USARTSLAVE 3
#define F_CPU 16000000UL

//struct uart{
	extern int ubrr;
	extern int UARTReg;
	extern char buffer[10];
	extern char timeVal[10];
	extern char RXh;
	extern char RXl;
	extern char RX;
	extern int UART_init(int mode, int baudRate, int UARTId);
	extern int UART_transmitChar(char transmitionData);
	extern int UART_transmitStr(char* transmitionData);
	/*int (*UART_init)(int mode, int baudRate, int UARTId);
	int (*UART_transmitChar)(char transmitionData);
	int (*UART_transmitStr)(char* transmitionData);*/
//};

//extern struct uart UART;



#endif /* UARTLIB_H_ */