#include <STC15F2K60S2.H>
#include "ds1302.h"
#define Y(n) P2=P2&0x1F|(n<<5),P2=P2&0x1F
unsigned char digit[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0X82,0xF8,0x80,0x90,0xBF};
unsigned char num[8];

void buzz_relay_off()
{
	P0&=~((1<<4)|(1<<6));
	Y(5);
}
void digit0num(Time t)
{
    num[0]=t.hour/10;
	num[1]=t.hour%10;
	
	num[3]=t.min/10;
	num[4]=t.min%10;
	
	num[6]=t.sec/10;
	num[7]=t.sec%10;
	
	num[2]=10;
	num[5]=10;
}
void display()
{
    static char i=0;

    P0=0xFF;
    Y(7);
    
    P0=(1<<i);
    Y(6);
    
    P0=digit[num[i]];
    Y(7);
    
    if(++i==8)
        i=0;
}
bit flag=0;
void timer0_init()
{
    EA=1;
    ET0=1;
    TMOD=TMOD&0xF0|0x01;
    TH0=(65536-2000)>>8;
    TL0=(65536-2000)&0x0F;
    TR0=1;
}
void timer0_isr() interrupt 1
{
    static unsigned short count=0;
	display();
	TH0=(65536-2000)>>8;
	TL0=(65536-2000)&0X00FF;
	if(++count==50)
	{
		count=0;
		flag=1;
		
	}
}
int main()
{
    Time t={1,35,30};
    buzz_relay_off();
    timer0_init();
    set_time(t);
    while(1)
    {
       if(flag)
		{
			get_time(&t);
			digit0num(t);
			flag=0;
		}
       
        
    }
}