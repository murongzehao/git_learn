#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include	"STC8H_PWM.h"


PWMx_Duty dutyA;

// frequency 频率  （>= 367Hz）
#define	FREQ		1000

// period 周期  [0, 65535]
#define PERIOD (MAIN_Fosc / FREQ)


#define LED_SW		P45
#define LED1			P27
#define LED2			P26
#define LED3			P15
#define LED4			P14
#define LED5			P23
#define LED6			P22
#define LED7			P21
#define LED8			P20


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_5 | GPIO_Pin_4;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//初始化
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

void	PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;
    // 配置PWM1
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyA.PWM1_Duty;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO1N | ENO1P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM1, &PWMx_InitStructure);

    // 配置PWM2
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyA.PWM2_Duty;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO2N | ENO2P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM2, &PWMx_InitStructure);

    // 配置PWM3
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyA.PWM3_Duty;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO3N | ENO3P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM3, &PWMx_InitStructure);


    // 配置PWM4
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE2;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyA.PWM4_Duty;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO4N | ENO4P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM4, &PWMx_InitStructure);

    // 配置PWMA
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;			  //周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

    // 切换PWM通道
    PWM1_SW(PWM1_SW_P20_P21);			//PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
    PWM2_SW(PWM2_SW_P22_P23);			//PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
    PWM3_SW(PWM3_SW_P14_P15);			//PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
    PWM4_SW(PWM4_SW_P26_P27);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

    // 初始化PWMA的中断
    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}


int duty_percent = 0; // [0, 100]

void on_uart1_recv() {
    // RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
    u8 dat = RX1_Buffer[0];

    if (dat == 0x00) {
        duty_percent -= 1;
    } else if( dat == 0x01) {
        duty_percent += 1;
    }

    //限制duty_percent的范围
    if(duty_percent > 100) duty_percent = 100;
    else if(duty_percent < 0) duty_percent = 0;
    printf("duty_percent:%d\n",duty_percent);

    //修改占空比
    dutyA.PWM1_Duty = PERIOD * duty_percent / 100 ;
    dutyA.PWM2_Duty = PERIOD * duty_percent / 100 ;
    dutyA.PWM3_Duty = PERIOD * duty_percent / 100 ;
    dutyA.PWM4_Duty = PERIOD * duty_percent / 100 ;
    UpdatePwm(PWMA, &dutyA);

}
void main() {
    u8 dat = RX1_Buffer[0];

    EAXSFR();
    GPIO_config();
    UART_config();
    PWM_config();

    //设置占空比
//    dutyA.PWM1_Duty = PERIOD * 0;
//    dutyA.PWM2_Duty = PERIOD * 0;
//    dutyA.PWM3_Duty = PERIOD * 0;
//    dutyA.PWM4_Duty = PERIOD * 0;

    //全局中断
    EA = 1;

    //开启总开关
    LED_SW = 0;
    while(1) {

        if(COM1.RX_TimeOut > 0) {
            //超时计数
            if(--COM1.RX_TimeOut == 0) {
                if(COM1.RX_Cnt > 0) {
                    if (dat == 0x00) {
                        duty_percent -= 1;
                    } else if( dat == 0x01) {
                        duty_percent += 1;
                    }

                    //限制duty_percent的范围
                    if(duty_percent > 100) duty_percent = 100;
                    else if(duty_percent < 0) duty_percent = 0;
                    printf("duty_percent:%d\n",duty_percent);

                    //修改占空比
                    dutyA.PWM1_Duty = PERIOD * duty_percent / 100 ;
                    dutyA.PWM2_Duty = PERIOD * duty_percent / 100 ;
                    dutyA.PWM3_Duty = PERIOD * duty_percent / 100 ;
                    dutyA.PWM4_Duty = PERIOD * duty_percent / 100 ;
                    UpdatePwm(PWMA, &dutyA);

                }
                COM1.RX_Cnt = 0;
            }


        }
        // 不要处理的太快
        delay_ms(1);
    }
}