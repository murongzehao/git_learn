#ifndef  __MATRIXKEYS_H_
#define  __MATRIXKEYS_H_

#include "config.h"


//列
#define KCOL1  P03
#define KCOL2  P06
#define KCOL3  P07
#define KCOL4  P17

//行
#define KROW1  P34
#define KROW2  P35
#define KROW3  P40
#define KROW4  P41


#define DOWM  0
#define UP    1

#define ROW_NUM  4
#define COL_NUM  4


//行 * 列的长度 算出在第几列，然后就按顺序一个一个从第0个加过去
#define POS(row, col) (row * COL_NUM + col)

void MK_init();

void MK_scan(void (*MK)(u8,u8,u8));

extern void MK_on_keyup(u8 row,u8 col);
extern void MK_on_keydown(u8 row, u8 col);


#endif