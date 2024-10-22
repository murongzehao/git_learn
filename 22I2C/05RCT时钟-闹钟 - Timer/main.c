#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"
#include "RTCTIME.h"
#include "I2C.h"
#include "Exti.h"
#include "NVIC.h"

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_7;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化
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

    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


/******************** INT配置 ********************/
void	Exti_config(void)
{
    EXTI_InitTypeDef	Exti_InitStructure;							//结构定义

    Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
    Ext_Inilize(EXT_INT3,&Exti_InitStructure);				//初始化
    NVIC_INT3_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}



void TRC_on_alarm() {

    printf("Alarm\n");

}

void TRC_on_timer() {

    printf("Timer\n");
}






void main() {
    Clock_t c;
    alarm a;

    EAXSFR();

    GPIO_config();
    UART_config();
    Exti_config();
    RCT_TIME_init();
    EA = 1;


    delay_ms(200);

    // 提前准备时间，写1次时间 -------------------------------------
    // 准备 秒、分、时、天、周、世纪&月、年
    c.year = 2024, c.month = 8, c.day = 12, c.week = 1;
    c.hour = 23, c.minute = 59, c.second = 55;
    RCT_TIME_set_clock(c);

    // 设置闹铃 0x09--------------------------------------------------------
    a.minute = 0, a.hour = 0, a.day = 13, a.week = 2;
    set_alarm(a);

    // 启用闹铃
    get_alarm(ENABLE);

// 设置定时器 0E的频率和0F的值
    set_Timer(HZ64, 64);

// 启用定时器
    get_Timer(ENABLE);


    while(1) {

        // 循环读取数据: 秒、分、时、天、周、月、年、世纪
        RCT_TIME_get_clock(&c);

        // 打印数据
        printf("%02d-%02d-%02d ", (int)c.year, (int)c.month, (int)c.day);
        printf("%02d:%02d:%02d ", (int)c.hour, (int)c.minute, (int)c.second);
        printf("week->%d\n", (int)c.week);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);


    }
}
