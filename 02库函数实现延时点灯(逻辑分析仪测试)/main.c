#include "Config.h"
#include "STC8G_H_GPIO.h"
#include "STC8G_H_Delay.h"


void GPIO_config(){
	GPIO_InitTypeDef GPIOx; 
	GPIOx.Mode = GPIO_PullUp;
	GPIOx.Pin = GPIO_Pin_3;
	GPIO_Inilize(GPIO_P5,&GPIOx);
}

void main(){
	//∑Ω Ω1£∫
//	P5_MODE_OUT_PP(GPIO_Pin_3);
//	P5M1 &= ~(0x08), P5M0 |= (0x08);
	GPIO_config();

	while(1){
	//	¡¡µ∆
	P53 = 1;
	delay_ms(250);
	delay_ms(250);

//	œ®µ∆
	P53 = 0;
	delay_ms(250);
	delay_ms(250);
	}


}