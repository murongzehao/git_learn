#include "config.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "Delay.h"
#include "I2C.h"


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_OD;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化
}

/****************  I2C初始化函数 *****************/
void	I2C_config(void)
{
    I2C_InitTypeDef		I2C_InitStructure;

    I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//主从选择   I2C_Mode_Master, I2C_Mode_Slave
    I2C_InitStructure.I2C_Enable    = ENABLE;			//I2C功能使能,   ENABLE, DISABLE
    I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			//主机使能自动发送,  ENABLE, DISABLE
    I2C_InitStructure.I2C_Speed     = 13;				//总线速度=Fosc/2/(Speed*2+4),      0~63
    // 400k, 24M => 13
    I2C_Init(&I2C_InitStructure);
    NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);	//主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    I2C_SW(I2C_P33_P32);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
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
#define NUMBER 7

void main() {
    u8 dev_addr = 0x51 << 1; //A2 设备写地址
    u8 mem_addr = 0x02;			// 这个芯片设定的秒地址是02
    u8 p[NUMBER] = {0};
    u8 second, minute, hour, day, week, month, cent;
    u16 year;

    EAXSFR();//扩展寄存器使能
    GPIO_config();
    UART_config();
    I2C_config();

    EA = 1;
    second = 55,  minute = 59, hour = 23;
    day = 31, week = 6, month = 12,year = 2024;

    p[0] = (second / 10) << 4 | (second % 10);
    p[1] = (minute / 10) << 4 | (minute % 10);
    p[2] = (hour / 10) << 4 | (hour % 10);
    p[3] = (day / 10) << 4 | (day % 10);
    p[4] = week;
    cent = (year >= 2100) ? 1 : 0;
    p[5] = cent << 7 | (month / 10) << 4 | (month % 10);
    p[6] = (year % 100 / 10) << 4 | (year % 10);
		//写数据（设备地址(主机地址)，寄存器地址，数据，数据字节数）
    I2C_WriteNbyte(dev_addr,mem_addr, p, NUMBER);


    while(1) {
        //读取数据
        I2C_ReadNbyte(dev_addr, mem_addr, p,NUMBER);
        //因为设定是BCD格式即4位二进制数为一位十进制数，所以获取十位的数需要把前4位的数移到后四位获取单个数再 *10 转换成十进制的十位
        //second 如 p[0] =  VL 1 1 1   0 0 0 0  要获取十位数只需获取前3位  &0x07(0 0 0 0   0 1 1 1)是为了把VL去掉 或是前面不用的位数就不需要换算
        second = ((p[0] >> 4) & 0x07) * 10 + (p[0] & 0x0f);
        minute = ((p[1] >> 4) & 0x07) * 10 + (p[1] & 0x0f);
        hour   = ((p[2] >> 4) & 0x03) * 10 + (p[2] & 0x0f);
        day    = ((p[3] >> 4) & 0x03) * 10 + (p[3] & 0x0f);
        week   =   p[4] & 0x07;
        month  = ((p[5] >> 4) & 0x01) * 10 + (p[5] & 0x0f);
        cent   =   p[5] >> 7;
        year   = ((p[6] >> 4) & 0x0f) * 10 + (p[6] & 0x0f);
        //年还要加上世纪组成四位数
        year   +=  ((cent == 0) ? 2000 : 2100);

        printf("%02d时:%02d分:%02d秒\n",(int)hour,(int)minute,(int)second);
        printf("%02d/%02d/%02d\n",(int)year,(int)month,(int)day);
        printf("week:%d\n",(int)week);
        printf("----------------------------");
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);


    }
}