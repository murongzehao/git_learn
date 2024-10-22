#include "config.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "Delay.h"
#include "RTCTIME.h"

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
#define NUMBER 7

void main() {
    Clock_t c;
//    u8 dev_addr = 0x51 << 1; //A2 设备写地址
//    u8 mem_addr = 0x02;			// 这个芯片设定的秒地址是02
    u8 p[NUMBER] = {0};

    EAXSFR();//扩展寄存器使能

    UART_config();

    RCT_TIME_init();


    EA = 1;
    c.second = 00,  c.minute = 8, c.hour = 21;
    c.week = 6, c.day = 10, c.month = 8,c.year = 2024;
    RCT_TIME_set_clock(c);




    while(1) {
			
        RCT_TIME_get_clock(&c);//调用指针是传地址

        printf("%02d时:%02d分:%02d秒\n",(int)c.hour,(int)c.minute,(int)c.second);
        printf("%02d/%02d/%02d\n",(int)c.year,(int)c.month,(int)c.day);
        printf("week:%d\n",(int)c.week);
        printf("----------------------------");
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);


    }
}