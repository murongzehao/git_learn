#include "config.h"
#include "GPIO.h"
#include "NIXIE.h"



void main() {
    u8 a_num = 0x78; // 0111 1000  ÷√0
    //  0000 0000
    u8 b_dig = 0xff;//  1111 1111  ÷√1
    NIXIE_init();
    NIXIE_show(a_num,b_dig);




    while(1);
}