#include "NTC.H"
#include "GPIO.h"
#include "ADC.h"
#include	"NVIC.h"

/**********************
通过热敏电阻读取环境温度

1、通过ADC读取热敏电阻的电压值
	-读取得到ADC数字
	-计算得到电压值

2、通过欧姆定律计算得到热敏电阻阻值

3、根据阻值表得到温度值

*****************************/




/******************* IO配置函数 *******************/
static void	GPIO_config(void)
{
    //P04
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = NTC_GPIO_PIN;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_HighZ;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(NTC_GPIO,&GPIO_InitStructure);	//初始化
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

//初始化

void NTC_init(void) {

    ADC_config();
    GPIO_config();
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

int search_temp(float rst_Rx100) {

    int i, min_index = 0; // 距离最近的电阻值的元素索引

    //设置一个最小距离用以记录
    float min_distance = abs(rst_Rx100 - temp_table[0]);//设置一个最小值

    //计算数组长度
    int len = sizeof(temp_table) / sizeof(temp_table[0]);


    for(i = 0; i < len; i++) {

        float distance = abs(rst_Rx100 - temp_table[i]);//计算的距离值

        //有距离更近的数值
        if(min_distance > distance) {
            min_distance = distance;
            min_index = i;
        }


    }

    return min_index;
}

int NTC_get_temperature(void) {

    u16 adc_value ;
    float rst_V,rst_R;
    int rst_T;
//	1、通过ADC读取热敏电阻的电压值
    // - 读取12通道的ADC值
    adc_value =  Get_ADCResult(NTC_ADC_CH);
    // - 计算得到电压值 adc_value = 4096 * (rst_V / 2.5)
    rst_V = adc_value * 2.5 / 4096;//输入电压


//2、通过欧姆定律计算得到热敏电阻阻值
    //R6
    rst_R = 10 * rst_V / (3.3 - rst_V);

//3、根据阻值表得到温度值
    rst_T = search_temp(rst_R * 100) - 55;
	printf("温度： %d ℃\n", rst_T);

    return rst_T;


}

