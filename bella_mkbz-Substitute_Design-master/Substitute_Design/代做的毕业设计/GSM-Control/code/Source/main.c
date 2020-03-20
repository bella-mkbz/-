/**********************************************************************************
 * ¹¤³ÌÃû  :
 * ÃèÊö    :Í¨¹ıÊÖ»ú·¢ËÍ¶ÌĞÅ¿ØÖÆLED
 * ÊµÑéÆ½Ì¨:STC12XX
 * ¿â°æ±¾  :
 * ×÷Õß    :ÄàÈËÍ¨ĞÅÄ£¿é¿ª·¢Æ½Ì¨ÍÅ¶Ó 
 * ²©¿Í    :http://nirenelec.blog.163.com
 * ÌÔ±¦    :http://shop105683814.taobao.com
**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"
#include "temp.h"

#define Buf1_Max 60 					  //´®¿Ú1»º´æ³¤¶È
#define Buf2_Max 200 					  //´®¿Ú2»º´æ³¤¶È
/*************	±¾µØ³£Á¿ÉùÃ÷	**************/
sbit Delay_1  			= P2^0;					//¼ÌµçÆ÷1
sbit Delay_2  			= P2^1;					//¼ÌµçÆ÷1

/*************  ±¾µØ±äÁ¿ÉùÃ÷	**************/
xdata u8 Uart1_Buf[Buf1_Max];
xdata u8 Uart2_Buf[Buf2_Max];
unsigned char Password[5] = "0806";
xdata unsigned char phone[14 ] = "\"18539863403\"";
static unsigned char *content = "Temperature: ";
static unsigned char *SMS_Num="\"+8613010761500\"";
unsigned char Datas_Buff[8]= {0, 0, 0, 0, 0, 0, 0,'\0'};
u8 Times=0,First_Int = 0,shijian=0;
//u8 Time_count=0;
bdata u8 Flag;//¶¨Ê±Æ÷±êÖ¾Î»
sbit Timer0_start =Flag^0;	//¶¨Ê±Æ÷0ÑÓÊ±Æô¶¯¼ÆÊıÆ÷
sbit Uart2_Start  =Flag^1;	//´®¿Ú2¿ªÊ¼½ÓÊÕÊı¾İ
sbit Uart2_End		=Flag^2;	//´®¿Ú2½ÓÊÕÊı¾İ½áÊø


/*************	±¾µØº¯ÊıÉùÃ÷	**************/
void GPIO_config(void);
void Timer0Init(void);
void CLR_Buf2(void);
u8 Find(u8 *a);
void Set_Text_Mode(void);
void Check_New_Message(void);
void Wait_CREG(void);
void Sec_AT_Command(u8 *b,u8 *a,u8 wait_time);
void Send_Text_Sms(void);
void Temp_Change(int temp,unsigned char* Buff);
/*************  Íâ²¿º¯ÊıºÍ±äÁ¿ÉùÃ÷*****************/


/*******************************************************************************
* º¯ÊıÃû : main 
* ÃèÊö   : Ö÷º¯Êı
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : ´®¿Ú2¸ºÔğÓëGPRSÄ£¿éÍ¨ĞÅ£¬´®¿Ú1ÓÃÓÚ´®¿Úµ÷ÊÔ£¬¿ÉÒÔ±ÜÃâÔÚÏÂÔØ³ÌĞòÊ±Êı¾İ
					 »¹·¢ËÍµ½Ä£¿é
*******************************************************************************/
void main(void)
{
	GPIO_config();
	Uart1Init();
	Uart2Init();
	Timer0Init();
	EA=1;	//¿ª×ÜÖĞ¶Ï
	UART1_SendString("GPRSÄ£¿é¶ÌĞÅ²âÊÔ³ÌĞò\r\n");
	UART1_SendString("GPRSÄ£¿éÔÚ×¢²áÍøÂç\r\n");
	Wait_CREG();
	UART1_SendString("GPRSÄ£¿é×¢²á³É¹¦\r\n");
	UART1_SendString("GPRSÄ£¿é¶ÌĞÅÄ£Ê½ÉèÖÃ\r\n");
	Set_Text_Mode();
	UART1_SendString("ÉèÖÃ³É¹¦£¬¶ÌĞÅÄ£Ê½£ºTEXT\r\n");
	
	UART1_SendString("³õÊ¼»¯Íê³É\r\n");
	
	
	while(1)
	{
		//Check_New_Message();
		Ds18b20ReadTemp();
	}
	
}

/*******************************************************************************
* º¯ÊıÃû : Uart1 
* ÃèÊö   : ´®¿Ú1ÖĞ¶Ï·şÎñÈë¿Úº¯Êı
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //Çå³ıRIÎ»
    }
    if (TI)
    {
        TI = 0;                 //Çå³ıTIÎ»
    }
}

/*******************************************************************************
* º¯ÊıÃû : Uart2
* ÃèÊö   : ´®¿Ú2ÖĞ¶Ï·şÎñÈë¿Úº¯Êı
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //¹Ø±Õ´®¿Ú2ÖĞ¶Ï
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //Çå³ıS2RIÎ»
			Uart2_Buf[First_Int] = S2BUF;  	  //½«½ÓÊÕµ½µÄ×Ö·û´®´æµ½»º´æÖĞ
			First_Int++;                			//»º´æÖ¸ÕëÏòºóÒÆ¶¯
			if(First_Int > Buf2_Max)       		//Èç¹û»º´æÂú,½«»º´æÖ¸ÕëÖ¸Ïò»º´æµÄÊ×µØÖ·
			{
				First_Int = 0;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //Çå³ıS2TIÎ»
    }
		IE2  |= 0x01;   //Ê¹ÄÜ´®¿Ú2ÖĞ¶Ï
}
/*******************************************************************************
* º¯ÊıÃû : Timer0_ISR
* ÃèÊö   : ¶¨Ê±Æ÷0ÖĞ¶Ï·şÎñÈë¿Úº¯Êı,20msÖĞ¶ÏÒ»´Î
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	TR0=0;//¹Ø¶¨Ê±Æ÷
	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	TR0=1;//¿ª¶¨Ê±Æ÷
}
/*******************************************************************************
* º¯ÊıÃû : GPIO_config
* ÃèÊö   : IO¿ÚÅäÖÃº¯Êı
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void	GPIO_config(void)
{
		P3M1 &= 0XC3;  //ÅäÖÃP32~P35ÎªÍÆÍìÊä³ö
		P3M0 |=	~0XC3;
}
void Timer0Init(void)		//20ºÁÃë@22.1184MHz
{
	AUXR &= 0x7F;	//12TÄ£Ê½
	TMOD &= 0xF0;	//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½ 16Î»ÖØÔØ
	TL0 = 0x00;		//Éè¶¨¶¨Ê±Æ÷³õÖµ
	TH0 = 0x70;		//Éè¶¨¶¨Ê±Æ÷³õÖµ
	TF0 = 0;			//Çå³ıTF0±êÖ¾
	TR0 = 1;			//¶¨Ê±Æ÷0¿ªÊ¼¼ÆÊ±
	ET0 = 1;    	//Ê¹ÄÜ¶¨Ê±Æ÷0ÖĞ¶Ï
}
/*******************************************************************************
* º¯ÊıÃû : CLR_Buf2
* ÃèÊö   : Çå³ı´®¿Ú2»º´æÊı¾İ
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //½«»º´æÄÚÈİÇåÁã
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //½ÓÊÕ×Ö·û´®µÄÆğÊ¼´æ´¢Î»ÖÃ
}

/*******************************************************************************
* º¯ÊıÃû : Find
* ÃèÊö   : ÅĞ¶Ï»º´æÖĞÊÇ·ñº¬ÓĞÖ¸¶¨µÄ×Ö·û´®
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : unsigned char:1 ÕÒµ½Ö¸¶¨×Ö·û£¬0 Î´ÕÒµ½Ö¸¶¨×Ö·û 
* ×¢Òâ   : 
*******************************************************************************/

u8 Find(u8 *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* º¯ÊıÃû : Second_AT_Command
* ÃèÊö   : ·¢ËÍATÖ¸Áîº¯Êı
* ÊäÈë   : ·¢ËÍÊı¾İµÄÖ¸Õë¡¢·¢ËÍµÈ´ıÊ±¼ä(µ¥Î»£ºS)
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/

void Sec_AT_Command(u8 *b,u8 *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;									
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))          
		{
			if(Timer0_start == 0)
			{
				b = c;					
				for (b; *b!='\0';b++)
				{
					UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1; 
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;  
		}
	}
	CLR_Buf2(); 
}

/*******************************************************************************
* º¯ÊıÃû : Set_Text_Mode
* ÃèÊö   : ÉèÖÃ¶ÌĞÅÎªTEXTÎÄ±¾Ä£Ê½
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void Set_Text_Mode(void)
{
//	unsigned char temp[50]="AT+CSCA=";									
	Sec_AT_Command("ATE0","OK",3);	                        //È¡Ïû»ØÏÔ	  					
	Sec_AT_Command("AT+CNMI=3,2,0,0,0","OK",3);	            //ĞÂ¶ÌĞÅÖ±½ÓÊä³ö						
	Sec_AT_Command("AT+CMGF=1","OK",3);	                  	//TEXTÄ£Ê½	
	Sec_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);  //ËùÓĞ²Ù×÷¶¼ÔÚSIM¿¨ÖĞ½øĞĞ	
//	strcat(temp,SMS_Num);
//	Sec_AT_Command(temp,"OK",3);                            //ÉèÖÃ¶ÌĞÅÖĞĞÄºÅÂë
}

/*******************************************************************************
* º¯ÊıÃû : Check_Message_rec
* ÃèÊö   : ¼ì²éÊÇ·ñÓĞĞÂĞÅÏ¢£¬²¢Ö´ĞĞĞÅÏ¢ÄÚÈİÖ¸Áî
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/

void Check_New_Message(void)
{
	u8 temp=0,i;
	if(strstr(Uart2_Buf,"+CMT")!=NULL)   		//Èô»º´æ×Ö·û´®ÖĞº¬ÓĞ"+CMT"¾Í±íÊ¾ÓĞĞÂµÄ¶ÌĞÅ
	{
		delay_ms(50);//µÈ´ıÊı¾İÈ«²¿½ÓÊÕÍê³É
		UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:\r\n");
		UART1_SendString(Uart2_Buf);			//°ÑÄ£¿é·µ»ØµÄĞÅÏ¢Ô­ÑùÊä³ö
		if(strstr(Uart2_Buf,Password)!=NULL)
		{
			if(strstr(Uart2_Buf,"Switch1:on")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Switch1:on\r\n");
        Delay_1 = 0;
			}
			if(strstr(Uart2_Buf,"Switch1:off")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Switch1:off\r\n");
        Delay_1 = 1;
			}
			if(strstr(Uart2_Buf,"Switch2:on")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Switch2:on\r\n");
				Delay_2 = 0;
			}
			if(strstr(Uart2_Buf,"Switch2:off")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Switch2:off\r\n");
				Delay_2 = 1;
			}
			if(strstr(Uart2_Buf,"Check")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Check\r\n");
				UART1_SendString(&Uart2_Buf[12]);
				for(i=0;i<11;i++)
				    phone[1+i] = Uart2_Buf[12+i];
				Send_Text_Sms();
			}
			if(strstr(Uart2_Buf,"Alter")!=NULL)
			{
				UART1_SendString("ÊÕµ½ĞÂ¶ÌĞÅ:Alter\r\n");
        UART1_SendString(&Uart2_Buf[First_Int-6]);
				Password[0] = Uart2_Buf[First_Int-6];
				Password[1] = Uart2_Buf[First_Int-5];
				Password[2] = Uart2_Buf[First_Int-4];
				Password[3] = Uart2_Buf[First_Int-3];
				UART1_SendString(Password);
				UART1_SendString("ĞÂÃÜÂë\r\n");
			}
	  }
		CLR_Buf2();
		Sec_AT_Command("AT+CMGD=1,1","OK",3);//É¾³ı¶ÌĞÅ
	}
}

/*******************************************************************************
* º¯ÊıÃû : Wait_CREG
* ÃèÊö   : µÈ´ıÄ£¿é×¢²á³É¹¦
* ÊäÈë   : 
* Êä³ö   : 
* ·µ»Ø   : 
* ×¢Òâ   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");
		UART2_SendLR();
		delay_ms(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
			if(Uart2_Buf[k] == ':')
			{
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))
				{
					i = 1;
					UART1_SendLR();
				  break;
				}
			}
		}
		UART1_SendString("×¢²áÖĞ.....");
	}
}

void Send_Text_Sms(void)
{
	unsigned char temp[50]="AT+CMGS=";
	Temp_Change(Ds18b20ReadTemp(),Datas_Buff);
	strcat(temp,phone); 
	Sec_AT_Command(temp,">",3); 
	UART2_SendString(content); 
	UART2_SendString(Datas_Buff);	
	UART2_SendData(0X1A); 
  UART2_SendLR();	
}

void Temp_Change(int temp,unsigned char* Buff)
{
	  float tp;  
	  if(temp< 0)				//µ±ÎÂ¶ÈÖµÎª¸ºÊı
  	{
			Buff[0] = '-';
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//ËãÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
  	}
 	else
  	{	
    Buff[0] = '+';			
		tp=temp;//ÒòÎªÊı¾İ´¦ÀíÓĞĞ¡ÊıµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãĞÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕıµÄÄÇÃ´£¬ÄÇÃ´ÕıÊıµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
	  }
	Buff[1] = temp / 10000 + 0x30;
	Buff[2] = temp % 10000 / 1000 + 0x30;
	Buff[3] = temp % 1000 / 100 + 0x30;
	Buff[4] = '.';	
	Buff[5] = temp % 100 / 10 + 0x30;
	Buff[6] = temp % 10 + 0x30;
}

