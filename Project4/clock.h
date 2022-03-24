#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver

struct clock{
	volatile int counter;
	volatile int interruptFlag;
	int second;
	int minute;
	int hour;
	int (*clock_enableTimerInterrupt)();
	int (*clock_init)();
	int (*clock_increment)();
};

extern struct clock CLOCK;

#endif /* CLOCK_H_ */