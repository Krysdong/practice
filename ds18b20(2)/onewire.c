/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台
  日    期: 2011-8-9
*/
#include <STC15F2K60S2.H>
#include "onewire.h"
#define K 8
//sbit DQ = P1^4;
//单总线延时函数
void Delay_OneWire(unsigned int t)
{
  while(t--);
}

//DS18B20芯片初始化
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12*K);  //1us
	DQ = 0;
	Delay_OneWire(80*K);  //480us
    
	DQ = 1;      //释放总权
    
	Delay_OneWire(10*K);   //60us
	initflag = DQ;        // 0
	Delay_OneWire(5*K);     //延满480us
  
	return initflag;      //0
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5*K);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5*K);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5*K);
	}
	return dat;
}
unsigned long DS18B20_temp()
{
    unsigned long n;
    unsigned short MSB,LSB;
    
    Init_DS18B20();
    
    Write_DS18B20(0xCC);
    Write_DS18B20(0x44);
    
    DQ=1;
    while(DQ==0);
        
    Init_DS18B20();
    
    Write_DS18B20(0xCC);
    Write_DS18B20(0xBE); 
    
    LSB=Read_DS18B20();
    MSB=Read_DS18B20();
    
    n=(MSB<<8)|LSB;
    	
	return n*10000UL>>4;
}
