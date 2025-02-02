#include "DHT11.H"
#include "Delay.h"
#include "GPIO.h"



/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

    GPIO_InitStructure.Pin  = DHT_PIN;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;		  //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(DHT_PORT,&GPIO_InitStructure);	//初始化

}
void DHT11_init() {
    GPIO_config();

}

void Delay1us(void)	//@24.000MHz
{
    unsigned char data i;

    i = 5;
    while (--i);
}

void delay1_us(void)	//@24.000MHz
{
    NOP12();
}

// 读取DHT11数据(千万不要随意打日志，会耗时)

#define wait_level_change(level,min,max,desc)                          \
    cnt = 0;                                                           \
    while( DHT == level) {                                             \
        delay1_us();                                                   \
        cnt++;                                                         \
																																			 \
    }                                                                  \
    if( cnt <min || cnt > max) {                                       \
        printf("err:时间[%dus],不满足%s[%d,%d]\n",          						 \
               (int)cnt,desc,(int)min,(int)max);                       \
																																			 \
    }                                                                  \

char on_read(u8 * dat) {
    char i,j;
    u16 cnt = 0;//计时器，每+1，过了1us，一定要初始化为0

    //1.主机起始信号（拉低10~30ms）
    DHT = 0;
    delay_ms(20);
    //然后拉高
    DHT = 1;

    //2.等待主机释放总线(10~35ms)
    cnt = 0;
    while(DHT == 1 && cnt < 40) {
        delay1_us();
        cnt++;

    }
    if( cnt < 8 || cnt > 37) {
        printf("err:时间[%dus]不满足主机释放总线时间[%d,%d]\n",
               (int)cnt, (int)8, (int)37);
        return -1;
    }

    //3.从机响应低电平时间，[78,88]us,当前0，直到1，结束循环
    wait_level_change(0,78,88,"响应低电平时间");

    // 4. 响应高电平时间，87us [80, 92]us, 当前1，直到0 结束循环
    wait_level_change(1,80,92,"响应高电平时间");

    for(i = 0; i < 5; i++ ) {
        //高位先输出，也就是第7位先输出、
        for(j = 7; j>= 0; j--) {
            //5.数据位：低电平时间54us[50,58]us,当前0，直到1，结束循环
            wait_level_change(0,48,64,"DATA信号低电平时间");

            //6.数据位：0的高电平时间[23,27],1的高电平时间[68,74],当前1，直到0，结束循环

            wait_level_change(1,23,74,"DATA信号低电平时间");

            if( cnt > 50 ) { //高电平时间较长，是1
                //初始化默认i是0 0 0 0		0 0 0 0
                //这个判断是用来判断索引i字节的第j号位是1的话就放进去
                dat[i] |= (1 << j);
            }

        }

    }
    //主机释放总线
    DHT = 1;
//    printf("cnt:%d\n",cnt);
//    for( i = 0; i < 5; i++) {
//        printf("%d ",(int)dat[i]);
//    }
//    printf("\n");

    //校验数据
    if(dat[0] +dat[1] + dat[2] +dat[3] != dat[4]) {
        printf("check failure!\n");
    }

    return 0 ;
}

char DHT11_get_info(float * humidity,float * temperature) {
    u8 dat[5] = {0};//容器存放40bit数据，也就是40 / 8 = 5个字节

    char rst = on_read(dat);

    if(rst != 0) {	   
        return rst;
    }
		//取出湿度的整数部分 + 小数部分 * 0.1
    *humidity = dat[0] + dat[1] * 0.1f;

    //取出温度的整数部分 + 小数部分(低7位) * 0.1
    *temperature = dat[2] +(dat[3] & 0x7f) * 0.1f;

    //温度小数部分的最高位：0表示正温度，1表示负温度
    if(dat[3] >> 7) {
        *temperature *= -1;
    }
   


    return 0 ;
}