#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "switch.h"


#define KEY1 P51
#define KEY2 P52

#define a  P00
#define MOTOR  P01

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//初始化
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			  //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				  //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			  //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		  //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


void main() {
    EAXSFR();
    GPIO_config();
    UART_config();



EA = 1;


    while(1) {
        if(KEY1 == 1) {
            MOTOR = 0;
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
            printf("KEY1 Up\n");

        }
        else if(KEY1 == 0) {
            MOTOR = 1;
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
            printf("KEY1 Dowm\n");

        }



    }
}