#ifndef DEFINE_H_
#define DEFINE_H_

#define UBBR 0
#define L 0
#define H 1
#define UCSRA 1
#define UCSRB 2
#define UCSRC 3
#define TXEN 4
#define RXEN 5
#define UCSZ 6
#define UDR 7
#define TXB8 8
#define RXCIE 9
#define TXCIE 10
#define RXC 11
#define TXC 12



int UARTBaudRateRegList[13][4][3];
void importRegs();

#endif /* DEFINE_H_ */