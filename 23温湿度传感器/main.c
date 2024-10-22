#include "config.h"
#include "Delay.h"
#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "switch.h"

#define DHT	P46

/*
读取温湿度传感器DHT11温湿度值
使用UART1收到任何数据时，读数据



*/
/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_6;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//初始化
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

// 读取DHT11数据(千万不要随意打日志，会耗时)

#define wait_level_change(level,min,max,desc)                          \
    cnt = 0;                                                           \
    while( DHT == level) {                                             \
        delay1_us();                                                   \
        cnt++;                                                         \
																																			 \
    }                                                                  \
    if( cnt <min || cnt > max) {                                      \
        printf("err:时间[%dus],不满足%s[%d,%d]\n",          \
               (int)cnt,desc,(int)min,(int)max);                       \
																																			 \
    }                                                                  \

char on_read() {
    float humidity,temperature;
    char i,j;
    u8 dat[5] = {0};//容器存放40bit数据，也就是40 / 8 = 5个字节
    u16 cnt = 0;//计时器，每+1，过了1us，一定要初始化为0
    printf("read\n");

    //1.主机起始信号（拉低10~30ms）
    DHT = 0;
    delay_ms(20);
    //然后拉高
    DHT = 1;

    //2.等待主机释放总线(10~35ms)
    cnt = 0;
    while(DHT == 1 && cnt < 40) {
        delay1_us();
        cnt++;

    }
    if( cnt < 8 || cnt > 37) {
        printf("err:时间[%dus]不满足主机释放总线时间[%d,%d]\n",
               (int)cnt, (int)8, (int)37);
        return -1;
    }

    //3.从机响应低电平时间，[78,88]us,当前0，直到1，结束循环
    wait_level_change(0,78,88,"响应低电平时间");

    // 4. 响应高电平时间，87us [80, 92]us, 当前1，直到0 结束循环
    wait_level_change(1,80,92,"响应高电平时间");

    for(i = 0; i < 5; i++ ) {
        //高位先输出，也就是第7位先输出、
        for(j = 7; j>= 0; j--) {
            //5.数据位：低电平时间54us[50,58]us,当前0，直到1，结束循环
            wait_level_change(0,48,64,"DATA信号低电平时间");

            //6.数据位：0的高电平时间[23,27],1的高电平时间[68,74],当前1，直到0，结束循环

            wait_level_change(1,23,74,"DATA信号低电平时间");

            if( cnt > 50 ) { //高电平时间较长，是1
                //初始化默认i是0 0 0 0		0 0 0 0
                //这个判断是用来判断索引i字节的第j号位是1的话就放进去
                dat[i] |= (1 << j);
            }

        }

    }
    //主机释放总线
    DHT = 1;
    printf("cnt:%d\n",cnt);
    for( i = 0; i < 5; i++) {
        printf("%d ",(int)dat[i]);
    }
    printf("\n");

    //校验数据
    if(dat[0] +dat[1] + dat[2] +dat[3] != dat[4]) {
        printf("check failure!\n");
    }
    //取出湿度的整数部分 + 小数部分 * 0.1
    humidity = dat[0] + dat[1] * 0.1f;

    //取出温度的整数部分 + 小数部分(低7位) * 0.1
    temperature = dat[2] +(dat[3] & 0x7f) * 0.1f;

                  //温度小数部分的最高位：0表示正温度，1表示负温度
    if(dat[3] >> 7){
        temperature *= -1;
    }
		printf("湿度：%.1f%%  温度：%.1f℃\n",humidity,temperature);

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
        //调用串口接收数据判断
        //如果 RX_TimeOut 大于 0，表示接收超时计时器正在运行。
        //检查递减后的 RX_TimeOut 是否等于 0。
        //如果等于 0，表示超时计时器已经倒计时完成。说明已经接收完成
        if(COM1.RX_TimeOut > 0 && --COM1.RX_TimeOut == 0) {
            //如果 RX_Cnt 大于 0，表示接收到的数据字节数大于 0。
            if(COM1.RX_Cnt > 0) {
                on_uart1_recv();
            }
            //清除数据
            COM1.RX_Cnt = 0;
        }

        delay_ms(10);
    }
}