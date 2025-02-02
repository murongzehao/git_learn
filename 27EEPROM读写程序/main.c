#include "Config.h"
#include "Delay.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"

#include <string.h>

#include "EEPROM.h"

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

#define MAX_LENGTH 100
u8 xdata tmp[MAX_LENGTH];

void main() {
    u16 address = 0x0004;
    char* str = "Hello!";
    u16 str_length = strlen(str);

    EAXSFR();

    UART_config();

    EA = 1;

    //擦除数据
    EEPROM_SectorErase(address);

    //写入数据
    EEPROM_write_n(address,str,str_length);

    //读取数据
    EEPROM_read_n(address,tmp,str_length);
	
		//添加字符串结束标志
	tmp[str_length] = '\0';


    printf(">>存储的字符串：%s\n",str);

    printf(">>读取的字符串：%s\n",tmp);

    if(strcmp(str,tmp) == 0) {
        printf("字符串相同");
    } else {
        printf("字符串不同");
    }


    UART_config();


    while(1);
}