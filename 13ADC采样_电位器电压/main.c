#include "config.h"
#include "GPIO.H"
#include "ADC.H"
#include "Delay.H"
#include "NVIC.H"
#include "UART.H"
#include "switch.h"


void	GPIO_config(void)
{

    //P05初始化
    P0_MODE_IN_HIZ(GPIO_Pin_5);
    //P30 P31 初始化
    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

/******************* AD配置函数 *******************/
void	ADC_config(void)
{
    ADC_InitTypeDef		ADC_InitStructure;		//结构定义

    ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
    ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);		//初始化
    ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
    NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
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
    u16 adc_value;
    float rst_V;

    EAXSFR();

    GPIO_config();
    ADC_config();
    UART_config();

    EA = 1;


    while(1) {
        adc_value =Get_ADCResult(ADC_CH13);

        printf("ADC:%d  ",adc_value);

        rst_V = adc_value * 2.5 / 4096;//输入电压
			
			printf("输入电压为：%.3fV",rst_V);

        delay_ms(250);
        delay_ms(250);

    }
}