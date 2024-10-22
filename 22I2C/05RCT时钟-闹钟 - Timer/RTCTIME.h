#ifndef  __RTCTIME_H_
#define  __RTCTIME_H_

#include "config.h"


// ��������1����������0
#define USE_ALARM		1
#define USE_TIMER		1

//�ѵ�ַ����������Ķ�
#define RCT_TIME_ADDR	(0x51 << 1)	// A2 �豸д��ַ
#define RCT_TIME_REG 	0x02				// �洢��ַ��ʱ��Ĵ洢��ַ��ʼλ��



#define  RCT_TIME_Write(a,m,p,n)     I2C_WriteNbyte(a,m,p,n)
#define  RCT_TIME_Read(a,m,p,n)      I2C_ReadNbyte(a,m,p,n)

typedef struct Clock { //typedef ��һ���������͸���һ��������
    //Clock_t�����Զ������ struct Clock��������
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

// ��������
void set_alarm(alarm a);

// ��������
void get_alarm(u8 enable);

// �������
void clear_alarm();

// ���ö�ʱ��
void set_Timer(TimerFreq freq, u8 countdown);

// ���ö�ʱ��
void get_Timer(u8 enable);

// �����ʱ��
void clear_Timer();

#endif