

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD_I2C.h.h"

void TWIint()
{
	
	TWSR=0x00;
	TWBR=18;
	TWCR=0x04;
	
}

void TWIstart()
{
	
	TWCR=0xA4;
	while((TWCR & 0x80)==0);
	
}

void TWIstop()
{
	
	TWCR=0x94;
}


void TWIwrite(char dat)
{
	
	TWDR=dat;
	TWCR=0x84;
	while((TWCR & 0x80)==0);
	
	
}

char TWIreadack()
{
	TWCR=0xC4;
	while((TWCR & 0x80)==0);
	return TWDR;
	
}

char TWIreadnack()
{
	TWCR=0x84;
	
	while((TWCR & 0x80)==0);
	

	return TWDR;
	
}

/*
void RTCwritebyte(char loc,char data)
{
	
	_delay_ms(10);
	
	TWIstart();
	TWIwrite(0xD0);
	TWIwrite(loc);
	TWIwrite(data);
	
	TWIstop();

	
}
*/

/*
void LCDtwi(char data)
{
	
	
	TWIstart();

	TWIwrite(0x3F);    //write address of i2c module

	
	TWIwrite(data);
	
	TWIstop();

	
}



void LCDwritecmd(char data1)
{

char uc=0x00;

	uc = data1|0xf0;   // d4 to d7
	uc |= 0x08;     //back light
	uc &=(~0x01);   // rs=0 command
	
LCDtwi(uc);

	uc |= 0x04;
LCDtwi(uc);
	_delay_us(500);
	uc &= (~0x04);
LCDtwi(uc);

	_delay_us(4100);



 uc=0x00;

	uc = (data1<<4)|0xf0;   // d4 to d7
	uc |= 0x08;     //back light
	uc &=(~0x01);   // rs=0 command
	
LCDtwi(uc);

	uc |= 0x04;
LCDtwi(uc);
	_delay_us(500);
	uc &= (~0x04);
LCDtwi(uc);

	_delay_us(500);
	
}




void LCDwritedata(char data1)
{

char uc=0x00;

	uc = data1|0xf0;   // d4 to d7
	uc |= 0x08;     //back light
	uc |=(0x01);   // rs=1 data
	
LCDtwi(uc);

	uc |= 0x04;
LCDtwi(uc);
	_delay_us(500);
	uc &= (~0x04);
LCDtwi(uc);

	_delay_us(4100);



 uc=0x00;

	uc = (data1<<4)|0xf0;   // d4 to d7
	uc |= 0x08;     //back light
	uc |=(0x01);   // rs=1 data
	
LCDtwi(uc);

	uc |= 0x04;
LCDtwi(uc);
	_delay_us(500);
	uc &= (~0x04);
LCDtwi(uc);

	_delay_us(500);
	
}



*/



/*

void lcd_command(char cmd)
{
	
	PORTB&=(~0x01);
	PORTD=cmd;
	PORTB|=0x02;
	_delay_ms(5);
	PORTB&=(~0x02);
	_delay_ms(5);
}


void lcd_data(char data)
{
	
	PORTB|=0x01;
	PORTD=data;
	PORTB|=0x02;
	_delay_ms(5);
	PORTB&=(~0x02);
	_delay_ms(5);
}
*/

void lcd_print(char *ptr)
{
	
	while(*ptr!='\0')
	{
		
		write(*ptr);
		ptr++;
		_delay_ms(10);
	}


}

void dtoa(unsigned int val,char *buf,char n)
{
	
	buf[n]=0;
	while(n-->0)
	{
		
		buf[n]=(val%10)+'0';
		val=val/10;
	}
}

int main(void)
{
	
	
/*	
	UBRRL=103;
	UBRRH=0;
	UCSRA=0x00;
	UCSRB=0x98;
	UCSRC=0x86;
	sei();
*/
	//TWIint();
	
	int a=1234;
	char dispbuf[5];
	DDRB=0x03;
	DDRD=0xFF;
	_delay_ms(5);

///////////////////////////////////////////////////////////////////
/*
	LCDwritecmd(0x30);
	_delay_ms(5);
	LCDwritecmd(0x30);
	_delay_ms(5);
	LCDwritecmd(0x30);
	_delay_ms(5);
	LCDwritecmd(0x38);
	_delay_ms(5);


	LCDwritecmd(0x01);
	LCDwritecmd(0x06);
	LCDwritecmd(0X0C);
	LCDwritecmd(0x80);
	
	*/
//////////////////////////////////////////////////////////////////////////	
	//functions:    lcd_print( ),   dtoa(xx,dispbuf,n)
	
	
	//lcd_data('n');
	
	
	
//	LCDwritedata('n');

lcd_init(0x3f,16,2,0);
lcd_begin();
lcd_backlight();
lcd_print("nikhil joseph jo");
	


	//dtoa(a,dispbuf,4);
	//	lcd_print(dispbuf);
	
	

	
	
	while(1);
	{
		

	}
	
	
	
	
	
}