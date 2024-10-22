#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "switch.h"


#define KEY1 P51
#define KEY2 P52

#define a  P00
#define MOTOR  P01

/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//��ʼ��
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
    EAXSFR();
    GPIO_config();
    UART_config();



EA = 1;


    while(1) {
        if(KEY1 == 1) {
            MOTOR = 0;
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
            printf("KEY1 Up\n");

        }
        else if(KEY1 == 0) {
            MOTOR = 1;
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
//            delay_ms(250);
            printf("KEY1 Dowm\n");

        }



    }
}