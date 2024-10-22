#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"
#include "RTCTIME.h"
#include "I2C.h"
#include "Exti.h"
#include "NVIC.h"

/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_7;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
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

    UART1_SW(UART1_SW_P30_P31);		// ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


/******************** INT���� ********************/
void	Exti_config(void)
{
    EXTI_InitTypeDef	Exti_InitStructure;							//�ṹ����

    Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
    Ext_Inilize(EXT_INT3,&Exti_InitStructure);				//��ʼ��
    NVIC_INT3_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}



void TRC_on_alarm() {

    printf("Alarm\n");

}

void TRC_on_timer() {

    printf("Timer\n");
}






void main() {
    Clock_t c;
    alarm a;

    EAXSFR();

    GPIO_config();
    UART_config();
    Exti_config();
    RCT_TIME_init();
    EA = 1;


    delay_ms(200);

    // ��ǰ׼��ʱ�䣬д1��ʱ�� -------------------------------------
    // ׼�� �롢�֡�ʱ���졢�ܡ�����&�¡���
    c.year = 2024, c.month = 8, c.day = 12, c.week = 1;
    c.hour = 23, c.minute = 59, c.second = 55;
    RCT_TIME_set_clock(c);

    // �������� 0x09--------------------------------------------------------
    a.minute = 0, a.hour = 0, a.day = 13, a.week = 2;
    set_alarm(a);

    // ��������
    get_alarm(ENABLE);

// ���ö�ʱ�� 0E��Ƶ�ʺ�0F��ֵ
    set_Timer(HZ64, 64);

// ���ö�ʱ��
    get_Timer(ENABLE);


    while(1) {

        // ѭ����ȡ����: �롢�֡�ʱ���졢�ܡ��¡��ꡢ����
        RCT_TIME_get_clock(&c);

        // ��ӡ����
        printf("%02d-%02d-%02d ", (int)c.year, (int)c.month, (int)c.day);
        printf("%02d:%02d:%02d ", (int)c.hour, (int)c.minute, (int)c.second);
        printf("week->%d\n", (int)c.week);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);


    }
}
