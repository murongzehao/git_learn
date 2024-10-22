#ifndef  __RTCTIME_H_
#define  __RTCTIME_H_

#include "config.h"

//�ѵ�ַ����������Ķ�
#define RCT_TIME_ADDR	(0x51 << 1)	// A2 �豸д��ַ
#define RCT_TIME_REG 	0x02				// �洢��ַ��ʱ��Ĵ洢��ַ��ʼλ��




#define  RCT_TIME_Write(a,m,p,n)     I2C_WriteNbyte(a,m,p,n)
#define  RCT_TIME_Read(a,m,p,n)      I2C_ReadNbyte(a,m,p,n)

typedef struct Clock{//typedef ��һ���������͸���һ��������
	//Clock_t�����Զ������ struct Clock��������
  u8 second;
	u8 minute;
	u8 hour;
	u8 day;
	u8 week;
	u8 month;
	u16 year;
}Clock_t;

typedef struct {
    u8 minute;
    u8 hour;
    u8 day;
    u8 week;
}alarm;

void RCT_TIME_init();

void RCT_TIME_set_clock(Clock_t c);

void RCT_TIME_get_clock(Clock_t * c);

// ��������
void set_alarm(alarm a);

// ��������
void get_alarm();

// �������
void clear_alarm();

#endif