#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include	"STC8H_PWM.h"

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//初始化
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


/********************本地变量声明****************/
PWMx_Duty dutyB;

//频率
#define FREQ  1000
//1个周期的计数值   24000 000 / 1000 = 24 000
#define PERIOD  (MAIN_Fosc / FREQ)

void	PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;

    // 配置PWM
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= 0;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO6P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM6, &PWMx_InitStructure);

    // 配置PWMB
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;			  //周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

    // 切换PWM通道
    PWM6_SW(PWM6_SW_P01);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

    // 初始化PWMA的中断
    NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}



void main() {
	u8 duty_percent = 0;
	char dir = 1;
    EAXSFR();

    GPIO_config();
    UART_config();
    PWM_config();
	
		EA = 1;

    while(1){
			duty_percent += dir;
			if(duty_percent >= 100 ){
				dir = -1;
			}else if(duty_percent <= 0){
				dir = 1;
			}
			
			dutyB.PWM6_Duty = PERIOD * duty_percent / 100;
			UpdatePwm(PWM6,&dutyB);
			
		delay_ms(10);
		}
}