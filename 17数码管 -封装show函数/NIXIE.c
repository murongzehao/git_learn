#include "NIXIE.h"


#define GET_BIT_VAL(byte,pos)  byte & (1<<pos)


//��ʼ������
void NIXIE_init(){
		NIXIE_GPIO_init();
}


//ȡֵ����
void NIXIE_show(u8 a_num,u8 b_dig){
	    char i;
		
    //�趨��ʾ�����ݣ��ӵ�7λ��ʼ�ݽ�ȥ
    for(i = 7; i >= 0; i--) {
//        if((a_num  & (1 << i) )== 0) {//ÿһλ����һ��1������0��˵�����λҪ����
//            //0����
//            NIX_DI = 0;
//        } else {
//            NIX_DI = 1;
//        }
        NIX_DI = GET_BIT_VAL(a_num,i);//NIX_DIֻ��0��1��ֻҪ������0��������ʲôֵ����1

        // �Ĵ�������λ���� ������ͨ��
        NIX_SCK = 0;
        NOP2();
        NIX_SCK = 1;
        NOP2();


    }

    //�趨��ʾ���ݵ�λ�ã�
    for(i = 7; i >= 0; i--) {
//        if( (b_dig  & (1 << i)) > 0) {
//            NIX_DI = 1;
//        } else {
//            NIX_DI = 0;
//        }
        NIX_DI = GET_BIT_VAL(b_dig,i);

        // �Ĵ�������λ����
        NIX_SCK = 0;
        NOP2();
        NIX_SCK = 1;
        NOP2();


    }

    // �Ĵ�����������������е�IO�ڲ��������������ͨ��
    NIX_RCK = 0;
    NOP2();
    NIX_RCK = 1;
    NOP2();

}