#ifndef _NIXIE_H_
#define _NIXIE_H_

#include "config.h"
#include "GPIO.h"

#define NIXIE_GPIO_init()  P4_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_2 | GPIO_Pin_3)


#define NIX_DI   P44
#define NIX_SCK  P42
#define NIX_RCK  P43

void NIXIE_init();

void NIXIE_show(u8 a_num,u8 b_dig);
#endif