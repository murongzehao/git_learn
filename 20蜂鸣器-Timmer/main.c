#include "config.h"
#include "GPIO.h"
#include "nvic.h"
#include "Timer.h"
#include "switch.h"
#include "delay.h"


//声明按钮
#define BUZZER	P00


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_0;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//初始化
}

//			 			C`	   D`     E`   F`	  G`	A`	  B`    C``
u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

/************************ 定时器配置 ****************************/
void	Timer_config(u16 hz_value)
{

    //Timer的频率跟蜂鸣器的的频率关系是两倍的关系，
    //蜂鸣器一高一低等于Timer一个周期
    TIM_InitTypeDef		TIM_InitStructure;						//结构定义

    //定时器0做16位自动重装, 中断频率为1000HZ，中断函数从P5.3取反输出方波信号.
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	
		//确保定时器的中断频率与目标频率 hz_value 保持一致。
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (hz_value * 2));		//初值, 1UL 4字节 unsigned long 1s, 1000ms, 1000 000us,1000 000 000ns
    // 设置重装数值：指定Timer频率 1000Hz (每秒执行1000次(频率)，每次间隔1ms(周期))
    // 频率最大：不要大于 1 000 000 Hz, (周期不低于1us)
    // 频率最小：不要小于       367 Hz, (周期不超过2.724ms)

    //  24000000 / 367hz 最小频率

    TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4

    NVIC_Timer0_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void timer0_callback() {
    BUZZER = !BUZZER;
}


void main() {
    u16 idx = 0;
    GPIO_config();

    EA = 1;

    while(1) {
        Timer_config(hz[idx]);
			
			if( ++idx > 7) idx = 0;
			
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);

    }
}