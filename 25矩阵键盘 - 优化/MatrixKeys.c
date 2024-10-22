#include "MatrixKeys.h"
#include "GPIO.h"

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    P0_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7)
    P1_MODE_IO_PU(GPIO_Pin_7)
    P3_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5)
    P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1)
}

void MK_init() {
    GPIO_config();
}


u16 states = 0xffff;

void ROW_OUT(u8 row) {
    KROW1 = (row == 0) ? 0 : 1;
    KROW2 = (row == 1) ? 0 : 1;
    KROW3 = (row == 2) ? 0 : 1;
    KROW4 = (row == 3) ? 0 : 1;
}

u8 COL_IN(u8 col) {
    if(col == 0) return KCOL1;
    if(col == 1) return KCOL2;
    if(col == 2) return KCOL3;
    if(col == 3) return KCOL4;
    return 0;
}

#define IS_KEY_DOWN(row,col)  		((states & (1L << POS(row, col))) == 0)
#define IS_KEY_UP(row,col)    		((states & (1L << POS(row, col))) > 0)
#define SET_KEY_DOWN(row, col) 		states &= ~(1L << POS(row, col))//当前位 置0
#define SET_KEY_UP(row, col) 			 states |= (1L << POS(row, col))//当前位 置1
void MK_scan(void (*MK)(u8,u8,u8)) {
    u8 row,col;
    //外循环遍历行
    for(row = 0; row < ROW_NUM; row++) {
        //控制每行的开关
        ROW_OUT(row);
        //内循环遍历列
        for(col = 0; col < COL_NUM; col++) {
            if(COL_IN(col) == DOWM && IS_KEY_UP(row,col)) {
//                MK_on_keydown(row,col);
                SET_KEY_DOWN(row, col);
                MK(row,col,1);
            } else if(COL_IN(col) == UP && IS_KEY_DOWN(row, col)) {
//                MK_on_keyup(row,col);
                SET_KEY_UP(row, col);
                MK(row,col,1);
            }
        }

    }





}

