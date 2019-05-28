#include <STC15F2K60S2.H>
#include <INTRINS.H>

sbit SCL=P2^0;
sbit SDA=P2^1;


void Delay5us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 12;
	while (--i);
}


void i2c_start()
{
	SDA=1;
	SCL=1;
	Delay5us();
	SDA=0;
	Delay5us();
}

void i2c_stop()
{
	SDA=0;
	SCL=1;
	Delay5us();
	SDA=1;
	Delay5us();
}

void i2c_write_bit(bit b)
{
	SCL=0;
	SDA=b;
	Delay5us();
	
	SCL=1;
	Delay5us();
	
	SCL=0;
	Delay5us();
}

bit i2c_read_bit()
{
	bit b;
	
	SDA=1;
	
	SCL=0;
	Delay5us();
	
	SCL=1;
	Delay5us();
	
	b=SDA;
	
	SCL=0;
	Delay5us();
	
	return b;
}

bit i2c_read_ack()
{
	bit b;
	
	SCL=0;
	Delay5us();
	SDA=1;
	Delay5us();
	
	SCL=1;
	Delay5us();
	
	b=SDA;
	
	SCL=0;
	Delay5us();
	
	return b;
}

void i2c_write_ack(bit b)
{
	SCL=0;
	Delay5us();
	
	SDA=b;
	Delay5us();
	
	SCL=1;
	Delay5us();
	
	SCL=0;
	Delay5us();
	
}

#define BIT(x,n) ((x&(1<<n))!=0)
void i2c_write_byte(unsigned char dat)
{
	char i;
	for(i=7;i>=0;i--)
		i2c_write_bit(BIT(dat,i));
}

unsigned char i2c_read_byte()
{
	unsigned char dat=0x00;
	char i;
	for(i=7;i>=0;i--)
		if(i2c_read_bit())
			dat|=(1<<i);

	return dat;
}


unsigned char at24c02_write_byte(unsigned char addr,unsigned char dat)
{
	i2c_start();
	
	i2c_write_byte(0xA0);
	if(i2c_read_ack()!=0)
		return 1;
	
	i2c_write_byte(addr);
	if(i2c_read_ack()!=0)
		return 2;
	
	i2c_write_byte(dat);
	if(i2c_read_ack()!=0)
		return 3;
	
	i2c_stop();
	
	return 0;
}

unsigned short at24c02_read_byte(unsigned char addr)
{
	unsigned char dat;
	
	i2c_start();
	
	i2c_write_byte(0xA0);
	if(i2c_read_ack()!=0)
		return 300;
	
	i2c_write_byte(addr);
	if(i2c_read_ack()!=0)
		return 301;
	
	i2c_start();
	
	i2c_write_byte(0xA1);
	if(i2c_read_ack()!=0)
		return 302;
	
	dat=i2c_read_byte();
	i2c_write_ack(1);
	
	i2c_stop();
	
	return dat;
}
