//����Ĵ�����ʽ��(special function register�� sfr �˿� = ��ַ
//������P5�˿����üĴ���1
sfr P5M1 = 0xC9;
//������P5�˿����üĴ���0
sfr P5M0 = 0xCA;

//������P5�˿ڼĴ���
sfr P5 = 0xC8;

//�������⹦��λ��ʽ��sbit ���� = ��ַ
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
//	//��ʽһ������IOģʽ���������
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
	