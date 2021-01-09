
#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "LCD_I2C.h.h"

#define WDTO_4S 8

////////////////////////////////////////////////////////////////////         GLOBAL VARIABLES
volatile int count=0;
volatile unsigned char value;
volatile char flag=0;
volatile int kount=0;
char com[20];
char start;
int i;



void send_char(volatile unsigned char c);
void prints(char *ptr);
void TWIint();
void TWIstart();
void TWIstop();
void TWIwrite(char dat);
void lcd_print(char *ptr);
void dtoa(unsigned int val,char *buf,char n);
void uart_init();
void timer_init();
void port_init();
void struct_int();
void alarm_check();
void station_sweep();






char dispbuf[10];
/////////////////////////////////////////////////////////////////////
struct gasstations
{
	char station_id;
	char fault_status;
	volatile char alarm_status;
	char valid_transmission;
	char ot_flag;
	char data_inp;
}station[10];
/////////////////////////////////////////////////////////////////////////////////////
ISR(USART_RX_vect)
{
	
	value=UDR0;
	
	if(start==0)
	{
		if(value=='*')
		{
			start=1;
			i=0;
		}
	}
	else
	{
		if(value=='&')
		{
			start=0;
			com[i]='\0';
			flag=1;
			i=0;
		}
		else
		{
			com[i++]=value;
		}
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(value=='s')
{
	prints(" \n");

	for(int j=0;j<10;j++)
		{
			dtoa(station[j].station_id,dispbuf,2);
			
			prints("S.I: ");
			prints(dispbuf);
			prints("  ");
			
			dtoa(station[j].fault_status,dispbuf,2);
			prints("F.S: ");
			prints(dispbuf);
			prints("  ");
			
			dtoa(station[j].alarm_status,dispbuf,2);
			prints("A.S: ");
			prints(dispbuf);
			prints("  ");
			
			dtoa(station[j].valid_transmission,dispbuf,2);
			prints("V.T: ");
			prints(dispbuf);
			prints("  ");
			
			dtoa(station[j].ot_flag,dispbuf,2);
			prints("O.T.F: ");
			prints(dispbuf);
			prints("  ");
			
			dtoa(station[j].data_inp,dispbuf,2);
			prints("DATA: ");
			prints(dispbuf);
			prints(" \n");
		}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
}

ISR(TIMER0_OVF_vect)
{
	
	count++;
	if(count==100)
	{
		
//		prints("\ntimer overflow\n");

			//for(k=0;k<10;k++)
			//{
				if(station[kount].fault_status==1)
				{			
							lcd_clear();
							lcd_setCursor(0,0);
							lcd_print(" GAS LEAK FOUND");
							
							lcd_setCursor(0,1);
							lcd_print("STATION FAULT-");
							dtoa(station[kount].station_id,dispbuf,2);
							lcd_print(dispbuf);
				}
				kount++;
				if(kount>=10)
				{
					kount=0;
				}
			//}




		count=0;
	}
	TCNT0=177;
	
	

}

ISR(INT0_vect)
{

//prints("\n interrupt\n");

	char k;
	for(k=0;k<10;k++)
	{
		if(station[k].alarm_status==1 && station[k].ot_flag==1)
		{
			//alarm mute
			station[k].alarm_status=0;
						lcd_clear();
						lcd_setCursor(0,0);
						lcd_print("     ALARM");
						lcd_setCursor(0,1);
						lcd_print("     MUTED");
						_delay_ms(1000);
												
			
			//PRINT ON LCD STATION NUMBER AND ALARM MUTE
			break;    //STOP ONLY ONE ALARM STATUS ON PRESSING MUTE SWITCH ONCE
		}
	}


	//_delay_ms(2000);
	EIFR=0x00;
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void send_char(volatile unsigned char c)
{
	
	UCSR0A=0x40;
	UDR0=c;
	while((UCSR0A & 0x40)==0);
}

void prints(char *ptr)
{
	
	while(*ptr!='\0')
	{
		
		send_char(*ptr);
		ptr++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
*/

void lcd_print(char *ptr)
{
	while(*ptr!='\0')
	{
		
		write(*ptr);
		ptr++;
		_delay_us(1);
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


void uart_init()
{
	UBRR0H=0;
	UBRR0L=103;
	
	UCSR0A=0x80;
	UCSR0B=0x98;
	UCSR0C=0x06;
	
	prints("UART READY");
}

void timer_init()
{
	TCCR0B=0x05;
	TCNT0=177;
	TIMSK0=0x01;
}

void interrupt_init()
{
	EICRA=0x00;
	EIMSK|=0x01;
}

void port_init()
{	
	////////////////////CHANGE ALL PORT DECLARATIONS AND INTITIALIZATIONS AS PER OUR REQUIREMENT
		
	//PORTB
	DDRB |= 0x3f;
	//	PORTB=0x00;         //INITIALIZE RELAY HERE         
	
	//PORTC
	DDRC |= 0x06;
	DDRC &= (~0x09);
	
	//PORTD
	DDRD |= 0xf0;
	DDRD &= (~0x04);        //HAVE DOUBT WHETHER PORT HAS TO BE DECLARED INPUT WHILE USING INTERRUPT	
}


void struct_int()
{
	char j;
	for(j=0;j<10;j++)
	{
		station[j].station_id=j;
		station[j].fault_status=0x00;
		station[j].alarm_status=0x00;
		station[j].valid_transmission=0x00;
		station[j].ot_flag=0x00;
		station[j].data_inp=0x01;	
	}
}

//////////////////////////////////////////////////////DRIVES CONTROL RELAYS
void relay_check()
{
	char k;
	char relay_var=1;
	
	for(k=0;k<10;k++)
	{
		if(station[k].fault_status==1)
		{
			relay_var=0;
		}
	}
	
	if(relay_var==0)
	{
		
		// DRIVE THE PORT FOR RELAYS FOR THE SOLINOIDS................................
		prints("\nrelays off\n");
	}
	
	else
	{
		// DRIVE THE PORT FOR RELAYS FOR THE SOLINOIDS................................
		prints("\nrelays on\n");
	}
	
}

void alarm_check()
{
	char k;
	char alarm_var=0;
	
	for(k=0;k<10;k++)
	{
		if(station[k].alarm_status==1)
		{
			alarm_var=1;
		}
	}
	
	if(alarm_var==1)
	{
		PORTC|=0X04;               //FAULT LED ON
		// DRIVE THE PORT FOR ALARM................................
		prints("\nalarm led on\n");
	}
	
	else
	{
		PORTC &= (~0X04);               //FAULT LED OFF
		// DRIVE THE PORT FOR ALARM................................
		prints("\nalarm led off\n");
	}
	
}

void station_sweep()
{
	char k;
	for(k=0;k<10;k++)
	{
		//set ht12e address
		PORTB &= (~0x0f);		
		PORTD &= (~0xf0);
		
		_delay_us(10);
		
		PORTB |= (station[k].station_id & 0x0f);
		PORTD |= (station[k].station_id & 0xf0);
	
		_delay_ms(300);
		//check if transmiter and reciever paired
		if((PINC & 0x01) == 0x00)
		{
			station[k].valid_transmission=0;
		}
		else if((PINC & 0x01) == 0x01)
		{
			station[k].valid_transmission=1;
		}
		
		//check data from receiver: 0- gas detected 1- normal working
		
		if(((PINC & 0x08) == 0x00) && station[k].ot_flag==0 && PINC & 0x01 == 0x01)
		{
			
//			prints("helloooooooooo ");
			//gas detected
			station[k].fault_status=1;
			station[k].alarm_status=1;
			station[k].ot_flag=1;
			station[k].data_inp=0;
			
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			
			
			
			
			//PRINT FAULT STATUS ON LCD
			lcd_clear();
			lcd_setCursor(0,0);
			lcd_print(" GAS LEAK FOUND");
			
			lcd_setCursor(0,1);
			lcd_print(" NEW FAULT-");
			dtoa(station[k].station_id,dispbuf,2);
			lcd_print(dispbuf);
			
			_delay_ms(2000);                  //REMOVE DELAY WHILE DEPLOYING
			
		}
		else if(((PINC & 0x08) == 0x08) && station[k].ot_flag==0 && PINC & 0x01 == 0x01)
		{
			
//			prints("hiiiiiiiii ");
			
			//gas not detected
			station[k].fault_status=0;
			station[k].alarm_status=0;
			station[k].data_inp=1;
		}
		
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
prints(" \n");
dtoa(station[k].station_id,dispbuf,2);

prints("S.I: ");
prints(dispbuf);
prints("  ");

dtoa(station[k].fault_status,dispbuf,2);
prints("F.S: ");
prints(dispbuf);
prints("  ");

dtoa(station[k].alarm_status,dispbuf,2);
prints("A.S: ");
prints(dispbuf);
prints("  ");

dtoa(station[k].valid_transmission,dispbuf,2);
prints("V.T: ");
prints(dispbuf);
prints("  ");

dtoa(station[k].ot_flag,dispbuf,2);
prints("O.T.F: ");
prints(dispbuf);
prints("  ");

dtoa(station[k].data_inp,dispbuf,2);
prints("DATA: ");
prints(dispbuf);
prints(" \n");

*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	_delay_ms(100);
	}	
}


int main(void)
{
		
	wdt_enable(WDTO_8S);

	lcd_init(0x27,16,2,0);
	lcd_begin();
	lcd_backlight();	
	
	
	port_init();
	
	timer_init();
	uart_init();
	struct_int();
	interrupt_init();
	sei();

	lcd_setCursor(0,0);
	lcd_print("  DEVICE READY");
	
//	_delay_ms(10000);
	while(1)
	{

		station_sweep();		
		alarm_check();
		relay_check();
				
		wdt_reset();
	}
	
}
