#include <STC12C5A60S2.H>
#include <DHT22.h>
#include <button.h>

extern 	temp_limit;
bit Flag;

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
		 Flag = 1;
		 Count = 0;
	 	 Read_Sensor();
		 UARTSend_Nbyte();
		 Beep_Alarm();
		 Show();
	}		

}