#include "config.h"
#include	"GPIO.h"
#include	"UART.h"
#include	"Delay.h"
#include	"NVIC.h"
#include	"Switch.h"




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



void main() {
    int8 i;

    GPIO_config();
    UART_config();

    EA = 1;

    LED_SW = 0;

    // LED, ��ȫ���ص�
    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

    while(1) {

        //ѭ���������жϲ�������յ�������
        if(COM1.RX_TimeOut > 0 &&  --COM1.RX_TimeOut == 0)//RX_TimeOut ���ճ�ʱ
        {
            if(COM1.RX_Cnt > 0)//�ж��Ƿ�������
            {
                //RX1_Buffer[i]����ǽ��ܵ�ÿ���ֽڣ�д����TX1_write2buff
                if(RX1_Buffer[0] == 0x00) {//��һ���ֽ�=0
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
                    delay_ms(250);
                    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

                }

                else if(RX1_Buffer[0] == 0x10)
                {
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
                    delay_ms(250);
                    LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;


                }
                //�������ݼ���ֵ
                COM1.RX_Cnt = 0;
            }





        }
    }

}

