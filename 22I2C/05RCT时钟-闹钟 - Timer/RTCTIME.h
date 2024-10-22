#ifndef  __RTCTIME_H_
#define  __RTCTIME_H_

#include "config.h"


// 启用设置1，禁用设置0
#define USE_ALARM		1
#define USE_TIMER		1

//把地址声明，方便改动
#define RCT_TIME_ADDR	(0x51 << 1)	// A2 设备写地址
#define RCT_TIME_REG 	0x02				// 存储地址：时间的存储地址开始位置



#define  RCT_TIME_Write(a,m,p,n)     I2C_WriteNbyte(a,m,p,n)
#define  RCT_TIME_Read(a,m,p,n)      I2C_ReadNbyte(a,m,p,n)

typedef struct Clock { //typedef 给一个变量类型赋予一个新名字
    //Clock_t就是自定义变量 struct Clock的新名字
    u8 second;
    u8 minute;
    u8 hour;
    u8 day;
    u8 week;
    u8 month;
    u16 year;
} Clock_t;

typedef struct {
    char minute;
    char hour;
    char day;
    char week;
} alarm;

typedef enum {
	HZ4096 = 0x00,
	HZ64	 = 0x01,
	HZ1		 = 0x10,
	HZ1_60 = 0x11,
} TimerFreq;


extern void TRC_on_alarm();
extern void TRC_on_timer();

void RCT_TIME_init();


void RCT_TIME_set_clock(Clock_t c);


void RCT_TIME_get_clock(Clock_t * c);

// 设置闹铃
void set_alarm(alarm a);

// 启用闹铃
void get_alarm(u8 enable);

// 清除闹铃
void clear_alarm();

// 设置定时器
void set_Timer(TimerFreq freq, u8 countdown);

// 启用定时器
void get_Timer(u8 enable);

// 清除定时器
void clear_Timer();

#endif