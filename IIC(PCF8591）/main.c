#include <STC15F2K60S2.H>
#include "IIC.h"
#define Y(n) P2=P2&0x1F|(n<<5),P2=P2&0x1F;
unsigned char digit[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned char num[8];



void buzz_relay(bit i,bit j)
{
    if(i==0&&j==0)
    {
        P0&=~((1<<4)|(1<<6));
        Y(5);
    }
    else if(i==1&&j==1)
    {
        P0|=(1<<4)|(1<<6);
        Y(5);
    }
    else if(i==1&&j==0)
    {
        P0|=(1<<6);
        P0&=~(1<<4);
        Y(5);
    }
    else if(j==0&&j==1)
    {
        P0&=~(1<<6);
        P0|=(1<<4);
        Y(5);
    }  
}
void display()
{
    static char i=0;
    
    P0=0xFF;
    Y(7);
    
    P0=1<<i;
    Y(6);
    
    P0=digit[num[i]];
    Y(7);
    
    if(++i==8)
        i=0;
}
void digit0num(unsigned long n)
{
    signed char i;
    for(i=7;i>=0;i--)
    {
        num[i]=n%10;
        n/=10;
    }
}
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
    TH0=(65536-2000)>>8;
    TL0=(65536-2000)&0x0F;
    display();
     
       
    
}
int main()
{unsigned long n;
    timer0_init();
    buzz_relay(0,0); 
    
    while(1)
    {
        
        //digit0num(pcf8591(1));//ds18b20_read_value());   
    }
}