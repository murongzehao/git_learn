#include "NIXIE.h"


#define GET_BIT_VAL(byte,pos)  byte & (1<<pos)


//初始化函数
void NIXIE_init(){
		NIXIE_GPIO_init();
}


//取值函数
void NIXIE_show(u8 a_num,u8 b_dig){
	    char i;
		
    //设定显示的内容，从第7位开始递进去
    for(i = 7; i >= 0; i--) {
//        if((a_num  & (1 << i) )== 0) {//每一位都与一个1，等于0就说明这个位要点亮
//            //0点亮
//            NIX_DI = 0;
//        } else {
//            NIX_DI = 1;
//        }
        NIX_DI = GET_BIT_VAL(a_num,i);//NIX_DI只有0和1，只要不等于0，无论是什么值都是1

        // 寄存器的移位操作 上升沿通电
        NIX_SCK = 0;
        NOP2();
        NIX_SCK = 1;
        NOP2();


    }

    //设定显示内容的位置，
    for(i = 7; i >= 0; i--) {
//        if( (b_dig  & (1 << i)) > 0) {
//            NIX_DI = 1;
//        } else {
//            NIX_DI = 0;
//        }
        NIX_DI = GET_BIT_VAL(b_dig,i);

        // 寄存器的移位操作
        NIX_SCK = 0;
        NOP2();
        NIX_SCK = 1;
        NOP2();


    }

    // 寄存器的锁存操作（所有的IO口并行输出）上升沿通电
    NIX_RCK = 0;
    NOP2();
    NIX_RCK = 1;
    NOP2();

}