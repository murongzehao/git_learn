#include "config.h"
#include "delay.h"
#include "gpio.h"

#define IO1  P20
#define IO2  P21
#define IO3  P22
#define IO4  P23
#define IO5  P26

#define PIN1 GPIO_Pin_0
#define PIN2 GPIO_Pin_1
#define PIN3 GPIO_Pin_2
#define PIN4 GPIO_Pin_3
#define PIN5 GPIO_Pin_6



#define OUT1(LEVEL) P2_MODE_IO_PU(PIN1); IO1=LEVEL;
#define OUT2(LEVEL) P2_MODE_IO_PU(PIN2); IO2=LEVEL;
#define OUT3(LEVEL) P2_MODE_IO_PU(PIN3); IO3=LEVEL;
#define OUT4(LEVEL) P2_MODE_IO_PU(PIN4); IO4=LEVEL;
#define OUT5(LEVEL) P2_MODE_IO_PU(PIN5); IO5=LEVEL;
#define HIZ(pin)		P2_MODE_IN_HIZ(pin)
#define HIZ_ALL()		HIZ(PIN1 | PIN2 | PIN3 | PIN4 | PIN5)

void test_hundred() {
    HIZ_ALL();
    OUT2(1);
    OUT3(1);
    OUT4(0);
}

void main() {

    EA = 1;
HIZ_ALL();
	
	OUT2(1);
	OUT



    while(1);
}