#ifndef __NTC_H__
#define __NTC_H__

#include "Config.h"

// GPIO端口
#define NTC_GPIO			GPIO_P0
// GPIO引脚
#define NTC_GPIO_PIN	GPIO_Pin_4
// ADC通道
#define NTC_ADC_CH		ADC_CH12


void NTC_init(void);

int NTC_get_temperature(void);
#endif