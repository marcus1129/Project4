#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "clock.h"


int clock_init(){
	CLOCK.second = 0;
	CLOCK.minute = 0;
	CLOCK.hour = 0;
	CLOCK.counter = 0;
	CLOCK.interruptFlag = 1;
	CLOCK.clock_makeTimeStr();
	return 1;
}

int clock_increment(){
	if(CLOCK.second == 59){
		if(CLOCK.minute == 59){
			if(CLOCK.hour == 23){
				CLOCK.hour = 0;
			}
			else{
				CLOCK.hour += 1;
			}
			CLOCK.minute = 0;
		}
		else{
			CLOCK.minute += 1;
		}
		CLOCK.second = 0;
	}
	else{
		CLOCK.second += 1;
	}
	CLOCK.clock_makeTimeStr();
	return 1;
}

int clock_makeTimeStr(){
	if(CLOCK.timeStr == NULL){
		CLOCK.timeStr = (char*)calloc(10, sizeof(char));
	}
	char tempHour[2];
	char tempMinute[2];
	char tempSecond[2];
	if(CLOCK.hour >= 10){
		tempHour[1] = CLOCK.hour%10 + '0';
		tempHour[0] = (CLOCK.hour/10)%10 + '0';
	} else{
		tempHour[1] = CLOCK.hour%10 + '0';
		tempHour[0] = '0';
	}
	if(CLOCK.minute >= 10){
		tempMinute[1] = CLOCK.minute%10 + '0';
		tempMinute[0] = (CLOCK.minute/10)%10 + '0';
		} else{
		tempMinute[1] = CLOCK.minute%10 + '0';
		tempMinute[0] = '0';
	}
	if(CLOCK.second >= 10){
		tempSecond[1] = CLOCK.second%10 + '0';
		tempSecond[0] = (CLOCK.second/10)%10 + '0';
		} else{
		tempSecond[1] = CLOCK.second%10 + '0';
		tempSecond[0] = '0';
	}
	CLOCK.timeStr[0] = tempHour[0];
	CLOCK.timeStr[1] = tempHour[1];
	CLOCK.timeStr[2] = ':';
	CLOCK.timeStr[3] = tempMinute[0];
	CLOCK.timeStr[4] = tempMinute[1];
	CLOCK.timeStr[5] = ':';
	CLOCK.timeStr[6] = tempSecond[0];
	CLOCK.timeStr[7] = tempSecond[1];
	CLOCK.timeStr[8] = '\n';
	CLOCK.timeStr[9] = '\r';
	return 1;
}

int initTimer0(){
	TCCR0A = (1<<WGM01);	     //Sets mode to CTC
	TCCR0B = (1<<CS01)|(1<<CS00);		//Sets prescaler to 1024
	OCR0A =	0xF9;				//Sets compare value to 249
	OCR0B =	0xF9;
	TIMSK0 = (1<<OCIE0A);
	TIFR0 = (1<<OCF0A);
	return 1;
};

/*int initTimer1(){
	TCCR1A = 0x00;	     //Sets mode to CTC
	TCCR1B = (1<<CS12)|(1<<CS10);		//Sets prescaler to 1024
	OCR1AH = 0x00;
	OCR1AL = 0x9b;				//Sets compare value to 15600
	OCR1B =	0x9b;
	OCR1C =	0x9b;
	TIMSK1 = (1<<OCIE1A);
	TIFR1 = (1<<OCF1A);
	return 1;
};

int initTimer2(){
	TCCR2A = (1<<WGM21);	     //Sets mode to CTC
	TCCR2B = (1<<CS22)|(1<<CS20);		//Sets prescaler to 1024
	OCR2A =	0x9b;				//Sets compare value to 15600
	OCR2B =	0x9b;
	TIMSK2 = (1<<OCIE2A);
	TIFR2 = (1<<OCF2A);
	return 1;
};

int initTimer3(){
	TCCR3A = 0x00;	     //Sets mode to CTC
	TCCR3B = (1<<CS32)|(1<<CS30);		//Sets prescaler to 1024
	OCR3A =	0x009b;				//Sets compare value to 15600
	OCR3B =	0x009b;
	OCR3C =	0x009b;
	TIMSK3 = (1<<OCIE3A);
	TIFR3 = (1<<OCF3A);
	return 1;
};

int initTimer4(){
	TCCR4A = 0x00;	     //Sets mode to CTC
	TCCR4B = (1<<CS42)|(1<<CS40);		//Sets prescaler to 1024
	OCR4A =	0x009b;				//Sets compare value to 15600
	OCR4B =	0x009b;
	OCR4C =	0x009b;
	TIMSK4 = (1<<OCIE4A);
	TIFR4 = (1<<OCF4A);
	return 1;
};*/

int clock_enableTimerInterrupt(int timerId){
	sei();
	int (*clock_interruptInitFuncs[5])() = {initTimer0/*, initTimer1, initTimer2, initTimer3, initTimer4*/};
	(*clock_interruptInitFuncs[timerId])();
	return 1;
}

int clock_updateClock(char* clockVal){
	char buffer[2];
	buffer[0] = clockVal[0];
	buffer[1] = clockVal[1];
	CLOCK.hour = atoi(buffer);
	buffer[0] = clockVal[3];
	buffer[1] = clockVal[4];
	CLOCK.minute = atoi(buffer);
	buffer[0] = clockVal[6];
	buffer[1] = clockVal[7];
	CLOCK.second = atoi(buffer);
	return 1;
}

struct clock CLOCK = {
	.clock_init = clock_init,
	.clock_increment = clock_increment,
	.clock_enableTimerInterrupt = clock_enableTimerInterrupt,
	.clock_makeTimeStr = clock_makeTimeStr,
	.clock_updateClock = clock_updateClock
};


ISR(TIMER0_COMPA_vect){
	CLOCK.counter += 1;
	if(CLOCK.counter == 1000){
		CLOCK.clock_makeTimeStr();
		CLOCK.clock_increment();
		CLOCK.interruptFlag = 1;
		CLOCK.counter = 0;
	}
}

ISR(TIMER1_COMPA_vect){
	CLOCK.counter += 1;
	if(CLOCK.counter == 1000){
		CLOCK.clock_makeTimeStr();
		CLOCK.clock_increment();
		CLOCK.interruptFlag = 1;
		CLOCK.counter = 0;
	}
}

ISR(TIMER2_COMPA_vect){
	CLOCK.counter += 1;
	if(CLOCK.counter == 1000){
		CLOCK.clock_makeTimeStr();
		CLOCK.clock_increment();
		CLOCK.interruptFlag = 1;
		CLOCK.counter = 0;
	}
}

ISR(TIMER3_COMPA_vect){
	CLOCK.counter += 1;
	if(CLOCK.counter == 1000){
		CLOCK.clock_makeTimeStr();
		CLOCK.clock_increment();
		CLOCK.interruptFlag = 1;
		CLOCK.counter = 0;
	}
}

ISR(TIMER4_COMPA_vect){
	CLOCK.counter += 1;
	if(CLOCK.counter == 1000){
		CLOCK.clock_makeTimeStr();
		CLOCK.clock_increment();
		CLOCK.interruptFlag = 1;
		CLOCK.counter = 0;
	}
}