/***************************************************************************
���⣺	ADC�ɼ���������
ʵ��壺����ʱ���׼�
���ߣ�	
�ͷ�QQ��
���䣺	
˵����	�������ѧϰʹ��
******************************************************************************/

#include <reg52.h>

sfr ADC_CONTR = 0XBC;	//ת�����ƼĴ���
sfr ADC_RES = 0XBD;		//ת�������8λ
sfr ADC_LOW2 = 0XBE;	//ת�������2λ
sfr P1ASF = 0X9D;			//I/O��ģʽ

#define ADC_POWER 0X80		//��Դλ
#define ADC_FLAG 	0X10		//����λ
#define ADC_START 0X08		//����λ

//=======��ʱ============
void ADdelay(unsigned char i)
{
	while(i--);
}
//=======��ʼ��=========
void InitADC()
{
	P1ASF=0xC0;    				//��P1.6��P1.7ΪADCת��
	ADC_RES=0;
	ADC_CONTR=ADC_POWER; //����ADC��Դ
	ADdelay(200);	
}
//=======P1ADת������=======
void ADCP1STA(unsigned char lch)
{
	ADC_CONTR=ADC_POWER|ADC_START|lch; //ѡ��һ·����ת���ɼ�
}
//=======��ȡת�����=======
unsigned char GetADC(void)
{
	while(!(ADC_CONTR&ADC_FLAG));
	ADC_CONTR&=~ADC_FLAG;      //ֹͣADת��
	return ADC_RES;
}
