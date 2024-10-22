#include "config.h"
#include "GPIO.h"
#include "NIXIE.h"
#include "delay.h"


int arr[] = {2,5};
void main() {
	u8 i;
//    u8 a_num = 0x78; // 0111 1000  ÷√0
//    //  0000 0000
//    u8 b_dig = 0xff;//  1111 1111  ÷√1
    NIXIE_init();
//    NIXIE_show(a_num,b_dig);
// NIXIE_display(6,2);



    while(1){
		for( i = 0; i < 8; i++){
			NIXIE_display(arr[i],i);
			}
		
		}
}