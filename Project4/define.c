#include "define.h"
#include <avr/io.h>

void importRegs(){
	int UARTBaudRateRegList[13][4][3] = {{{UBRR0L, UBRR0H},{UBRR1L, UBRR1H},{UBRR2L, UBRR2H},{UBRR3L, UBRR3H}},
	{{UCSR0A}, {UCSR1A}, {UCSR2A}, {UCSR3A}},
	{{UCSR0B}, {UCSR1B}, {UCSR2B}, {UCSR3B}},
	{{UCSR0C}, {UCSR1C}, {UCSR2C}, {UCSR3C}},
	{{TXEN0}, {TXEN1}, {TXEN2}, {TXEN3}},
	{{RXEN0}, {RXEN1}, {RXEN2}, {RXEN3}},
	{{UCSZ00, UCSZ01, UCSZ02}, {UCSZ10, UCSZ11, UCSZ12}, {UCSZ20 ,UCSZ21 ,UCSZ22}},
	{{UDR0}, {UDR1}, {UDR2}, {UDR3}},
	{{TXB80}, {TXB81}, {TXB82}, {TXB83}},
	{{RXCIE0}, {RXCIE1}, {RXCIE2}, {RXCIE3}},
	{{TXCIE0}, {TXCIE1}, {TXCIE2}, {TXCIE3}},
	{{RXC0}, {RXC1}, {RXC2}, {RXC3}},
	{{TXC0}, {TXC1}, {TXC2}, {TXC3}}};
};