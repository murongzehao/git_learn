#include "config.h"
#include "delay.h"
#include "gpio.h"

/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
  GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

  GPIO_InitStructure.Pin  = GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
  GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//��ʼ��
}


void main(){
	GPIO_config();

while(1){
	P01 = 1;
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	P01 = 0;
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);

}
}