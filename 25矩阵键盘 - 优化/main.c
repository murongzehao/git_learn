#include "config.h"
#include "MatrixKeys.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"



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

void MK_on_keydown(u8 row, u8 col) {
    printf("第 %d 行 %d 列按钮dowm!\n",(int)row+1,(int)col+1);

}

void MK_on_keyup(u8 row,u8 col) {
    printf("第 %d 行 %d 列按钮Up!\n", (int)row+1, (int)col+1);
}


void MK_callback(u8 row,u8 col,u8 action) {
    if(action) {
        printf("第 %d 行 %d 列按钮dowm\n",(int)row+1,(int)col+1);
    } else {
        printf("第 %d 行 %d 列按钮Up\n", (int)row+1, (int)col+1);
    }


}
void main() {


    UART_config();

    MK_init();

    EA = 1;

    while(1) {
        MK_scan(MK_callback);
        delay_ms(10);


    }
}