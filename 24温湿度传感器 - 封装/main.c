#include "config.h"
#include "Delay.h"
#include "DHT11.H"
#include "NVIC.h"
#include "UART.h"
#include "switch.h"

#define DHT	P46

/*
��ȡ��ʪ�ȴ�����DHT11��ʪ��ֵ
ʹ��UART1�յ��κ�����ʱ��������




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





void on_uart1_recv() {
    float humidity, temperature;
    DHT11_get_info(&humidity,&temperature);
    printf("ʪ�ȣ�%.1f%%  �¶ȣ�%.1f��\n",humidity,temperature);
}

void main() {
    EAXSFR();
    UART_config();
    DHT11_init();


    EA = 1;

    while(1) {
        //���ô��ڽ��������ж�
        //��� RX_TimeOut ���� 0����ʾ���ճ�ʱ��ʱ���������С�
        //���ݼ���� RX_TimeOut �Ƿ���� 0��
        //������� 0����ʾ��ʱ��ʱ���Ѿ�����ʱ��ɡ�˵���Ѿ��������
        if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
            //��� RX_Cnt ���� 0����ʾ���յ��������ֽ������� 0��
            if(COM1.RX_Cnt > 0) {
                on_uart1_recv();
            }
            //�������
            COM1.RX_Cnt = 0;
        }

        delay_ms(10);
    }
}