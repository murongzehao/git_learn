#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include	"STC8H_PWM.h"

#define LED_SW		P45
#define LED1			P27
#define LED2			P26
#define LED3			P15
#define LED4			P14
#define LED5			P23
#define LED6			P22
#define LED7			P21
#define LED8			P20

/*************	 配置 GPIO 引脚	**************/
void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    // LED_SW
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化

    // P2
    GPIO_InitStructure.Pin  = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//初始化

    // P1
    GPIO_InitStructure.Pin  = GPIO_Pin_5 | GPIO_Pin_4;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化
}


/**************************配置 UART***********************/
void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/*************	本地变量声明	**************/

PWMx_Duty dutyA;

// 频率
#define FREQ		1000
//  1个周期的计数值 24 000 000 / 1000  = 24 000
#define PERIOD	(MAIN_Fosc / FREQ)

/***************  串口初始化函数 *****************/
void	PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;
    // Positive:积极，正极
    // Negative:消极，负极

    // P,N单独选择EnoSelect时，MODE1情况下
    //        Duty设置的是高电平占比

    // P,N同时选择EnoSelect时，MODE1情况下
    //  			P正极:Duty设置高电平占比
    //        N负极:Duty设置低电平占比
    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO1P | ENO1N;		    //  输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM1, &PWMx_InitStructure);

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO2P | ENO2N;		    //  输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM2, &PWMx_InitStructure);			  //初始化PWM

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO3P | ENO3N;		    //  输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM3, &PWMx_InitStructure);			  //初始化PWM

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO4P | ENO4N;		    //  输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM4, &PWMx_InitStructure);			  //初始化PWM

    PWMx_InitStructure.PWM_Period   = PERIOD - 1;					// * 周期时间（1/频率）,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;						  //死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

    PWM4_SW(PWM4_SW_P26_P27);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33
    PWM1_SW(PWM1_SW_P20_P21);
    PWM2_SW(PWM2_SW_P22_P23);
    PWM3_SW(PWM3_SW_P14_P15);

    // 禁用PWM中断
    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}
void main() {
    u8 duty_percent = 0;
    char direction = 1;

    EAXSFR();		/* 扩展寄存器访问使能 */

    GPIO_config();

    UART_config();

    PWM_config();

    EA = 1;

    // 打开总开关
    LED_SW = 0;

    // LED2, LED4
    LED2 = 0;
    LED1 = 1;
		LED3 = 1;
    LED4 = 0;
		LED5 = 1;
    LED6 = 0;
		LED7 = 1;
    LED8 = 0;

    while(1) {
        // 0 -> 100 -> 0 -> .....
        duty_percent += direction;
        // 让duty_percent在[0, 100]往返
        if (duty_percent >= 100) {
            direction = -1;
        } else if (duty_percent <= 0) {
            direction = 1;
        }

// 设置新的占空比
				dutyA.PWM1_Duty = PERIOD * duty_percent / 100;
				dutyA.PWM2_Duty = PERIOD * duty_percent / 100;
				dutyA.PWM3_Duty = PERIOD * duty_percent / 100;
        dutyA.PWM4_Duty = PERIOD * duty_percent / 100;
        UpdatePwm(PWMA, &dutyA);

        delay_ms(10);
    }
}