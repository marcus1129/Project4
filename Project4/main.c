/*
 * Project4.c
 *
 * Created: 3/10/2022 3:05:52 PM
 * Author : Marcus Korre
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "UARTLib.h"
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver
#include <util/delay.h>

int main(void)
{
	_i2c_address = 0X78;
	I2C_Init();
	InitializeDisplay();
	print_fonts();
	clear_display();
	UART.init(ASYNC, 19200, 1);
    while (1){
		UART.transmitChar('s');
		clear_display();
		sendCharXY(UART.RX, 0, 0);
		_delay_ms(1000);
    }
}

