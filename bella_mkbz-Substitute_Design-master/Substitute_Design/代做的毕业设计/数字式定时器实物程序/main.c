#include <reg51.h>
#include "button.h"
#include "delay.h"
#include "lcd.h"
sbit Beep =    P2^0;
sbit Delay_1 = P2^2;
sbit Delay_2 = P2^3;
bit  F_F = 0;
char Flag = 0,Ge = 0,Shi = 0,Timer = 0;
int Key_Value = 0,j;

void main(void)
{   
	  TMOD = 0X01;
	  TH0 = 0X3c;
	  TL0 = 0Xb0;
	  EA = 1;
	  ET0 = 1;
	  TR0 = 0;
    LcdInit();
	  Show_String(0x80,"Time System :");
	  Show_Dec_Number(0X8d,Shi);
		Show_Dec_Number(0X8e,Ge);
	  while(1)
		 {
			 if(F_F)
			 {
				F_F = 0;
				Shi = 0;
				Ge = 0;
				Show_Dec_Number(0X8d,Shi);
		    Show_Dec_Number(0X8e,Ge);
        Delay_2 = 0; 
				for(j = 0;j < 4;j++)
				 {
				 	Show_String(0xc0,"Time Finished!");
					Delay_1 = 0;
					Beep = 0;
          Delay1ms(500);				 
				  Show_String(0xc0,"              ");
					Beep = 1;
					 Delay_1 = 1;
          Delay1ms(500);	
				 }			 
			 }
			 Key_Value = Button_Value();
			 switch (Key_Value)
       {
       	case 1:
					Flag++;
				  if(Flag == 3)
						 Flag = 1;
       		break;
       	case 2:
          if(Flag == 2)
					  {
						  Ge++;
							if(Ge == 10)
								Ge = 0;
						}
						else if(Flag == 1)
						  {
							  Shi++;
                if(Shi == 10)
								   Shi = 0;
							}
										Show_Dec_Number(0X8d,Shi);
			          Show_Dec_Number(0X8e,Ge);
       		break;
       	case 3:
					if(Flag == 2)
					  {
						  Ge--;
							if(Ge < 0)
								Ge = 9;
						}
						else if(Flag == 1)
						  {
							  Shi--;
                if(Shi < 0)
								   Shi = 9;
							}
							 Show_Dec_Number(0X8d,Shi);
			         Show_Dec_Number(0X8e,Ge);
       		break;
       	case 4:
					Show_String(0xcd," ");
	        Show_String(0xce," ");
				  Flag = 0;
				  TR0 = 1;
				  Timer = 10*Shi + Ge;
       		break;
       	default:
       		break;
       } 
			switch (Flag)
      {
      	case 1:
	        Show_String(0xcd,"-");
	        Show_String(0xce," ");
      		break;
      	case 2:
	        Show_String(0xcd," ");
	        Show_String(0xce,"-");			
      		break;
      	default:
      		break;
      }
		 }
}

void Timer0(void) interrupt 1
{
    static int i = 0;
	  TH0 = 0X3c;
	  TL0 = 0Xb0;
	  i++;
	  if(i == 18)
		{
		  i = 0;
      Timer--;
			if(Timer < 10)
			{
			  Show_Dec_Number(0x8d,0);
			  Show_Dec_Number(0x8e,Timer);				
			}
			else
			  Show_Dec_Number(0x8d,Timer);
			if(Timer == 0)
			  {
				  TR0 = 0;
					F_F = 1;
				}
		}
}
