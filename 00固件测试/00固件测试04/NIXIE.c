#include "NIXIE.h"


#define GET_BIT_VAL(byte,pos)  byte & (1<<pos)


u8 code LED_TABLE[] =
{
    // 0 	1	 2	-> 9	(索引012...9)
    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
    // 0. 1. 2. -> 9.	(索引10,11,12....19)
    0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
    // . -						(索引20,21)
    0x7F, 0xBF,
    // AbCdEFHJLPqU		(索引22,23,24....33)
    0x88,0x83,0xC6,0xA1,0x86,0x8E,0x89,0xF1,0xC7,0x8C,0x98,0xC1
};


//初始化函数
void NIXIE_init() {
    NIXIE_GPIO_init();
}

#define SCK_Action()


#define RCK_Action()                                       \
 /* 寄存器的锁存操作（所有的IO口并行输出）上升沿通电*/     \
    NIX_RCK = 0;                                           \
    NOP2();                                                \
    NIX_RCK = 1;                                           \
    NOP2();                                                \


#define NIXIE_out(byte){                                   \
	    for(i = 7; i >= 0; i--) {                            \
				/*NIX_DI只有0和1，只要不等于0，无论是什么值都是1 */\ 
			NIX_DI = GET_BIT_VAL(byte,i);                        \
																													 \
/* 寄存器的移位操作 上升沿通电*/                           \
    NIX_SCK = 0;                                           \
    NOP2();                                                \
    NIX_SCK = 1;                                           \
    NOP2();                                                \
																													 \
}                                                          \
																													 \
																													 \
}                                                          \

//取值函数
void NIXIE_show(u8 a_num,u8 b_dig) {
    char i;

    //设定显示的内容，从第7位开始递进去
    NIXIE_out(a_num);

    //设定显示内容的位置，
    NIXIE_out(b_dig);

    RCK_Action();

}

void NIXIE_display(u8 num,u8 pos) {
    u8 a_num = LED_TABLE[num];
    u8 b_dig = 1 << pos;

    NIXIE_show(a_num, b_dig);

}