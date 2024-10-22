#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "switch.h"



#define MOTOR		P01


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
    GPIO_InitStructure.Pin  = GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//��ʼ��

    P5_MODE_IO_PU(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
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
#define KEY0 P51 ///��ťĬ���Ǹߵ�ƽ������֮����ͨ��ӵ�GND��Ϊ�͵�ƽ
#define KEY1 P52
#define KEY2 P53
#define KEY3 P54

#define DOWM  0
#define UP    1

//��һ�εĳ�ʼ״̬��̧��
u8 last_key_states = 0x0f;		// key�����һ��״̬

//�ж�ָ��λ�õ��ǰ��»���̧��
#define IS_KEY_DOWM(i)   (last_key_states & (1 << i)) == 0 //�ж�last_key_states��״̬���������0�������ͣ����ǰ���״̬
#define IS_KEY_UP(i)     (last_key_states & (1 << i)) > 0  //�ж�last_key_states��״̬���������0�������ߣ�����̧��״̬

//��ָ��λ�õ�״̬��Ϊ���»�̧��
#define SET_KEY_DOWM(i)   last_key_states &= ~(1 << i)  //��ָ���ı�λ�ø�Ϊ0�����ͣ�������״̬
#define SET_KEY_UP(i)     last_key_states |= (1 << i)		//��ָ���ı�λ�ø�Ϊ1�����ߣ���̧��״̬

void main() {
    GPIO_config();
    UART_config();

    EA = 1;

    while(1) {
        if( KEY0 && IS_KEY_DOWM(0)) {// �����̧��Up 1, ��һ���ǰ���Down 0
            printf("KEY0 Up\n");
            MOTOR = 0;
            SET_KEY_UP(0);
        }
        else if(!KEY0 && IS_KEY_UP(0)) {// ����ǰ���Down 0, ��һ����̧��Up 1
            printf("KEY0 Dowm\n");
            MOTOR = 1;
            SET_KEY_DOWM(0);

        }
        if( KEY1 && IS_KEY_DOWM(1)) {// �����̧��Up 1, ��һ���ǰ���Down 0
            printf("KEY1 Up\n");
            SET_KEY_UP(1);
        }
        else if(!KEY1 && IS_KEY_UP(1)) {// ����ǰ���Down 0, ��һ����̧��Up 1
            printf("KEY1 Dowm\n");
            SET_KEY_DOWM(1);

        }
        if( KEY2 && IS_KEY_DOWM(2)) {// �����̧��Up 1, ��һ���ǰ���Down 0
            printf("KEY2 Up\n");
            SET_KEY_UP(2);
        }
        else if(!KEY2 && IS_KEY_UP(2)) {// ����ǰ���Down 0, ��һ����̧��Up 1
            printf("KEY2 Dowm\n");
            SET_KEY_DOWM(2);

        }
        if( KEY3 && IS_KEY_DOWM(3)) {// �����̧��Up 1, ��һ���ǰ���Down 0
            printf("KEY3 Up\n");
            SET_KEY_UP(3);
        }
        else if(!KEY3 && IS_KEY_UP(3)) {// ����ǰ���Down 0, ��һ����̧��Up 1
            printf("KEY3 Dowm\n");
            SET_KEY_DOWM(3);

        }


        delay_ms(10);

    }
}