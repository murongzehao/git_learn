#include "config.h"
#include "GPIO.H"
#include "UART.h"
#include "Delay.h"
#include "Timer.h"
#include "switch.h"
#include "NVIC.h"
#include "adc.h"

#define LED_SW   P45
#define LED2 	   P26
#define LED4 	   P14
#define LED6 	   P22
#define LED8 	   P20
#define LED1 	   P27
#define LED3 	   P15
#define LED5 	   P23
#define LED7 	   P21

/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//初始化


    GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_2 | GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_3 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化

    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			  //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				  //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			  //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		  //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************* AD配置函数 *******************/
void	ADC_config(void)
{
    ADC_InitTypeDef		ADC_InitStructure;		//结构定义

    ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
    ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
    ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_Inilize(&ADC_InitStructure);		//初始化
    ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
    NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

u16 code temp_table[]= {
    58354, // -55
    55464, // -54
    52698, // -53
    50048, // -52
    47515, // -51
    45097, // -50
    42789, // -49
    40589, // -48
    38492, // -47
    36496, // -46
    34597, // -45
    32791, // -44
    31075, // -43
    29444, // -42
    27896, // -41
    26427, // -40
    25034, // -39
    23713, // -38
    22460, // -37
    21273, // -36
    20148, // -35
    19083, // -34
    18075, // -33
    17120, // -32
    16216, // -31
    15361, // -30
    14551, // -29
    13785, // -28
    13061, // -27
    12376, // -26
    11728, // -25
    11114, // -24
    10535, // -23
    9986,  // -22
    9468,  // -21
    8977,  // -20
    8513,  // -19
    8075,  // -18
    7660,  // -17
    7267,  // -16
    6896,  // -15
    6545,  // -14
    6212,  // -13
    5898,  // -12
    5601,  // -11
    5319,  // -10
    5053,  // -9
    4801,  // -8
    4562,  // -7
    4336,  // -6
    4122,  // -5
    3920,  // -4
    3728,  // -3
    3546,  // -2
    3374,  // -1
    3211,  // 0
    3057,  // 1
    2910,  // 2
    2771,  // 3
    2639,  // 4
    2515,  // 5
    2396,  // 6
    2284,  // 7
    2177,  // 8
    2076,  // 9
    1978,  // 10
    1889,  // 11
    1802,  // 12
    1720,  // 13
    1642,  // 14
    1568,  // 15
    1497,  // 16
    1430,  // 17
    1366,  // 18
    1306,  // 19
    1248,  // 20
    1193,  // 21
    1141,  // 22
    1092,  // 23
    1044,  // 24
    1000,  // 25
    957,   // 26
    916,   // 27
    877,   // 28
    840,   // 29
    805,   // 30
    771,   // 31
    739,   // 32
    709,   // 33
    679,   // 34
    652,   // 35
    625,   // 36
    600,   // 37
    576,   // 38
    552,   // 39
    530,   // 40
    509,   // 41
    489,   // 42
    470,   // 43
    452,   // 44
    434,   // 45
    417,   // 46
    401,   // 47
    386,   // 48
    371,   // 49
    358,   // 50
    344,   // 51
    331,   // 52
    318,   // 53
    306,   // 54
    295,   // 55
    284,   // 56
    274,   // 57
    264,   // 58
    254,   // 59
    245,   // 60
    236,   // 61
    228,   // 62
    220,   // 63
    212,   // 64
    205,   // 65
    198,   // 66
    191,   // 67
    184,   // 68
    178,   // 69
    172,   // 70
    166,   // 71
    160,   // 72
    155,   // 73
    150,   // 74
    145,   // 75
    140,   // 76
    135,   // 77
    131,   // 78
    126,   // 79
    122,   // 80
    118,   // 81
    115,   // 82
    111,   // 83
    107,   // 84
    104,   // 85
    101,   // 86
    97,    // 87
    94,    // 88
    91,    // 89
    89,    // 90
    86,    // 91
    83,    // 92
    81,    // 93
    78,    // 94
    76,    // 95
    74,    // 96
    71,    // 97
    69,    // 98
    67,    // 99
    65,    // 100
    63,    // 101
    61,    // 102
    60,    // 103
    58,    // 104
    56,    // 105
    55,    // 106
    53,    // 107
    52,    // 108
    50,    // 109
    49,    // 110
    47,    // 111
    46,    // 112
    45,    // 113
    43,    // 114
    42,    // 115
    41,    // 116
    40,    // 117
    39,    // 118
    38,    // 119
    37,    // 120
    36,    // 121
    35,    // 122
    34,    // 123
    33,    // 124
    32,    // 125
};



int search_temp() {
    float V_c,R_c;
    int T_c;
    u16 adc_value;
    u8 i,min_index;

    float min_distance = abs(R_c * 100 - temp_table[0]);

    int len = sizeof(temp_table) / sizeof(temp_table[0]);

    for( i = 0; i < len; i++) {
        float distance = abs(R_c * 100 - temp_table[i]);
        if( min_distance > distance) {
            min_distance = distance;
            min_index = i;
        }

    }
    //通过ADC引脚获取电压值
    adc_value = Get_ADCResult(ADC_CH12);
    //通过ADC的电压求出采样电压
    V_c = adc_value * 2.5 / 4096;
    //通过采样电压求出热敏阻值
    R_c = V_c * 10 /(3.3 - V_c);
    //通过阻值表算出温度
    T_c = min_index - 55;

    return T_c;
}

void main() {
    int T_c,t_c;

    EAXSFR();
    GPIO_config();
    UART_config();
    ADC_config();




    EA = 1;

    LED_SW = 0;

    delay_ms(250);
    t_c = search_temp();

    while(1) {
        T_c = search_temp();

        delay_ms(250);
        delay_ms(250);
        if(T_c >= t_c + 2) {
            LED1 = LED2 = LED3  = LED4  = LED5 = LED6  = LED7  = LED8 = 0;
            delay_ms(10);
            LED1 = LED2 = LED3  = LED4  = LED5 = LED6  = LED7  = LED8 = 1;
            delay_ms(10);
        }

        printf("何泽豪  %d度\n",T_c);
        printf("何泽豪  %d度\n",t_c);
    }
}