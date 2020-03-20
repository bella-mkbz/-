#include <STC12C5A60S2.H>
#include "DHT11.h"
#include <button.h>
#include <1602.h>
#include <stdio.h>

static unsigned char temp_limit = 25;
unsigned char Sensor_Data[5]={0x00};
bit Flag;
void UARTSend(char UCHAR);
void UART_PutString(unsigned char *buf);
sbit Beep = P3^7; 
void Timer0_Init()
{	
  AUXR &= 0x7F;	
	TMOD &= 0xF0;		
	TMOD |= 0x01;		
	TL0 = 0x00;		
	TH0 = 0x4C;		
	TF0 = 0;		
	TR0 = 1;
  EA = 1;
  ET0 = 1;
  TR0 = 1;	
}

void main()
{
	 Timer0_Init();
	 InitUART();
   LCD_Init(); 
   LCD_Clear();                  
   DelayMs(255);

	 for(;;)
	 {
		 switch (Button_Value())
     {
     	case 1:
				temp_limit++;
     		break;
     	case 2:
				temp_limit--;
     		break;
     	default:
     		break;
     }
		 if(temp_limit <= Sensor_Data[2])
			 Beep = 0;
		 else
			 Beep = 1;
		 if(Flag)
		 {
		 			 LCD_Write_Char(14,0,'0' + temp_limit/10);
	     LCD_Write_Char(15,0,'0' + temp_limit%10);
		 }

	 }
}
void Timer0() interrupt 1
{
  static unsigned int Count;
	TL0 = 0x00;		
	TH0 = 0x4C;
  if(++Count == 30)
  { 
	   if(Read_Sensor(Sensor_Data))
		 {
		 	 LCD_Write_String(0,0,"Humidity: ");
	     LCD_Write_String(0,1,"Temparature: ");				 
			 LCD_Write_Char(10,0,'0' + Sensor_Data[0]/10);
	     LCD_Write_Char(11,0,'0' + Sensor_Data[0]%10); 
		 	 LCD_Write_Char(13,1,'0' + Sensor_Data[2]/10);
	     LCD_Write_Char(14,1,'0' + Sensor_Data[2]%10);
			 LCD_Write_Char(15,1,' ');
       UART_PutString("Temp: ");
			 UARTSend(Sensor_Data[2]/10 + 0x30);
			 UARTSend(Sensor_Data[2]%10 + 0x30);
			 UART_PutString(", RH: ");
			 UARTSend(Sensor_Data[0]/10 + 0x30);
			 UARTSend(Sensor_Data[0]%10 + 0x30);
			 UARTSend(0x0D);
			 UARTSend(0x0A);
       Flag = 1;
		 }
		 else
		 {
       UART_PutString("Sensor Not Connected");
			 UARTSend(0x0D);
			 UARTSend(0x0A);
			 LCD_Write_String(0,0,"                ");	
			 LCD_Write_String(0,1,"                ");	
			 Flag = 0;
		 }
		  Count = 0;
	}		

}
/********************************************\
|* ??: ?????? 			            *|
\********************************************/
void UARTSend(char UCHAR)
  {
    SBUF=UCHAR;
    while(TI==0);
    TI=0;
  }

 /********************************************\
|* ??: ??????? 			        *|
\********************************************/
void UART_PutString(unsigned char *buf)
  {
	while(*buf)
      UARTSend(*buf++);
  }
