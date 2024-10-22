#include "RTCTIME.h"
#include "GPIO.h"
#include "NVIC.h"
#include "I2C.h"
#include "Switch.h"

#define NUMBER 7


/******************* IO���ú��� *******************/
static void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_OD;		  //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}

/****************  I2C��ʼ������ *****************/
void	I2C_config(void)
{
    I2C_InitTypeDef		I2C_InitStructure;

    I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//����ѡ��   I2C_Mode_Master, I2C_Mode_Slave
    I2C_InitStructure.I2C_Enable    = ENABLE;			//I2C����ʹ��,   ENABLE, DISABLE
    I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			//����ʹ���Զ�����,  ENABLE, DISABLE
    I2C_InitStructure.I2C_Speed     = 13;				//�����ٶ�=Fosc/2/(Speed*2+4),      0~63
    // 400k, 24M => 13
    I2C_Init(&I2C_InitStructure);
    NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);	//����ģʽ, I2C_Mode_Master, I2C_Mode_Slave; �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

    I2C_SW(I2C_P33_P32);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

void RCT_TIME_init() {
    GPIO_config();
    I2C_config();

}

void RCT_TIME_set_clock(Clock_t c) {
    u8 p[NUMBER] = {0};
    u8 cent;

    p[0] = ((c.second / 10) << 4) | (c.second % 10);
    p[1] = ((c.minute / 10) << 4) | (c.minute % 10);
    p[2] = ((c.hour / 10) << 4) | (c.hour % 10);
    p[3] = ((c.day / 10) << 4) | (c.day % 10);
    p[4] = c.week;
    cent = (c.year >= 2100) ? 1 : 0;
    p[5] = (cent << 7 | (c.month / 10) << 4) | (c.month % 10);
    p[6] = ((c.year % 100 / 10) << 4) | (c.year % 10);

    RCT_TIME_Write(RCT_TIME_ADDR,RCT_TIME_REG, p, NUMBER);
}

// ���������������˺����ͻ�������̣�Ҫ���ú��������ֵ��Ҫ��ָ��
void RCT_TIME_get_clock(Clock_t * c) {
    u8 p[NUMBER] = {0};
    u8 cent;
    //�ṹ��
//��ȡ����
    RCT_TIME_Read(RCT_TIME_ADDR,RCT_TIME_REG,p,NUMBER);
    //��Ϊ�趨��BCD��ʽ��4λ��������Ϊһλʮ�����������Ի�ȡʮλ������Ҫ��ǰ4λ�����Ƶ�����λ��ȡ�������� *10 ת����ʮ���Ƶ�ʮλ
    //second �� p[0] =  VL 1 1 1   0 0 0 0  Ҫ��ȡʮλ��ֻ���ȡǰ3λ  &0x07(0 0 0 0   0 1 1 1)��Ϊ�˰�VLȥ�� ����ǰ�治�õ�λ���Ͳ���Ҫ����
    c -> second = ((p[0] >> 4) & 0x07) * 10 + (p[0] & 0x0f);
    c -> minute = ((p[1] >> 4) & 0x07) * 10 + (p[1] & 0x0f);
    c -> hour   = ((p[2] >> 4) & 0x03) * 10 + (p[2] & 0x0f);
    c -> day    = ((p[3] >> 4) & 0x03) * 10 + (p[3] & 0x0f);
    c -> week   =   p[4] & 0x07;
    c -> month  = ((p[5] >> 4) & 0x01) * 10 + (p[5] & 0x0f);
    cent   =   p[5] >> 7;
    c -> year   = ((p[6] >> 4) & 0x0f) * 10 + (p[6] & 0x0f);
    //�껹Ҫ�������������λ��
    c -> year   +=  ((cent == 0) ? 2000 : 2100);
}

void set_alarm(alarm a) {
    u8 p[4];

    // �� M 1 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.minute >= 0) {
        p[0] = ((a.minute / 10) << 4) + (a.minute % 10);
    } else {
        p[0] = 0x80;
    }
    // ʱ H X 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.hour >= 0) {
        p[1] = ((a.hour / 10) << 4) + (a.hour % 10);
    } else {
        p[1] = 0x80;
    }
    // �� D X 1 1 - 0 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.day >= 0) {
        p[2] = ((a.day / 10) << 4) + (a.day % 10);
    } else {
        p[2] = 0x80;
    }
    // �� W X X X - X 0 0 0 		Enable -> 0, Disable -> 1 << 7 (0x80)
    if(a.week >= 0) {
        p[3] = a.week;
    } else {
        p[3] = 0x80;
    }

    RCT_TIME_Write(RCT_TIME_ADDR, 0x09, p, 4);

}
//0x09�Ǳ����Ĵ�����λ�ã�0x01�ǿ��ƼĴ�����λ�á�
//д�������Ĵ�����ͨ�����ƼĴ����Ŀ�������ȡ

void clear_alarm() {
    u8 cs2;
    // ���Alarm��ǣ�AF����Ϊ0���´����ӵ�����ܴ���INT
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);//�Ȱ���������ֵ��cs2���������п���֮ǰ����������
    //�����cs2��ֵ��0x00,����������Ƕ�������cs2
    //���Alarm���,AF��0
    cs2 &= ~(1 << 3);				//0x03
    // д��cs2�Ĵ���
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &cs2, 1);

}

void get_alarm(u8 enable) {
    u8 cs2;

// �������� 0x01 cs2--------------------------------------------------------
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);//�Ȱ���������ֵ��cs2���������п���֮ǰ����������
    //���Alarm���,AF��0
    //�����cs2��ֵ��0x00,����������Ƕ�������cs2
    cs2 &= ~(1 << 3);				//0x03
    // ����Alarm�жϣ�AIE����Ϊ1
    if(enable) {
        cs2 |= (1 << 1);	// 0x02
    } else {
        cs2 &=~(1 << 1);	// 0x02
    }
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &cs2, 1);

}


// ���ö�ʱ��
void set_Timer(TimerFreq freq, u8 countdown) {
    u8 tim;
    //����Timer����ֵ
    tim = (1 << 7) + freq;
    RCT_TIME_Write(RCT_TIME_ADDR, 0x0E, &tim, 1);
    tim = countdown;
    RCT_TIME_Write(RCT_TIME_ADDR, 0x0f, &tim, 1);
}

// ���ö�ʱ��
void get_Timer(u8 enable) {
    u8 tim;
    //����Timer
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &tim, 1);//�Ȱ���������ֵ��cs2���������п���֮ǰ����������
    //���Alarm���,AF��0
    //�����cs2��ֵ��0x00,����������Ƕ�������cs2
    tim &= ~(1 << 2);				//0x02
    // ����Timer�жϣ�TIE����Ϊ1
    if(enable) {
        tim |= (1 << 0);	// 0x01
    } else {
        tim &=~(1 << 0);	// 0x01
    }
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &tim, 1);

}

// �����ʱ��
void clear_Timer() {
    u8 tim;
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &tim, 1);
    //���Timer���,TF��0
    //�����cs2��ֵ��0x00,����������Ƕ�������cs2
    tim &= ~(1 << 2);				//0x02
    RCT_TIME_Write(RCT_TIME_ADDR, 0x01, &tim, 1);
}

void ext_int3_call(void) {
#if USE_ALARM || USE_TIMER
    u8 cs2;
    // ���Alarm��ǣ�AF����Ϊ0���´����ӵ�����ܴ���INT
    RCT_TIME_Read(RCT_TIME_ADDR, 0x01, &cs2, 1);
#if USE_ALARM
    //AIE && AF
    if( (cs2 & 0x02) &&(cs2 & 0x08) ) {
        //����Alarm��AF���
        clear_alarm();
        //���жϴ���ʱ��ʵ���߼�
        TRC_on_alarm();
    }
#endif

#if USE_TIMER
    //TIE && TF
    if((cs2 & 0x01) && (cs2 & 0x04)) {
        clear_Timer();
        TRC_on_timer();
    }

#endif
#endif

}








