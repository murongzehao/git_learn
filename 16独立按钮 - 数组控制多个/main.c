#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "switch.h"



#define MOTOR		P01


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

    P5_MODE_IO_PU(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
}


/***************  串口初始化函数 *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			  //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				  //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			  //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		  //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}
#define KEY0 P51 ///按钮默认是高电平，按下之后联通会接到GND变为低电平
#define KEY1 P52
#define KEY2 P53
#define KEY3 P54

#define DOWM  0
#define UP    1

//上一次的初始状态是抬起
u8 last_key_states = 0x0f;		// key的最后一次状态

//判断指定位置的是按下还是抬起
#define IS_KEY_DOWM(i)   (last_key_states & (1 << i)) == 0 //判断last_key_states的状态，如果等于0就是拉低，就是按下状态
#define IS_KEY_UP(i)     (last_key_states & (1 << i)) > 0  //判断last_key_states的状态，如果大于0就是拉高，就是抬起状态

//将指定位置的状态改为按下或抬起
#define SET_KEY_DOWM(i)   last_key_states &= ~(1 << i)  //将指定改变位置改为0，拉低，即按下状态
#define SET_KEY_UP(i)     last_key_states |= (1 << i)		//将指定改变位置改为1，拉高，即抬起状态

void main() {
    GPIO_config();
    UART_config();

    EA = 1;

    while(1) {
        if( KEY0 && IS_KEY_DOWM(0)) {// 这次是抬起Up 1, 上一次是按下Down 0
            printf("KEY0 Up\n");
            MOTOR = 0;
            SET_KEY_UP(0);
        }
        else if(!KEY0 && IS_KEY_UP(0)) {// 这次是按下Down 0, 上一次是抬起Up 1
            printf("KEY0 Dowm\n");
            MOTOR = 1;
            SET_KEY_DOWM(0);

        }
        if( KEY1 && IS_KEY_DOWM(1)) {// 这次是抬起Up 1, 上一次是按下Down 0
            printf("KEY1 Up\n");
            SET_KEY_UP(1);
        }
        else if(!KEY1 && IS_KEY_UP(1)) {// 这次是按下Down 0, 上一次是抬起Up 1
            printf("KEY1 Dowm\n");
            SET_KEY_DOWM(1);

        }
        if( KEY2 && IS_KEY_DOWM(2)) {// 这次是抬起Up 1, 上一次是按下Down 0
            printf("KEY2 Up\n");
            SET_KEY_UP(2);
        }
        else if(!KEY2 && IS_KEY_UP(2)) {// 这次是按下Down 0, 上一次是抬起Up 1
            printf("KEY2 Dowm\n");
            SET_KEY_DOWM(2);

        }
        if( KEY3 && IS_KEY_DOWM(3)) {// 这次是抬起Up 1, 上一次是按下Down 0
            printf("KEY3 Up\n");
            SET_KEY_UP(3);
        }
        else if(!KEY3 && IS_KEY_UP(3)) {// 这次是按下Down 0, 上一次是抬起Up 1
            printf("KEY3 Dowm\n");
            SET_KEY_DOWM(3);

        }


        delay_ms(10);

    }
}