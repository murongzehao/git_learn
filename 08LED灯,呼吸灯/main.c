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

/*************	 ���� GPIO ����	**************/
void GPIO_config(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    // LED_SW
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//��ʼ��

    // P2
    GPIO_InitStructure.Pin  = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//��ʼ��

    // P1
    GPIO_InitStructure.Pin  = GPIO_Pin_5 | GPIO_Pin_4;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//��ʼ��
}


/**************************���� UART***********************/
void UART_config(void) {
    // >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//�ṹ����
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/*************	���ر�������	**************/

PWMx_Duty dutyA;

// Ƶ��
#define FREQ		1000
//  1�����ڵļ���ֵ 24 000 000 / 1000  = 24 000
#define PERIOD	(MAIN_Fosc / FREQ)

/***************  ���ڳ�ʼ������ *****************/
void	PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;
    // Positive:����������
    // Negative:����������

    // P,N����ѡ��EnoSelectʱ��MODE1�����
    //        Duty���õ��Ǹߵ�ƽռ��

    // P,Nͬʱѡ��EnoSelectʱ��MODE1�����
    //  			P����:Duty���øߵ�ƽռ��
    //        N����:Duty���õ͵�ƽռ��
    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWMռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO1P | ENO1N;		    //  ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM1, &PWMx_InitStructure);

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWMռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO2P | ENO2N;		    //  ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM2, &PWMx_InitStructure);			  //��ʼ��PWM

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWMռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO3P | ENO3N;		    //  ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM3, &PWMx_InitStructure);			  //��ʼ��PWM

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE2;  // * ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  PERIOD * 0.0f; 	  // *  PWMռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_EnoSelect   =   ENO4P | ENO4N;		    //  ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM4, &PWMx_InitStructure);			  //��ʼ��PWM

    PWMx_InitStructure.PWM_Period   = PERIOD - 1;					// * ����ʱ�䣨1/Ƶ�ʣ�,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;						  //��������������, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//�����ʹ��, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

    PWM4_SW(PWM4_SW_P26_P27);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33
    PWM1_SW(PWM1_SW_P20_P21);
    PWM2_SW(PWM2_SW_P22_P23);
    PWM3_SW(PWM3_SW_P14_P15);

    // ����PWM�ж�
    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}
void main() {
    u8 duty_percent = 0;
    char direction = 1;

    EAXSFR();		/* ��չ�Ĵ�������ʹ�� */

    GPIO_config();

    UART_config();

    PWM_config();

    EA = 1;

    // ���ܿ���
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
        // ��duty_percent��[0, 100]����
        if (duty_percent >= 100) {
            direction = -1;
        } else if (duty_percent <= 0) {
            direction = 1;
        }

// �����µ�ռ�ձ�
				dutyA.PWM1_Duty = PERIOD * duty_percent / 100;
				dutyA.PWM2_Duty = PERIOD * duty_percent / 100;
				dutyA.PWM3_Duty = PERIOD * duty_percent / 100;
        dutyA.PWM4_Duty = PERIOD * duty_percent / 100;
        UpdatePwm(PWMA, &dutyA);

        delay_ms(10);
    }
}