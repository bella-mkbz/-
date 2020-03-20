#include<reg51.h>
#include<intrins.h>
#include "delay.h"
#include "button.h"
#include "lcd.h"
sbit Motor_A  =  P1^5;
sbit Motor_B  =  P1^6;
sbit Motor_EN =  P1^7;
sbit Relay = P2^1;
sbit Led = P2^2;

static char Pulse = 100;
bit Mode_Flag = 0;
bit Start_Stop_Flag = 0;
bit Temp_Flag = 0;
bit Flag;
extern bit Start18B20();
extern bit Get18B20Temp(int *temp);
void GetShowTemp();
void main()
{ 
  unsigned char temp;
	int temparature;
  bit Show_Flag=0;
	Start18B20();
  Motor_A = 1;
  Motor_B = 0;
  Motor_EN = 0;
  TMOD = 0X12;
  TH0 = 0X74;
  TL0 = 0X74;
  TH1 = 0X3c;
  TL1 = 0Xb0;
  EA = 1;
  ET0 = 1;
  ET1 = 1;
	IT0 = 1;                        
  EX0 = 1;                      
  LcdInit();	
  Show_String(0x80,"Mode:Manu");
  while(1)
  {
    GetShowTemp();
    temp = Button_Value();
		switch (temp)
    {	 
    	case 1:Mode_Flag = ~Mode_Flag;
		       Flag = Mode_Flag;
			     break;
    	case 2:Start_Stop_Flag = ~Start_Stop_Flag;		 
    		     break;
        case 3:Pulse -=35;
			   if(Pulse <= 0)
					Pulse = 0;   						 
		         break;
        case 4:Pulse +=35;
			   if(Pulse >= 100)
					Pulse = 100;
		         break;
    	default:
    		     break;
    }
     if((Mode_Flag==0)&&(Start_Stop_Flag==1))
	 {
		 if(Temp_Flag)
		 {
		   Temp_Flag = 0;
			 TR0 = 1;
	     Show_Flag = 1;
		 }


	  }
	 else if((Mode_Flag==0)&&(Start_Stop_Flag==0))
	 {
	   TR0 = 0;
	   Motor_EN = 0;
	   if(Show_Flag)
	   {
		 Pulse = 100;
		 Show_Flag = 0;
		}
	 }    
	if(Flag==1)
	 {
		 Show_String(0x85,"Auto");
		 if(Temp_Flag)
		 {
			 Flag = 0;
			 TR0 = 1;
			 TR1 = 1;
		 }
	 }

	 if(Pulse == 100)
	    Show_String(0xc5,"-----");
	 else if((Pulse == 65) || (Pulse == 70))
	     Show_String(0xc5,"Small");
	 else if((Pulse == 35) || (Pulse == 30))
	     Show_String(0xc5,"Midle");
	 else if(Pulse == 0)
	     Show_String(0xc5,"Large");
  }
}
void exint0() interrupt 0         
{
    Led = 0;
}

void Timer_0(void) interrupt 1
{
  static unsigned char Count = 0;
  Count++;
  if(Count > 100)
     Count = 0;
  if(Count <= Pulse)
	  Motor_EN = 0;
  else
	  Motor_EN = 1;
}
void Timer_1(void) interrupt 3
{
  static unsigned int count = 0; 
  TH1 = 0X3c;
  TL1 = 0Xb0;
  count++;
  if(count == 200)
  {
     count = 0;
	 TR0 = 0;
	 TR1 = 0;
	 Pulse  = 100;
	 Motor_EN = 0;
	 Mode_Flag = 0;
	 Show_String(0x85,"Manu       ");
  }
}
void GetShowTemp()
{
    int temp;
	  static unsigned char i =0;
    unsigned char str[4];
    
    if (Get18B20Temp(&temp))  
    {  
			  if(!i)
				Delay1ms(1000);
			  Get18B20Temp(&temp);
        temp >>= 4;  
        str[0] = temp/10 + '0'; 
        str[1] = temp%10 + '0';
        str[2] = '\0';
        Show_String(0x8c, str);
			  if(temp >= 80)
				{
				  Temp_Flag  = 1;
					Relay = 0;
				}
					
				else 
				{
				  Relay = 1;
				}
				i++;
				if(i>=100)
					i = 1;
    }
    else
    {   
        Show_String(0x8c, "--");
    }
    Start18B20();  
}
