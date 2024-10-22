//定义寄存器格式：(special function register） sfr 端口 = 地址
//声明：P5端口配置寄存器1
sfr P5M1 = 0xC9;
//声明：P5端口配置寄存器0
sfr P5M0 = 0xCA;

//声明：P5端口寄存器
sfr P5 = 0xC8;

//定义特殊功能位格式：sbit 引脚 = 地址
sbit P53 = P5^3;

void Delay1000ms(void)	//@24.000MHz
{
	unsigned char data i, j, k;

	i = 122;
	j = 193;
	k = 128;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(){
//	//方式一：配置IO模式，推挽输出
//	P5_MODE_OUT_PP(GPIO_Pin_3);

	  P5M1 &= ~0x08;
    P5M0 |= 0x08;
		while(1){
			P53 = 1;
			Delay1000ms();
			P53 = 0;
			Delay1000ms();

		};
	
}
	