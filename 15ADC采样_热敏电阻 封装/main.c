#include "config.h"
#include "NTC.h"
#include "Delay.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "SWITCH.h"


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    // P30, P31 ׼˫��
    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
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
int rst_T;
    EAXSFR();

    GPIO_config();
    UART_config();
	
	NTC_init();

    EA = 1;

    while(1) {
			
			rst_T = NTC_get_temperature();
					printf("�¶ȣ� %d ��\n", rst_T);

    delay_ms(250);
    delay_ms(250);

    }
}