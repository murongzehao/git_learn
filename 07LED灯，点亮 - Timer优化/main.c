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
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    // LED_SW
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//��ʼ��

    // P2
    GPIO_InitStructure.Pin  = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);//��ʼ��

    //P1
    GPIO_InitStructure.Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//�ṹ����

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
    NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


/***************  ��ʱ����ʼ������ *****************/
void	Timer_config(void)
{
    TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
    //��ʱ��0��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//��ֵ,
    TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

// ȫ�ֱ���
u8 temp = 2;

void timer0_call() {
    //ѭ���������жϲ�������յ�������
    if(COM1.RX_TimeOut > 0 &&  --COM1.RX_TimeOut == 0)//RX_TimeOut ���ճ�ʱ
    {
        if(COM1.RX_Cnt > 0)//�ж��Ƿ�������
        {
            //RX1_Buffer[i]����ǽ��ܵ�ÿ���ֽڣ�д����TX1_write2buff
            if(RX1_Buffer[0] == 0x00) { //��һ���ֽ�=0
                temp = 0;
            }
            else if (RX1_Buffer[0] == 0x01) {//��һ���ֽ�=1
                temp = 1;

            }
            //�������ݼ���ֵ
            COM1.RX_Cnt = 0;
        }



    }

}
/****************************
ͨ�����ڿ��ƣ�ʵ����ת����ˮ�ƺ���ת����ˮ��Ч��
  a. ���ڷ�������0����ת��
  b. ���ڷ�������1����ת��

1. Timer��1000Hz���У���Timer�ж���ж����ڽ�������
	- �յ�����0����һ��ȫ�ֱ����޸�Ϊ0
	- �յ�����1����һ��ȫ�ֱ����޸�Ϊ1

2. main������whileѭ���������жϱ���ֵ
	- ���ݴ�ȫ�ֱ�����������ˮ�Ʋ���
  - ������ֵ�ָ�Ϊ2

****************************/


void main() {
    int8 i;

    GPIO_config();

    UART_config();

    Timer_config();

    EA = 1;

    // �ܿ�������-��,pnp�����ܹ��ɣ��͵�����ͨ���ߵ�����ֹ������Ҫ����
    LED_SW = 0;

    // LED, ��ȫ���ص�
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;


    while(1) {
        if( temp == 0x00) {
            temp = 0x02;// �ָ�Ĭ��ֵ�������ظ�����
            // ��ת(��->��)
            // ���Ҫִ��1000ms����Ҫ���ж�������ʱ����
            // ��->��
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
            temp = 0x02;// �ָ�Ĭ��ֵ�������ظ�����
            // ��ת(��->��)
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

