#include "config.h"
#include "GPIO.h"
#include "nvic.h"
#include "Timer.h"
#include "switch.h"
#include "delay.h"


//������ť
#define BUZZER	P00


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_0;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//��ʼ��
}

//			 			C`	   D`     E`   F`	  G`	A`	  B`    C``
u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

/************************ ��ʱ������ ****************************/
void	Timer_config(u16 hz_value)
{

    //Timer��Ƶ�ʸ��������ĵ�Ƶ�ʹ�ϵ�������Ĺ�ϵ��
    //������һ��һ�͵���Timerһ������
    TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����

    //��ʱ��0��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ���жϺ�����P5.3ȡ����������ź�.
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	
		//ȷ����ʱ�����ж�Ƶ����Ŀ��Ƶ�� hz_value ����һ�¡�
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (hz_value * 2));		//��ֵ, 1UL 4�ֽ� unsigned long 1s, 1000ms, 1000 000us,1000 000 000ns
    // ������װ��ֵ��ָ��TimerƵ�� 1000Hz (ÿ��ִ��1000��(Ƶ��)��ÿ�μ��1ms(����))
    // Ƶ����󣺲�Ҫ���� 1 000 000 Hz, (���ڲ�����1us)
    // Ƶ����С����ҪС��       367 Hz, (���ڲ�����2.724ms)

    //  24000000 / 367hz ��СƵ��

    TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4

    NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

void timer0_callback() {
    BUZZER = !BUZZER;
}


void main() {
    u16 idx = 0;
    GPIO_config();

    EA = 1;

    while(1) {
        Timer_config(hz[idx]);
			
			if( ++idx > 7) idx = 0;
			
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);

    }
}