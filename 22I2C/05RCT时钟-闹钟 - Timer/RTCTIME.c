#include "RTCTIME.h"
#include "GPIO.h"
#include "NVIC.h"
#include "I2C.h"
#include "Switch.h"

#define NUMBER 7


/******************* IO配置函数 *******************/
static void	GPIO_config(void)
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

void RCT_TIME_init() {
    GPIO_config();
    I2C_config();

}

void RCT_TIME_set_clock(Clock_t c) {
    u8 p[NUMBER] = {0};
    u8 cent;

    p[0] = ((c.second / 10) << 4) | (c.second % 10);
    p[1] = ((c.minute / 10) << 4) | (c.minute % 10);
    p[2] = ((c.hour / 10) << 4) | (c.hour % 10);
    p[3] = ((c.day / 10) << 4) | (c.day % 10);
    p[4] = c.week;
    cent = (c.year >= 2100) ? 1 : 0;
    p[5] = (cent << 7 | (c.month / 10) << 4) | (c.month % 10);
    p[6] = ((c.year % 100 / 10) << 4) | (c.year % 10);

    RCT_TIME_Write(RCT_TIME_ADDR,RCT_TIME_REG, p, NUMBER);
}

// 函数里面的运算出了函数就会结束进程，要调用函数里面的值需要用指针
void RCT_TIME_get_clock(Clock_t * c) {
    u8 p[NUMBER] = {0};
    u8 cent;
    //结构体
//读取数据
    RCT_TIME_Read(RCT_TIME_ADDR,RCT_TIME_REG,p,NUMBER);
    //因为设定是BCD格式即4位二进制数为一位十进制数，所以获取十位的数需要把前4位的数移到后四位获取单个数再 *10 转换成十进制的十位
    //second 如 p[0] =  VL 1 1 1   0 0 0 0  要获取十位数只需获取前3位  &0x07(0 0 0 0   0 1 1 1)是为了把VL去掉 或是前面不用的位数就不需要换算
    c -> second = ((p[0] >> 4) & 0x07) * 10 + (p[0] & 0x0f);
    c -> minute = ((p[1] >> 4) & 0x07) * 10 + (p[1] & 0x0f);
    c -> hour   = ((p[2] >> 4) & 0x03) * 10 + (p[2] & 0x0f);
    c -> day    = ((p[3] >> 4) & 0x03) * 10 + (p[3] & 0x0f);
    c -> week   =   p[4] & 0x07;
    c -> month  = ((p[5] >> 4) & 0x01) * 10 + (p[5] & 0x0f);
    cent   =   p[5] >> 7;
    c -> year   = ((p[6] >> 4) & 0x0f) * 10 + (p[6] & 0x0f);
    //年还要加上世纪组成四位数
    c -> year   +=  ((cent == 0) ? 2000 : 2100);
}

void set_alarm(alarm a) {
    u8 p[4];

    // 分 M 1 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.minute >= 0) {
        p[0] = ((a.minute / 10) << 4) + (a.minute % 10);
    } else {
        p[0] = 0x80;
    }
    // 时 H X 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.hour >= 0) {
        p[1] = ((a.hour / 10) << 4) + (a.hour % 10);
    } else {
        p[1] = 0x80;
    }
    // 天 D X 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.day >= 0) {
        p[2] = ((a.day / 10) << 4) + (a.day % 10);
    } else {
        p[2] = 0x80;
    }
    // 周 W X X X - X 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.week >= 0) {
        p[3] = a.week;
    } else {
        p[3] = 0x80;
    }

    RCT_TIME_Write(RCT_TIME_ADDR, 0x09, p, 4);

}
//0x09是报警寄存器的位置，0x01是控制寄存器的位置。
//写进报警寄存器，通过控制寄存器的开关来读取

void clear_alarm() {
    u8 cs2;
    // 清除Alarm标记，AF设置为0，下次闹钟到点才能触发INT
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);//先把闹钟里存的值用cs2读出来，有可能之前留存有数据
    //定义的cs2的值是0x00,这里操作的是读出来的cs2
    //清除Alarm标记,AF置0
    cs2 &= ~(1 << 3);				//0x03
    // 写回cs2寄存器
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &cs2, 1);

}

void get_alarm(u8 enable) {
    u8 cs2;

// 启用闹铃 0x01 cs2--------------------------------------------------------
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);//先把闹钟里存的值用cs2读出来，有可能之前留存有数据
    //清除Alarm标记,AF置0
    //定义的cs2的值是0x00,这里操作的是读出来的cs2
    cs2 &= ~(1 << 3);				//0x03
    // 开启Alarm中断，AIE设置为1
    if(enable) {
        cs2 |= (1 << 1);	// 0x02
    } else {
        cs2 &=~(1 << 1);	// 0x02
    }
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &cs2, 1);

}


// 设置定时器
void set_Timer(TimerFreq freq, u8 countdown) {
    u8 tim;
    //设置Timer计数值
    tim = (1 << 7) + freq;
    RCT_TIME_Write(RCT_TIME_ADDR, 0x0E, &tim, 1);
    tim = countdown;
    RCT_TIME_Write(RCT_TIME_ADDR, 0x0f, &tim, 1);
}

// 启用定时器
void get_Timer(u8 enable) {
    u8 tim;
    //启用Timer
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &tim, 1);//先把闹钟里存的值用cs2读出来，有可能之前留存有数据
    //清除Alarm标记,AF置0
    //定义的cs2的值是0x00,这里操作的是读出来的cs2
    tim &= ~(1 << 2);				//0x02
    // 开启Timer中断，TIE设置为1
    if(enable) {
        tim |= (1 << 0);	// 0x01
    } else {
        tim &=~(1 << 0);	// 0x01
    }
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &tim, 1);

}

// 清除定时器
void clear_Timer() {
    u8 tim;
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &tim, 1);
    //清除Timer标记,TF置0
    //定义的cs2的值是0x00,这里操作的是读出来的cs2
    tim &= ~(1 << 2);				//0x02
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &tim, 1);
}

void ext_int3_call(void) {
#if USE_ALARM || USE_TIMER
    u8 cs2;
    // 清除Alarm标记，AF设置为0，下次闹钟到点才能触发INT
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);
#if USE_ALARM
    //AIE && AF
    if( (cs2 & 0x02) &&(cs2 & 0x08) ) {
        //清理Alarm的AF标记
        clear_alarm();
        //当中断触发时的实现逻辑
        TRC_on_alarm();
    }
#endif

#if USE_TIMER
    //TIE && TF
    if((cs2 & 0x01) && (cs2 & 0x04)) {
        clear_Timer();
        TRC_on_timer();
    }

#endif
#endif

}








