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



int UARTBaudRateRegList[96][4][3];
void importRegs();

#endif /* DEFINE_H_ */