#include <STC15F2K60S2.H>
#include <INTRINS.H>
#define Y(n) P2=P2&0x1F|(n<<5),P2=P2&0x1F
unsigned char digit[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char num[8];
sbit TX=P1^0;
sbit RX=P1^1;
void buzz_relay(char i,char j)
{
    if(i==0&&j==0)
    {
        P0&=~((1<<4)|(1<<6));
        Y(5);
    }
}
void Delay13us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 36;
	while (--i);
}

void display()
{
    static signed char i=0;
    
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
    ET1=0;
    TMOD=0x11;
    TH0=(65536-2000)>>8;
    TL0=(65536-2000)&0x0F;
    TR0=1;
    TR1=0;
}
volatile bit keyflag0=0;
void timer0_isr() interrupt 1
{
    static unsigned short count0=0;
    TH0=(65536-2000)>>8;
    TL0=(65536-2000)&0x0F;
    display();
    if(++count0==250)
    {
        keyflag0=1;
        count0=0;
    }
    
}
void sendwave()  //·¢²¨
{
    unsigned char i;
    for(i=0;i<=7;i++)
    {
        TX=1;
        Delay13us();_nop_();_nop_();_nop_();_nop_();
        TX=0;
        Delay13us();_nop_();_nop_();_nop_();_nop_();
    }
}
unsigned long ultrasonic_distance()
{
    unsigned long distance;
    RX=1; 
    
    TH1=0;
    TL1=0;
    TF1=0;
    TR1=1;
    sendwave();
    
    while((RX==1)&&(TF1==0));
    TR1=0;
    
    distance=TF1?99999:((TH1<<8|TL1)*17UL/1000UL);
    return distance;
}
int main()
{
    buzz_relay(0,0);
    timer0_init();
    
    while(1)
    {
        if(keyflag0==1)
        {
            keyflag0=0;
            digit0num(ultrasonic_distance());
        }
    }      
}
