#ifndef DEFINE_H_
#define DEFINE_H_

#include <avr/io.h>

int UARTBaudRateRegList[6][4][2];

UARTBaudRateRegList =	{{{UBRR0L, UBRR0H},{UBRR1L, UBRR1H},{UBRR2L, UBRR2H},{UBRR3L, UBRR3H}},
						{{UCSR0A}, {UCSR1A}, {UCSR2A}, {UCSR3A}},
						{{UCSR0B}, {UCSR1B}, {UCSR2B}, {UCSR3B}},
						{{UCSR0C}, {UCSR1C}, {UCSR2C}, {UCSR3C}},
						{{TXEN0}, {TXEN1}, {TXEN2}, {TXEN3}},
						{{RXEN0}, {RXEN1}, {RXEN2}, {RXEN3}}};
#endif /* DEFINE_H_ */