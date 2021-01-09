#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>


#define WDTO_4S 8

/////////////////////////////////////////////////////////////////         GLOBAL VARIABLES
volatile int count=0;

volatile unsigned char value;
volatile char flag=0;
char com[20];
char start;
int i;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct gasstations
{
	char station_id;
	char fault_status;
	volatile char alarm_status;
	char valid_transmission;
	char ot_flag;	
	char data_inp;	
}station[10];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////           ISR ROUTINES
ISR(USART_RXC_vect)
{
	value=UDR;	
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
}



ISR(TIMER0_OVF_vect)
{
	
	count++;
	if(count==200)
	{
		//BEST GUESS 200 MILLI SEC TIMER
		//PORTB=~PORTB;
		
		//CHECK FAULT STATUS OF ALL STATIONS AND PRINT ON LCD
		
		
		count=0;
	}
	TCNT0=177;
}



ISR(INT0_vect)
{
	//change this commented lines as per requirement		
/*
	PORTB=0xff;
	_delay_ms(500);
	PORTB=0x00;
	_delay_ms(500);	
*/

	char k;
	for(k=0;k<10;k++)
	{
		if(station[k].alarm_status==1 && station[k].ot_flag==1)
		{
			//alarm mute
			station[k].alarm_status=0;
			
			//PRINT ON LCD STATION NUMBER AND ALARM MUTE
			break;    //STOP ONLY ONE ALARM STATUS ON PRESSING MUTE SWITCH ONCE
		}
	}



	GIFR=0x00;
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////UART

void send_char(volatile unsigned char c)
{
	
	UCSRA=0x40;
	UDR=c;
	while((UCSRA & 0x40)==0);
}



void prints(char *ptr)
{
	
	while(*ptr)
	{
		
		send_char(*ptr);
		ptr++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////LCD
void lcd_command(char cmd)
{
	
	PORTC&=(~0x80);
	
	PORTC=cmd;
	PORTC|=0x40;
//	_delay_ms(5);                 //IF REQUIRED ADD DELAYS FOR ENABLE SIGNAL 
	PORTC&=(~0x40);
	
	PORTC=cmd<<4;
	PORTC|=0x40;
//	_delay_ms(5);
	PORTC&=(~0x40);
	_delay_ms(5);	
}


void lcd_data(char data)
{	
//	PORTB|=0x01;
//	PORTB&=(~0x02);
	
	PORTC|=0x80;
	
	PORTC=data;
	PORTC|=0x40;
//	_delay_ms(5);                           //IF REQUIRED ADD DELAYS FOR ENABLE SIGNAL
	PORTC&=(~0x40);
	
	PORTC=data<<4;
	PORTC|=0x40;
//	_delay_ms(5);
	PORTC&=(~0x40);
	_delay_ms(5);
}

void lcd_print(char *ptr)
{
	while(*ptr!=0)
	{
		
		lcd_data(*ptr);
		ptr++;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uart_init()
{
		UBRRH=0;
		UBRRL=103;
		
		UCSRA=0x00;
		UCSRB=0x98;
		UCSRC=0x86;
//		sei();
		prints("UART READY\n");
}


void lcd_init()
{
	_delay_ms(5);
	PORTC |= 0x30;
	_delay_ms(5);           //CHECK IF THESE LINES ARE REQUIRED BY CONNECTING LCD
	PORTC |= 0x30;
	_delay_ms(5);
	
	lcd_command(0x30);
	_delay_ms(5);
	lcd_command(0x28);
	_delay_ms(5);

	lcd_command(0x01);
	lcd_command(0x06);
	lcd_command(0X0c);
	lcd_command(0x80);
	
	lcd_print("LCD OK");
	
	
}


void timer_init()
{
	TCCR0=0x05;
	TCNT0=177;
	TIMSK=0x01;	
}


void interrupt_init()
{
	GICR|=0x40;
}


void port_init()
{
	
	////////////////////CHANGE ALL PORT DECLARATIONS AND INTITIALIZATIONS AS PER OUR REQUIREMENT
	
	
	//  PORTA
		DDRA=0x3f;
	//SET RELAY PINS TO HIGH OR LOW ...........INITIALISATION REQUIRED
	
	//PORTB
		DDRB=0xff;
	//	PORTB=0x00;                //CHECK HT12D AND SET PORTB HIGH OR LOW
		
	//PORTC
		PORTC|=0xfc;
	
	//PORTD
		DDRD=0x00;          //HAVE DOUBT WHETHER PORT HAS TO BE DECLARED INPUT WHILE USING INTERRUPT
	
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
		station[j].data_inp=1;
		
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
	}
	
	else
	{
		// DRIVE THE PORT FOR RELAYS FOR THE SOLINOIDS................................
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
		PORTA|=0X20;               //FAULT LED ON
		// DRIVE THE PORT FOR ALARM................................
	}
	
	else
	{
		PORTA &= (~0X20);               //FAULT LED OFF
		// DRIVE THE PORT FOR ALARM................................
	}
	
}

void station_sweep()
{
	char k;
	for(k=0;k<10;k++)
	{	
		//set ht12e address
		PORTB=station[k].station_id;
		
		//check if transmiter and reciever paired
		if((PIND & 0x10) == 0x00)
		{
			station[k].valid_transmission=0;			
		}
		else if((PIND & 0x10) == 0x10)
		{
			station[k].valid_transmission=1;	
		}
		
		//check data from reciever: 0- gas detected 1- normal working
		
		if(((PIND & 0x08) == 0x00) && station[k].ot_flag==0)
		{
			//gas detected
				station[k].fault_status=1;
				station[k].alarm_status=1;
				station[k].ot_flag=1;
				
				//PRINT FAULT STATUS ON LCD			
		}
		else if(((PIND & 0x08) == 0x08) && station[k].ot_flag==0)
		{
			//gas not detected
				station[k].fault_status=0;
				station[k].alarm_status=0;			
		}		
	}
	
}

int main(void)
{
	
//	char dispbuf[5];     //TO BE USED WHILE USING DTOA FUNCTION           //functions:    lcd_print( ),   dtoa(xx,dispbuf,n)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////WATCH DOG TIMER ENABLE 4 SECONDS
	wdt_enable(WDTO_4S);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	port_init();
	
	interrupt_init();
	timer_init();
	uart_init();
	lcd_init();
	
	sei();	
	
	struct_int();
	

	while(1);
	{
		station_sweep();
		
		alarm_check();
		relay_check();
		
		wdt_reset();
	}
	
	
}