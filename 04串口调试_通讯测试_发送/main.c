#include "config.h"
#include	"GPIO.h"
#include	"UART.h"
#include	"Delay.h"
#include	"NVIC.h"
#include	"Switch.h"

void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ���壬����һ���ṹ�����

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ����0��λ��GPIO_Pin_1 ��1��λ
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��������λP3��λ��ַ0��1
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
    GPIO_config();
    UART_config();

    //����ȫ���ж�
    EA = 1;
    //Ҫ�����ļ�����Ҫ�õ�UART.H�������ж��巢���ļ��ķ���

    TX1_write2buff(0X23);//�����ֽ�
    PrintString1("PrintString1");
	printf("printf-> %s : %d\n","Hello",(int)123);//��ӡ����Ҫǿת��int



  
    while(1) {
        TX1_write2buff(0X47);


        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }

}

