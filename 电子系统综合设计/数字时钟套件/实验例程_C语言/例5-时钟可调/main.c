/***************************************************************************
���⣺	ʱ�ӳ���
ʵ��壺����ʱ���׼�
���ߣ�	оǶ
�ͷ�QQ��
���䣺	
˵����	ʱ����ʾ+�ɵ�
����������S2��ť����ʱ�����ã���˳��ӷ֣�ʱ���գ��£����ڣ�����е��ڣ�
      S1��ȷ�ϣ�S2���л���
	  S1����ֵ��1�ı䣬ֱ����ȫ�����ٰ�S2���б����˳�
******************************************************************************/

//ͷ�ļ�:
#include <reg52.h>
#include "SMG4.h"
#include "DS1302.h"

//���Ŷ���:
sbit S1 = P3^1;	//���尴��1
sbit S2 = P3^0;	//���尴��2

sbit SPK = P1^5;	//��������������

//��������
code unsigned char ly_Tup[]={59,59,23,31,12,7,99};		//���õ���ʱʱ�����ֵ�ж�
							//��=59����=59	��ʱ=23����=31����=12������=7����=99

//��������
unsigned char ly_timer[7]={0,20,12,9,7,4,15};	//0�룬20�֣�12ʱ��9�գ�7�£�����4��15��			
																							//���ڶ�ʱ����Ϣ���壬��ֵΪ��һ�ε�д��ʱ��				
unsigned char ly_key;				//�����ֵ
unsigned char ly_keyon=0;		//��Ϊ��ť�Ƿ��ɿ����ж�


//��ʱ�Ӻ���
void delay(unsigned char i){
	while(i--);
}

//ɨ���������
void ReadKey(void)
{
	if(!(S1&S2))					//���ж��Ƿ��м�����
	{
		SPK=0;							//�м����£�����������ʾ��
		delay(100);					//�������ʱ����������
		if((!(S1&S2))&&(ly_keyon==0))	//ȷ���а������� ���Ǹհ���ִ�У�����ִ��
		{			
			if(S1==0){				//����1����
				ly_key=1;				//��ֵ
				ly_keyon=1;			//�����꣬��������Ϊ����
			}
			else if(S2==0){	//����2���£�ͬ��
				ly_key=2;			
				ly_keyon=1;
			}
		}
	}
	else
	{
		ly_keyon=0;	//	�ް�ť����
		SPK=1; 			// �رշ�����
	}
}

//����ʱ���趨����
void SetRTC(void)
{
	unsigned char i=0,tmp;
	unsigned char lcc=1;		//��Ϊ����λ�õ������ţ���1Ҳ���ǡ��֡���ʼ���ã��벻������0
	unsigned char lt[7];		//ʱ�仺��

	ReadKey();							//����һ�μ���ɨ�裬Ȼ�󽫼�ֵ����0���������ð���Ӱ�����Ĵ���
	ly_key=0;
	
	for(i=1;i<7;i++){				//������BCD��תΪ������
		tmp=ly_timer[i]>>4;		
		lt[i]=ly_timer[i]&0x0f;
		lt[i]=tmp*10+lt[i];	
	}
	tmp=0;
	
	while(1){
		DisPlayS(15);					//������ʾ����ʱ����ʾ��������˸�Ŀ����������ʱ��������
		ReadKey();						//����ɨ��
		
		if(ly_key)						//�м����´���
		{
			switch(ly_key){
			case 1:								//1��ֵ��1����С���趨�����ֵʱ��1�������0
				if(lt[lcc]<ly_Tup[lcc])
					lt[lcc]++;
				else
					lt[lcc]=0;
				break;
			case 2:								//2���л�
				lcc++;
				if(lcc>6){					//��ʱ��ֵ��������ɺ󱣴��˳�
					lt[0]=0;   			 	//���0
					Set_RTC(lt);
					SPK=1;						//������������л���ʱ���������Ǳ��򿪵ģ������˳�ʱҪ���������ر�
					return;
				}
				break;
			}
			ly_key=0;
		}
		
		if(tmp==0)	//��˸Ч����ͨ����������ж���Ӧλ����ʾ�벻��ʾ��������Ϊ0ʱ��ʾ��Ϊ180ʱ�ر���ʾ
		{
			switch(lcc){		//�����������������ʾ��Ӧ������
			case 1:
			case 2:
				ly_SMGcp[1]=1;			//ʱ���м�����������
				ly_SMGcp[2]=1;
			
				ly_SMGbuf[0]=(lt[2]/10);		//ʱ
				ly_SMGbuf[1]=(lt[2]%10);

				ly_SMGbuf[2]=(lt[1]/10);		//��
				ly_SMGbuf[3]=(lt[1]%10);
				break;
			case 3:
			case 4:
				ly_SMGcp[1]=0;
				ly_SMGcp[2]=1;							//��ʾ�����¡�ʱ��һ�������
			
				ly_SMGbuf[0]=(lt[4]/10);		//��
				ly_SMGbuf[1]=(lt[4]%10);

				ly_SMGbuf[2]=(lt[3]/10);		//��
				ly_SMGbuf[3]=(lt[3]%10);
				break;
			case 5:
				ly_SMGcp[1]=0;
				ly_SMGcp[2]=0;						//ֻ��ʾһλ������ʾ��
				ly_SMGbuf[0]=10;					//�����10�ڶ�ֵ���е�ֵΪOXFF�����ر��������ʾ
				ly_SMGbuf[1]=10;
				ly_SMGbuf[2]=10;
			
				ly_SMGbuf[3]=lt[5];				//����
				break;
			case 6:
				ly_SMGbuf[0]=2;
				ly_SMGbuf[1]=0;			
				ly_SMGbuf[2]=(lt[6]/10);		//��
				ly_SMGbuf[3]=(lt[6]%10);
				break;	
			}
		}
		if(tmp==180)	//�ر���Ӧλ��ʾ���Դ���˸Ч��
		{
			switch(lcc){				
			case 1:
			case 3:
			case 5:
				ly_SMGbuf[2]=10;	//�����10�ڶ�ֵ���е�ֵΪOXFF�����ر��������ʾ
				ly_SMGbuf[3]=10;
				break;
			case 2:
			case 4:
				ly_SMGbuf[0]=10;	
				ly_SMGbuf[1]=10;
				break;
			case 6:
				ly_SMGbuf[0]=10;	
				ly_SMGbuf[1]=10;
				ly_SMGbuf[2]=10;	
				ly_SMGbuf[3]=10;
				break;
			}
		}
		tmp++;
	}
}


//������	
void main(void)  
{
	unsigned char oldT,DisUp=1;	//��Ϊʱ����µ���ʾ�Ŀ���
	
	RST=0;											//DS1302��RST�ǵ͵�ƽ��λ����˽���ر�

	Set_RTC(ly_timer);					//дʱ�䣬оƬ��ʼ����Ҫ��д��ʱ����в��ԣ��ǲ���ʱ����ȥ����
		
	while(1){
		if(S2==0){				//������2�������ʱ
			DisPlayS(5000);	//������ʾ������ʱΪĿ��
			if(S1==0){			//�ٴ�ȷ����������
				SetRTC();
				DisUp=1;			//������ɺ������ʾ
			}
		}
		
		if(DisUp){				//������ʾʱ��
			DisUp=0;
			Read_RTC(ly_timer,7);						//��ʱ����Ϣ
				
			ly_SMGbuf[0]=ly_timer[2]/16;		//��ȡСʱ�ĸ�λ��ʮλ��������ʾ�������ʾ
			ly_SMGbuf[1]=ly_timer[2]&0x0f;	//Ϊʲô���ǳ���10������һ��ע�⣬����������BCDʮ������
			ly_SMGbuf[2]=ly_timer[1]/16;		//��ȡ���ֵĸ�λ��ʮλ��������ʾ�������ʾ
			ly_SMGbuf[3]=ly_timer[1]&0x0f;
		}
		
		Read_RTC(ly_timer,1);					//��ʱ����Ϣ��	
		if(ly_timer[0]!=oldT)  				//����������м����˸
		{
			oldT=ly_timer[0];
			ly_SMGcp[1]=oldT&0x01;			//��ȡ��ĵ�0λ�����м�����С������˸
			ly_SMGcp[2]=ly_SMGcp[1];
			if(oldT==0)									//���0ʱ����ʱ����ʾ
				DisUp=1;
		}
		
		DisPlayS(100);	//������ʾ����
	}
}

