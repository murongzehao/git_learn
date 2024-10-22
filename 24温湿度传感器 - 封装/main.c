#include "config.h"
#include "Delay.h"
#include "DHT11.H"
#include "NVIC.h"
#include "UART.h"
#include "switch.h"

#define DHT	P46

/*
读取温湿度传感器DHT11温湿度值
使用UART1收到任何数据时，读数据




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





void on_uart1_recv() {
    float humidity, temperature;
    DHT11_get_info(&humidity,&temperature);
    printf("湿度：%.1f%%  温度：%.1f℃\n",humidity,temperature);
}

void main() {
    EAXSFR();
    UART_config();
    DHT11_init();


    EA = 1;

    while(1) {
        //调用串口接收数据判断
        //如果 RX_TimeOut 大于 0，表示接收超时计时器正在运行。
        //检查递减后的 RX_TimeOut 是否等于 0。
        //如果等于 0，表示超时计时器已经倒计时完成。说明已经接收完成
        if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
            //如果 RX_Cnt 大于 0，表示接收到的数据字节数大于 0。
            if(COM1.RX_Cnt > 0) {
                on_uart1_recv();
            }
            //清除数据
            COM1.RX_Cnt = 0;
        }

        delay_ms(10);
    }
}