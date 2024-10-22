#include "config.h"
#include "GPIO.h"
#include "Delay.h"
#include "uart.h"
#include "nvic.h"
#include "switch.h"

#define NIX_DI   P44
#define NIX_SCK  P42
#define NIX_RCK  P43

#define KEY1     P51
#define KEY2     P52
#define KEY3     P53
#define KEY4     P54

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//初始化
	
	   GPIO_InitStructure.Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//初始化
}

#define GET_

void main(){
	
	GPIO_config();

while(1);
}