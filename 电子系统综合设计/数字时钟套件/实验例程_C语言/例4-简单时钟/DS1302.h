/***************************************************************************
���⣺	DS1302��������
ʵ��壺����ʱ���׼�
���ߣ�	
�ͷ�QQ��
���䣺	
˵����	�������ѧϰ�ο�
******************************************************************************/
#include <reg52.h>

/********IO���Ŷ���***********************************************************/
sbit SCK=P1^2;		//DS1302��SCLʱ������
sbit SDA=P1^1;		//DS1302��IO��
sbit RST = P1^0;  // DS1302��λ�ţ����Ե���Ƭѡ�ã�

/********���ݶ���*************************************************************/
code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; 	//дʱ����Ϣ��ַ
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};	//��ʱ����Ϣ��ַ

/************д�ֽڵ�DS1302*******************************************/
void Write_Ds1302_Byte(unsigned  char dat) 
{
	unsigned char i;
	for (i=0;i<8;i++)     		//дһ���ֽ�
	{ 
		SCK=0;
		SDA=dat&0x01;     		//��ȡ��λ���ݣ���λ��ǰ��λ�ں�
		dat>>=1;  					//����һλ
		SCK=1;
	}
	SDA=1;
}   

/************���ݵ�ַд����*************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	SCK=0;						//׼��
 	RST=1;						//��ʼ

 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	RST=0;  					//�ָ�
}

/***********���ݵ�ַ������*************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,la=0x00;
 	SCK=0;
	SDA=1;
 	RST=1;

 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 			//��һ�ֽ�
 	{
		la>>=1;							//����һλ����λ��ǰ��λ�ں�
		SCK=0;			  
 		if(SDA)
 			la|=0x80;					//��λ��ǰ��λ�ں�����1ʱ�Ӹ�λ����
		SCK=1;						
	} 
 	RST=0;
	return (la);				//������Ӧֵ
}

/**********���룬�֣�ʱ���գ��£����ڣ���*************************************/
void Read_RTC(unsigned char *ptimer,unsigned char llen)		//��ȡ �������ɰ���������ȡ˳�����룬�֣�ʱ���գ��£����ڣ���
{
 	unsigned char i,*p;
 	p=read_rtc_address; 				//����ַ
 	for(i=0;i<llen;i++)					//
 	{
 		 ptimer[i]=Read_Ds1302(*p);
 		 p++;
 	}
}

/**********д�룬�֣�ʱ���գ��£����ڣ���*************************************/
void Set_RTC(unsigned char *ptimer)
{
	unsigned char i,*p,tmp;
	for(i=0;i<7;i++){						//������תΪBCD��
		tmp=ptimer[i]/10;
		ptimer[i]=ptimer[i]%10;
		ptimer[i]=ptimer[i]+tmp*16;
	}
 	Write_Ds1302(0x8E,0X00);		//��д����
	
 	p=write_rtc_address;				//д��ַ	
	for(i=0;i<7;i++)						//���7��д�� ˳�����룬�֣�ʱ���գ��£����ڣ���
	{
		Write_Ds1302(*p,ptimer[i]);
		p++;  
	}
	Write_Ds1302(0x8E,0x80);		//�ر�д����
}