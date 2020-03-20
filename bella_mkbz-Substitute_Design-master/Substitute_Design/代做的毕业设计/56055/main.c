#include <reg51.h>
#include "button.h"
#include "intrins.h"
#include <stdio.h>
#include "delay.h"
const char* String = "Init Suceses! ";
void SerialInit(void);
void SendString(const char* string);

sbit L_U = P2^0;  //×óÇ°µÆ
sbit L_D = P2^1;  //×óºóµÆ
sbit R_U = P2^2;  //ÓÒÇ°µÆ
sbit R_D = P2^3;  //ÓÒºóµÆ

void main()
{   
	  int Key_Value = 0,i = 0;
    SerialInit();
    SendString(String);
	  Delay1ms(400);
	  String = "Welcome to Lighting Control System !";
	  SendString(String);
    while(1)
		{
		 Key_Value = Button_Value();
     switch (Key_Value)
          {
          	case 1:
						  String = "Turn Left !!!";
	            SendString(String);
							for(i = 0;i < 4;i++)
									{
										L_U = 0;
										L_D = 0;
										Delay1ms(300);
										L_U = 1;
										L_D = 1;
										Delay1ms(300);
									}
          		break;
          	case 2:
						  String = "Turn Right !!!";
	            SendString(String);
							for(i = 0;i < 4;i++)
									{
										R_U = 0;
										R_D = 0;
										Delay1ms(300);
										R_U = 1;
										R_D = 1;
										Delay1ms(300);
									}
          		break;
          	case 3:
						  String = "Break! Break! Break!";
	            SendString(String);
							for(i = 0;i < 4;i++)
									{
										L_U = 0;
										L_D = 0;
										R_U = 0;
										R_D = 0;
										Delay1ms(300);
										L_U = 1;
										L_D = 1;
										R_U = 1;
										R_D = 1;
										Delay1ms(300);
									}
          		break;
					  case 4:
						  String = "Reverse! Reverse! Reverse!";
	            SendString(String);
						  L_D = 0;
						  R_D = 0;
							Delay1ms(5000);
							L_D = 1;
						  R_D = 1;
          		break;
          	default:
          		break;
          }			
		}
}
void SendString(const char* string)
{
    TI = 1;
	  printf("%s\n",string);
	  while(!TI);
	  TI = 0;
}
void SerialInit(void)
{
    SCON = 0x50;
    PCON = 0X00;	
    TMOD = 0x20;            
    TH1 = TL1 = 0xFD; 
    TR1 = 1;              
    ES = 1;              
    EA = 1;                 
}

void Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;             
    }
}

