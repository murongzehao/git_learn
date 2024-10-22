#ifndef __NTC_H__
#define __NTC_H__

#include "Config.h"

// GPIO�˿�
#define NTC_GPIO			GPIO_P0
// GPIO����
#define NTC_GPIO_PIN	GPIO_Pin_4
// ADCͨ��
#define NTC_ADC_CH		ADC_CH12


void NTC_init(void);

int NTC_get_temperature(void);
#endif