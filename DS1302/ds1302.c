#include <STC15F2K60S2.H>
#include "ds1302.h"
/********************************************************************/ 
/*���ֽ�д��һ�ֽ�����*/
void Delay5us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 12;
	while (--i);
}

void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	
	for (i=0;i<8;i++) 
	{ 	
		SCK = 0;
		Delay5us();
		
		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
		SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
		else 
		SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
		Delay5us();	
		
		SCK_SET;
		Delay5us();	
		
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}

/********************************************************************/ 
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
		dat |= 0x80;
		else 
		dat &= 0x7F;
		
		SCK_SET;
		Delay5us();
		SCK_CLR;
		Delay5us();	
	}
	return dat;
}

/********************************************************************/ 
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	Delay5us();	
	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/	
	Write_Ds1302_Byte(dat);	 /*д�����ݣ�dat*/
	RST_CLR;				 /*ֹͣDS1302����*/
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	Delay5us();	
	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/		
	RST_CLR;	/*ֹͣDS1302����*/
	return temp;
}

unsigned char NCD(unsigned char n)
{
	return ((n/10)<<4)+(n%10);
}

unsigned char BCD(unsigned char n)
{
	return (n>>4)*10+(n&0X0F);
}

void set_time(Time t)
{
	Ds1302_Single_Byte_Write(ds1302_control_addr,0x00);
	Ds1302_Single_Byte_Write(ds1302_charger_addr,0x00);
	
	Ds1302_Single_Byte_Write(ds1302_hr_addr,NCD(t.hour));
	Ds1302_Single_Byte_Write(ds1302_min_addr,NCD(t.min));
	Ds1302_Single_Byte_Write(ds1302_sec_addr,NCD(t.sec));
}

void Delay2ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

void get_time(Time *t)
{
	t->hour=BCD(Ds1302_Single_Byte_Read(ds1302_hr_addr));Delay2ms();
	t->min=BCD(Ds1302_Single_Byte_Read(ds1302_min_addr));Delay2ms();
	t->sec=BCD(Ds1302_Single_Byte_Read(ds1302_sec_addr));Delay2ms();
}