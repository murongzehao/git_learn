#ifndef  __DHT11_H_
#define  __DHT11_H_

#include "config.h"

#define DHT	P46

#define DHT_PORT   GPIO_P4 
#define DHT_PIN    GPIO_Pin_6

void DHT11_init();

char DHT11_get_info(float * humidity,float * temperature);



#endif