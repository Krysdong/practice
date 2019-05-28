#ifndef _ONEWIRE_H
#define _ONEWIRE_H
#include <STC15F2K60S2.H>
//#include "reg52.h"

#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

//IC引脚定义
sbit DQ = P1^4;

//函数声明
extern void Delay_OneWire(unsigned int t);
extern void Write_DS18B20(unsigned char dat);
extern bit Init_DS18B20(void);
extern unsigned char Read_DS18B20(void);
extern unsigned long DS18B20_temp();
#endif