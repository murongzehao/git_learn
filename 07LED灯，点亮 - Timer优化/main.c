#include "config.h"
#include	"GPIO.h"
#include	"UART.h"
#include	"Delay.h"
#include	"NVIC.h"
#include	"Switch.h"
#include "Timer.h"



#define LED_SW 			P45
#define LED1   			P27
#define LED2   			P26
#define LED3   			P15
#define LED4   			P14
#define LED5   			P23
#define LED6   			P22
#define LED7   			P21
#define LED8   			P20

void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    // LED_SW
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化

    // P2
    GPIO_InitStructure.Pin  = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化

    //P1
    GPIO_InitStructure.Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


/***************  定时器初始化函数 *****************/
void	Timer_config(void)
{
    TIM_InitTypeDef		TIM_InitStructure;						//结构定义
    //定时器0做16位自动重装, 中断频率为1000HZ
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//初值,
    TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

// 全局变量
u8 temp = 2;

void timer0_call() {
    //循环在这里判断并处理接收到的数据
    if(COM1.RX_TimeOut > 0 &&  --COM1.RX_TimeOut == 0)//RX_TimeOut 接收超时
    {
        if(COM1.RX_Cnt > 0)//判断是否有数据
        {
            //RX1_Buffer[i]存的是接受的每个字节，写出用TX1_write2buff
            if(RX1_Buffer[0] == 0x00) { //第一个字节=0
                temp = 0;
            }
            else if (RX1_Buffer[0] == 0x01) {//第一个字节=1
                temp = 1;

            }
            //清零数据计数值
            COM1.RX_Cnt = 0;
        }



    }

}
/****************************
通过串口控制，实现左转弯流水灯和右转弯流水灯效果
  a. 串口发送数字0，左转弯
  b. 串口发送数字1，右转弯

1. Timer以1000Hz运行，在Timer中断里，判定串口接收数据
	- 收到数据0，将一个全局变量修改为0
	- 收到数据1，将一个全局变量修改为1

2. main函数的while循环，不断判断变量值
	- 根据此全局变量，进行流水灯操作
  - 将变量值恢复为2

****************************/


void main() {
    int8 i;

    GPIO_config();

    UART_config();

    Timer_config();

    EA = 1;

    // 总开关拉低-打开,pnp三极管构成，低电流导通，高电流截止，所以要拉低
    LED_SW = 0;

    // LED, 先全部关掉
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;


    while(1) {
        if( temp == 0x00) {
            temp = 0x02;// 恢复默认值，避免重复进入
            // 左转(右->左)
            // 大概要执行1000ms，不要在中断里做耗时操作
            // 右->左
            for(i = 0; i <= 8; i++)
            {
                if( i == 0) LED1 = 0;
                else if (i == 1) LED2 = 0;
                else if (i == 2) LED3 = 0;
                else if (i == 3) LED4 = 0;
                else if (i == 4) LED5 = 0;
                else if (i == 5) LED6 = 0;
                else if (i == 6) LED7 = 0;
                else if (i == 7) LED8 = 0;
                delay_ms(100);
            }
            delay_ms(200);
            LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

        }
        else if(temp == 0x01) {
            temp = 0x02;// 恢复默认值，避免重复进入
            // 右转(左->右)
            for(i = 7; i >= -1; i--)
            {
                if( i == 0) LED1 = 0;
                else if (i == 1) LED2 = 0;
                else if (i == 2) LED3 = 0;
                else if (i == 3) LED4 = 0;
                else if (i == 4) LED5 = 0;
                else if (i == 5) LED6 = 0;
                else if (i == 6) LED7 = 0;
                else if (i == 7) LED8 = 0;
                delay_ms(100);
            }
            delay_ms(200);
            LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

        }






    }

}

