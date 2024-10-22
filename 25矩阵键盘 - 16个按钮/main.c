#include "config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"

//��
#define KCOL1  P03
#define KCOL2  P06
#define KCOL3  P07
#define KCOL4  P17

//��
#define KROW1  P34
#define KROW2  P35
#define KROW3  P40
#define KROW4  P41

/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    P0_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7)
    P1_MODE_IO_PU(GPIO_Pin_7)
    P3_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5)
    P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1)
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

#define DOWM  0
#define UP    1

#define ROW_NUM  4
#define COL_NUM  4

u8 last_key_dowm = UP;


void ROW_OUT(u8 row) {
    KROW1 = (row == 0) ? 0 : 1;
    KROW2 = (row == 1) ? 0 : 1;
    KROW3 = (row == 2) ? 0 : 1;
    KROW4 = (row == 3) ? 0 : 1;
}

//��һ�ΰ�ť��״̬
u8 states[] = {
    1,1,1,1,		// 0, 1, 2, 3
    1,1,1,1,		// 4, 5, 6, 7,
    1,1,1,1,		// 8, 9,10,11,
    1,1,1,1,		//12,13,14,15,
};

u8 COL_IN(u8 col) {
    if(col == 0) return KCOL1;
    if(col == 1) return KCOL2;
    if(col == 2) return KCOL3;
    if(col == 3) return KCOL4;

    return 0;
}

//���㹫ʽ��
#define IS_KEY_DOWN(row,col)  		states[row * COL_NUM + col] == DOWM
#define IS_KEY_UP(row,col)    		states[row * COL_NUM + col] == UP
#define SET_KEY_DOWN(row, col) 		states[row * COL_NUM + col] = DOWM
#define SET_KEY_UP(row, col) 			states[row * COL_NUM + col] = UP
void scan() {
    u8 row,col;
    //��ѭ��������
    for(row = 0; row < ROW_NUM; row++) {
        //����ÿ�еĿ���
        ROW_OUT(row);
        //��ѭ��������
        for(col = 0; col < COL_NUM; col++) {
            if(COL_IN(col) == DOWM && states[row * COL_NUM + col] ) {
                printf("��%d�е�%d�а�ťdowm\n",(int)row+1,(int)col+1);
                SET_KEY_DOWN(row, col);
            } else if(COL_IN(col) == UP && IS_KEY_DOWN(row, col)) {
                printf("�� %d �� %d �а�ťUp!\n", (int)row+1, (int)col+1);
                SET_KEY_UP(row, col);
            }
        }

    }





}


void main() {

    GPIO_config();
    UART_config();

    EA = 1;

    while(1) {
        scan();


    }
}