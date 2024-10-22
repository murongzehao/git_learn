#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "switch.h"

#define DHT	P46

/*
��ȡ��ʪ�ȴ�����DHT11��ʪ��ֵ
ʹ��UART1�յ��κ�����ʱ��������



*/
/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_6;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//��ʼ��
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

void Delay1us(void)	//@24.000MHz
{
    unsigned char data i;

    i = 5;
    while (--i);
}

void delay1_us(void)	//@24.000MHz
{
    NOP12();
}

// ��ȡDHT11����(ǧ��Ҫ�������־�����ʱ)

#define wait_level_change(level,min,max,desc)                          \
    cnt = 0;                                                           \
    while( DHT == level) {                                             \
        delay1_us();                                                   \
        cnt++;                                                         \
																																			 \
    }                                                                  \
    if( cnt <min || cnt > max) {                                      \
        printf("err:ʱ��[%dus],������%s[%d,%d]\n",          \
               (int)cnt,desc,(int)min,(int)max);                       \
																																			 \
    }                                                                  \

char on_read() {
    float humidity,temperature;
    char i,j;
    u8 dat[5] = {0};//�������40bit���ݣ�Ҳ����40 / 8 = 5���ֽ�
    u16 cnt = 0;//��ʱ����ÿ+1������1us��һ��Ҫ��ʼ��Ϊ0
    printf("read\n");

    //1.������ʼ�źţ�����10~30ms��
    DHT = 0;
    delay_ms(20);
    //Ȼ������
    DHT = 1;

    //2.�ȴ������ͷ�����(10~35ms)
    cnt = 0;
    while(DHT == 1 && cnt < 40) {
        delay1_us();
        cnt++;

    }
    if( cnt < 8 || cnt > 37) {
        printf("err:ʱ��[%dus]�����������ͷ�����ʱ��[%d,%d]\n",
               (int)cnt, (int)8, (int)37);
        return -1;
    }

    //3.�ӻ���Ӧ�͵�ƽʱ�䣬[78,88]us,��ǰ0��ֱ��1������ѭ��
    wait_level_change(0,78,88,"��Ӧ�͵�ƽʱ��");

    // 4. ��Ӧ�ߵ�ƽʱ�䣬87us [80, 92]us, ��ǰ1��ֱ��0 ����ѭ��
    wait_level_change(1,80,92,"��Ӧ�ߵ�ƽʱ��");

    for(i = 0; i < 5; i++ ) {
        //��λ�������Ҳ���ǵ�7λ�������
        for(j = 7; j>= 0; j--) {
            //5.����λ���͵�ƽʱ��54us[50,58]us,��ǰ0��ֱ��1������ѭ��
            wait_level_change(0,48,64,"DATA�źŵ͵�ƽʱ��");

            //6.����λ��0�ĸߵ�ƽʱ��[23,27],1�ĸߵ�ƽʱ��[68,74],��ǰ1��ֱ��0������ѭ��

            wait_level_change(1,23,74,"DATA�źŵ͵�ƽʱ��");

            if( cnt > 50 ) { //�ߵ�ƽʱ��ϳ�����1
                //��ʼ��Ĭ��i��0 0 0 0		0 0 0 0
                //����ж��������ж�����i�ֽڵĵ�j��λ��1�Ļ��ͷŽ�ȥ
                dat[i] |= (1 << j);
            }

        }

    }
    //�����ͷ�����
    DHT = 1;
    printf("cnt:%d\n",cnt);
    for( i = 0; i < 5; i++) {
        printf("%d ",(int)dat[i]);
    }
    printf("\n");

    //У������
    if(dat[0] +dat[1] + dat[2] +dat[3] != dat[4]) {
        printf("check failure!\n");
    }
    //ȡ��ʪ�ȵ��������� + С������ * 0.1
    humidity = dat[0] + dat[1] * 0.1f;

    //ȡ���¶ȵ��������� + С������(��7λ) * 0.1
    temperature = dat[2] +(dat[3] & 0x7f) * 0.1f;

                  //�¶�С�����ֵ����λ��0��ʾ���¶ȣ�1��ʾ���¶�
    if(dat[3] >> 7){
        temperature *= -1;
    }
		printf("ʪ�ȣ�%.1f%%  �¶ȣ�%.1f��\n",humidity,temperature);

    return 0 ;
}

void on_uart1_recv() {

    on_read();
}

void main() {
    EAXSFR();
    GPIO_config();
    UART_config();


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