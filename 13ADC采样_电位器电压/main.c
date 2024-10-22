#include "config.h"
#include "GPIO.H"
#include "ADC.H"
#include "Delay.H"
#include "NVIC.H"
#include "UART.H"
#include "switch.h"


void	GPIO_config(void)
{

    //P05��ʼ��
    P0_MODE_IN_HIZ(GPIO_Pin_5);
    //P30 P31 ��ʼ��
    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

/******************* AD���ú��� *******************/
void	ADC_config(void)
{
    ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����

    ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
    ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
    ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);		//��ʼ��
    ADC_PowerControl(ENABLE);				//ADC��Դ����, ENABLE��DISABLE
    NVIC_ADC_Init(DISABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//�ṹ����

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			  //������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				  //��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			  //�����ʼӱ�, ENABLE��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		  //��ʼ������1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

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

        rst_V = adc_value * 2.5 / 4096;//�����ѹ
			
			printf("�����ѹΪ��%.3fV",rst_V);

        delay_ms(250);
        delay_ms(250);

    }
}