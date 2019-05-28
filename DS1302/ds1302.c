#include <STC15F2K60S2.H>
#include "ds1302.h"
/********************************************************************/ 
/*单字节写入一字节数据*/
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
		
		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
		SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
		else 
		SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
		Delay5us();	
		
		SCK_SET;
		Delay5us();	
		
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}

/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
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
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/
	Delay5us();	
	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/	
	Write_Ds1302_Byte(dat);	 /*写入数据：dat*/
	RST_CLR;				 /*停止DS1302总线*/
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/
	Delay5us();	
	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/		
	RST_CLR;	/*停止DS1302总线*/
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