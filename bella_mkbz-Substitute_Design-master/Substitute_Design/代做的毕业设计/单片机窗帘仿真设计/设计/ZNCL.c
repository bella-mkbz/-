#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_dat P0			 		 //1602Êı¾İ¿Ú


sbit LCD1602_rs=P2^5;					 //1602 I/O ¶¨Òå
sbit LCD1602_rw=P2^6;					 //1602 I/O ¶¨Òå
sbit LCD1602_e=P2^7;

sbit SCLK=P3^4;	                        //DS1302Ê±ÖÓ
sbit SDA=P3^5;	                    	//DS1302Êı¾İ
sbit RST=P3^6;	                    	//DS1302Æ¬Ñ¡

sbit key_1=P1^0;						//ÉèÖÃ°´¼ü
sbit key_2=P1^1;						//
sbit key_3=P1^2;						//
sbit key_4=P1^3;						//ÇĞ»»Ä£Ê½°´¼ü
										 
sbit IDR=P1^4;							//¶¨Òå¹âÃôµç×è½Ó¿Ú


uchar zz[]=				   //Õı×ªÊı¾İ
{
	0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09
};


uchar fz[]=				   //·´×ªÊı¾İ
{
	0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01
};




uchar irdate[33];						  //´æ´¢½ÓÊÕµ½µÄºìÍâĞÅºÅ
uchar irbyte[4];						  //´æ´¢¶Ô½ÓÊÕĞÅºÅ´¦ÀíºóµÄÊı¾İ
uchar irtime,irflag,bitnum,irdateok;	  //¶¨Òå½ÓÊÕºìÍâËùĞèÒªµÄ±äÁ¿
uint time_s;
uint DJ_dat;

uint ms;
uchar Time[12];	                            //´æ´¢Ê±¼äÊı¾İ     
uchar state,loop;
bit s1,open,now_state;
bit Serial_Flag;

void delay(uint T)							   //ÑÓÊ±³ÌĞò
{
	while(T--);
}

//¶¨Ê±Ğ´Êı¾İ
void DS1302WriteByte(uchar dat)
{
	uchar i;
	SCLK=0;					 // ³õÊ¼Ê±ÖÓÏßÖÃÎª 0 0 0 0
	delay(20);
	for(i=0;i<8;i++)			 // ¿ªÊ¼´«Êä 8 8 8 8  ¸ö×Ö½ÚµÄÊı¾İ
	{
		SDA=dat&0x01;			 // È¡×îµÍÎ»£¬×¢Òâ  2 DS1302 µÄÊı¾İºÍµØÖ·¶¼ÊÇ´Ó×îµÍÎ»¿ªÊ¼´«ÊäµÄ
		delay(20);
		SCLK=1;					 // Ê±ÖÓÏßÀ­¸ß£¬ÖÆÔìÉÏÉıÑØ£¬ SDA  µÄÊı¾İ±»´«Êä
		delay(20);
		SCLK=0;					 // Ê±ÖÓÏßÀ­µÍ£¬ÎªÏÂÒ»¸öÉÏÉıÑØ×ö×¼±¸
		dat>>=1;				 // Êı¾İÓÒÒÆÒ»Î»£¬×¼±¸´«ÊäÏÂÒ»Î»Êı¾İ
	}
}
   //¶¨Ê±¶ÁÊı¾İ
uchar DS1302ReadByte()
{
	uchar i,dat;
	delay(20);
	for(i=0;i<8;i++)
	{
		dat>>=1;				 // Òª·µ»ØµÄÊı¾İ×óÒÆÒ»Î»
		if(SDA==1)				 // µ±Êı¾İÏßÎª¸ßÊ±£¬Ö¤Ã÷¸ÃÎ»Êı¾İÎª 1 1 1 1
		dat|=0x80;			 // Òª´«ÊäÊı¾İµÄµ±Ç°ÖµÖÃÎª , 1, Èô²»ÊÇ, , , , ÔòÎª 0 0 0 0
		SCLK=1;                 //À­¸ßÊ±ÖÓÏß
		delay(20);
		SCLK=0;					 // ÖÆÔìÏÂ½µÑØ
		delay(20);
	}
	return dat;				 // ·µ»Ø¶ÁÈ¡³öµÄÊı¾İ
}


uchar DS1302Read(uchar cmd)
{
	uchar dat;
	RST=0; 					// ³õÊ¼  CE  ÏßÖÃÎª 0 0 0 0
	SCLK=0;					 // ³õÊ¼Ê±ÖÓÏßÖÃÎª 0 0 0 0
	RST=1; 					// ³õÊ¼  CE  ÖÃÎª 1 1 1 1 £¬´«Êä¿ªÊ¼
	DS1302WriteByte(cmd);		 // ´«ÊäÃüÁî×Ö£¬Òª¶ÁÈ¡µÄÊ±¼ä
	dat=DS1302ReadByte(); 		// ¶ÁÈ¡ÒªµÃµ½µÄÊ±¼ä
	SCLK=1;					 // Ê±ÖÓÏßÀ­¸ß
	RST=0;					 // ¶ÁÈ¡½áÊø£¬ CE ÖÃÎª 0 0 0 0 £¬½áÊøÊı¾İµÄ´«Êä
	return dat; 				// ·µ»ØµÃµ½µÄÊ±¼ä
}

void DS1302Write(uchar cmd, uchar dat)
{
	RST=0; 					 // ³õÊ¼  CE  ÏßÖÃÎª 0 0 0 0
	SCLK=0; 				 // ³õÊ¼Ê±ÖÓÏßÖÃÎª 0 0 0 0
	RST=1; 						 // ³õÊ¼  CE  ÖÃÎª 1 1 1 1 £¬´«Êä¿ªÊ¼
	DS1302WriteByte(cmd); 
	dat=(dat/10)*16+dat%10;
	DS1302WriteByte(dat);  		// Ğ´ÈëÒªĞŞ¸ÄµÄÊ±¼ä
	SCLK=1; 				 // Ê±ÖÓÏßÀ­¸ß
	RST=0; 					 // ¶ÁÈ¡½áÊø£¬ CE  ÖÃÎª 0 0 0 0 £¬½áÊøÊı¾İµÄ´«Êä
}


void Init_DS1302(void)
{
	if (DS1302Read(0xc1)!=3)
	{
		DS1302Write(0x8e,0x00);			// Ğ´±£»¤¹Ø
		DS1302Write(0xc0,3);
		DS1302Write(0x80,0); //Ãë			 // ³õÊ¼ÃëÖµÎª 0 
		DS1302Write(0x82,18);//·Ö			 // ³õÊ¼·ÖÖÓÖµÎª  0
		DS1302Write(0x84,6);//Ê± 		 // ³õÊ¼Îª 24 Ê½ Ğ¡Ê±Ä£Ê½  ³õÊ¼Ê±¼äÎª 0µã
		DS1302Write(0x88,3);
		DS1302Write(0x8c,15);
		DS1302Write(0x8a,3);
		DS1302Write(0x90,0x01); 		 // ³äµç
		DS1302Write(0x8e,0x80);
	}else
	{ 	
		DS1302Write(0x8e,0x00);			// Ğ´±£»¤¹Ø
		DS1302Write(0x90,0x01); 		 // ³äµç
		DS1302Write(0x8e,0x80);	
	}
}

void read_time1()						 //ÊµÊ±¶ÁÈ¡DS1302ÖĞµÄÊ±¼äÊı¾İ
{
	uchar i,j;
	for (j=0;j<7;j++)					//ÒÀ´Î¶Á³öÃë.·Ö.Ğ¡Ê± ´æµ½TimeÊı×é
	{
		i=DS1302Read(0x81+j*2);
		i=(i/16)*10+i%16;
		Time[j]=i;
	}
	
	for (j=0;j<5;j++)
	{
		i=DS1302Read(0xc3+j*2);			 //¶Á´æµ½Ds1302¾²Ì¬RAMµÄÊı¾İ
		i=(i/16)*10+i%16;
		Time[7+j]=i;
	}
	if(Time[7]>23||Time[9]>23||Time[8]>59||Time[10]>59)	 //Èô¶Á³öµÄÊı¾İ²»ºÏ³£Àí£¬³õÊ¼¸³Öµ
	{
		Time[7]=6;	                     //´æ´¢µÄÊÇ¶¨Ê±Ê±¼äÏÂÏŞĞ¡Ê±
		Time[8]=30;						 //´æ´¢µÄÊÇ¶¨Ê±Ê±¼äÏÂÏŞ·Ö
		Time[9]=8;						 //´æ´¢µÄÊÇ¶¨Ê±Ê±¼äÉÏÏŞĞ¡Ê±
		Time[10]=0;						 //´æ´¢µÄÊÇ¶¨Ê±Ê±¼äÉÏÏŞ·Ö
	}
	if(Time[11]>2)Time[11]=0;			 //´æ´¢µÄÊÇ´¦ÓÚÄÇÖÖÄ£Ê½£¨0ÊÖ¶¯Ä£Ê½   1×Ô¶¯Ä£Ê½      2¶¨Ê±Ä£Ê½£©
}

void read_time()						 //¶ÁÈ¡DS1302 µÄµ±Ç°Ê±¼ä
{
	uchar i;

	i=DS1302Read(0x81+0*2);
	i=(i/16)*10+i%16;
	Time[0]=i;
	
/*	if(Time[0]==0)
	{
		i=DS1302Read(0x81+1*2);			//0x83¶Á³öµÄÊÇ·Ö
		i=(i/16)*10+i%16;				//¶Á³öµÄÊÇÊ®Áù½øÖÆBCD£¬×ª»»ÎªÊ®½øÖÆ
		Time[1]=i;						//´æ´¢
		if(Time[1]==0)
		{
			i=DS1302Read(0x81+2*2);
			i=(i/16)*10+i%16;
			Time[2]=i;
			if(Time[2]==0)
			{
				i=DS1302Read(0x81+3*2);
				i=(i/16)*10+i%16;
				Time[3]=i;
				i=DS1302Read(0x81+5*2);
				i=(i/16)*10+i%16;
				Time[5]=i;
				if(Time[3]==1)
				{
					i=DS1302Read(0x81+4*2);
					i=(i/16)*10+i%16;
					Time[4]=i;
					if(Time[4]==1)
					{
						i=DS1302Read(0x81+6*2);
						i=(i/16)*10+i%16;
						Time[6]=i;
					}
				}	
			}	
		}	
	}	*/	
}
   
void write_DS1302(void)						//  ½«ÉèÖÃºÃµÄÊ±¼ä¡¢·¢ËÍµ½DS1302   ÄÚ
{										
	DS1302Write(0x8e,0x00);					//È¥±£»¤
	DS1302Write(0xc4,Time[8]); 		
	DS1302Write(0xc6,Time[9]);
	DS1302Write(0xc8,Time[10]);
	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}

void write_SYS_mode(void)						//  ½«ÉèÖÃºÃµÄÊ±¼ä¡¢ ·¢ËÍµ½DS1302   ÄÚ
{										
	DS1302Write(0x8e,0x00);
	DS1302Write(0xcA,Time[11]);

	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}



void LCD1602_write(uchar order,dat)				  //1602 Ò»¸ö×Ö½Ú  ´¦Àí
{
    LCD1602_e=0;
    LCD1602_rs=order;
    LCD1602_dat=dat;
    LCD1602_rw=0;
    LCD1602_e=1;
    delay(1);
    LCD1602_e=0;																								     
}

void LCD1602_writebyte(uchar *prointer)				   //1602 ×Ö·û´®    ´¦Àí
{
    while(*prointer!='\0')
    {
        LCD1602_write(1,*prointer);
        prointer++;
    }
}

void LCD1602_cls()									 //1602 ³õÊ¼»¯
{
	LCD1602_write(0,0x01);     //1602 ÇåÆÁ Ö¸Áî
	delay(1500);
	LCD1602_write(0,0x38);     // ¹¦ÄÜÉèÖÃ 8Î»¡¢5*7µãÕó
	delay(1500);
	LCD1602_write(0,0x0c);     //ÉèÖÃ ¹â±ê   ²»ÏÔÊ¾¿ª¹Ø¡¢²»ÏÔÊ¾¹â±ê¡¢×Ö·û²»ÉÁË¸
	LCD1602_write(0,0x06);
	LCD1602_write(0,0xd0);
	delay(1500);
}

void show()
{
	LCD1602_write(0,0x80);					 //ÔÚ1602µÚÒ»ĞĞÏÔÊ¾
	if(state==1&&s1==1)						 //ÉÁË¸´¦Àí
	{
		LCD1602_writebyte("    ");
	}else

	LCD1602_writebyte(" ");
	if(state==4&&s1==1)						//ÏÔÊ¾Ğ¡Ê±
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[2]/10%10);
		LCD1602_write(1,0x30+Time[2]%10);
	}
	if(state==0&&s1==1)	                     //Ã°ºÅ×öĞ¡Êı´¦Àí£¬Ã¿Ò»ÃëÉÁË¸Ò»´Î
	{
		LCD1602_writebyte(" ");
	}else
	{
		LCD1602_writebyte(":");
	}
	if(state==5&&s1==1)						//ÏÔÊ¾·Ö
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[1]/10%10);
		LCD1602_write(1,0x30+Time[1]%10);
	}
  

	LCD1602_write(0,0xc0);					  //1602µÚ¶şĞĞ£¬c0=80+40
	if(Time[11]==2&&state>5)
	{
		LCD1602_writebyte("O:");			  //¶¨Ê±Ä£Ê½ÏÂµÄ£¬¶¨Ê±Ê±¼äÏÂÏŞ
		if(state==6&&s1==1)					  //ÉÁË¸´¦Àí
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[7]/10%10);//ÏÔÊ¾¶¨Ê±Ê±¼äÏÂÏŞ µÄĞ¡Ê±
			LCD1602_write(1,0x30+Time[7]%10);
		}
		LCD1602_writebyte(":");
		if(state==7&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[8]/10%10); //ÏÔÊ¾¶¨Ê±Ê±¼äÏÂÏŞ µÄ·ÖÖÓ
			LCD1602_write(1,0x30+Time[8]%10);
		}

		LCD1602_writebyte("  C:");				 //ÏÔÊ¾Ê±¼äÉÏÏŞ
		if(state==8&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[9]/10%10);  //Ğ¡Ê±
			LCD1602_write(1,0x30+Time[9]%10);
		}
		LCD1602_writebyte(":");
		if(state==9&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[10]/10%10);  //·ÖÖÓ
			LCD1602_write(1,0x30+Time[10]%10);
		}
	}else
	{
		switch(Time[11])
		{
			case 0:
				LCD1602_writebyte(" Manual");		//ÊÖ¶¯
			break;
	
			case 1:
				LCD1602_writebyte(" Auto  ");	  //×Ô¶¯
			break;
	
			case 2:
				LCD1602_writebyte(" Timing");		//¶¨Ê±
			break;
		} 											//×´Ì¬
		LCD1602_writebyte("  WIN:");
		if(open==0)
		{
			LCD1602_writebyte("OFF");
		}else
		{
			LCD1602_writebyte(" ON");	
		}
	}	
}


void qing()			                          //Çå¿Õ½ÓÊÕµ½µÄºìÍâÊı¾İ
{
	uint i;
	for(i=0;i<33;i++)
	{
		irdate[i]=0;
	}
}


void deal_with()							//ºìÍâÊı¾İ´¦Àí£¬ºìÍâÊı¾İÊÇÊ®Áù½øÖÆ
{
	uchar i,j,k,temp;
	k=1;									//K=1,ÒòÎª´æÔÚirdateÊı×éÀï½ÓÊÕµ½µÄµÚÒ»Î»ÊÇ Òıµ¼Âë£¬²»ĞèÒª
	for(j=0;j<4;j++)						//Ò»Ö¡ºìÍâÊı¾İ°üÀ¨£¬Ò»Î»Òıµ¼Âë£¬ËÄÎ»ÓÃ»§Âë
	{
		for(i=0;i<8;i++)					//Ñ­»·8´Î£¬Ìá³öÀ´Ò»Î»Êı¾İ
		{	
			temp=temp>>1;
			if(irdate[k]>7)					 //Í¨¹ı¼¼ÊõÊÖ²á£¬¿ÉÖª£¬½ÓÊÜµ½µÄÊı¾İÔÚ4×óÓÒ£¬ÊÇ0£¬    8×óÓÒÊÇ1£¬È¡ÖĞ¼äÖµ£¬´óÓÚÆß£¬¼´Îª1
			{
				temp=temp|0x80;	
			}
			k++;
		}
		irbyte[j]=temp;						  //¶Á³öÀ´µÄÊı¾İ´æ´¢  £¬Êı¾İµÄ¸ñÊ½ÊÇ16½øÖÆ
	}
}

void key()									  //°´¼ü¼ì²â
{
	if(!key_4||irbyte[2]==0x08)				  //Èç¹û°´¼üKEY4ÓĞ°´ÏÂ£¬»òÕßºìÍâ½ÓÊÕµÄĞÅºÅ0x08,ÄÇÃ´¾ÍÖ´ĞĞÕâ¸öÓï¾ä
	{
		delay(1000);
		if(!key_4||irbyte[2]==0x08)
		{
			while(!key_4||irbyte[2]==0x08)
			{
				qing();						   //Çå¿Õ´æ´¢ºìÍâµÄÊı×é
				deal_with();				   //ºìÍâÊı¾İ´¦Àí£¬ºìÍâÊı¾İÊÇÊ®Áù½øÖÆ
			}
			Time[11]=(Time[11]+1)%3;		   //Ä£Ê½±êÖ¾Î»¼ÓÒ»  
			state=0;						   //Çå¿ÕÑ¡Ôñ±êÖ¾Î»
			write_SYS_mode();				   //½«ÉèÖÃºÃµÄÊ±¼ä·¢ËÍµ½DS1302   ÄÚ
		}
	}

	if(!key_1||irbyte[2]==0x0c)					//ÉèÖÃ°´æI°´ÏÂ£¬»òÕß¼tÍâ½ÓÊÕµ½µÄÊı¾İÊÇ0x0c¾ÍÖ´ĞĞÕâÌõÓï¾ä
	{											
		delay(1000);
		if(!key_1||irbyte[2]==0x0c)
		{
			while(!key_1||irbyte[2]==0x0c)
			{
				qing();
				deal_with();
			}
			if(Time[11]==2)
			{
				state=(state+1)%10;	
			}else
			{
				state=(state+1)%6;
			}

			if(state==0)
			{
				write_DS1302();
			}
		}
	}
	
	if(!key_2||irbyte[2]==0x18)				   
	{
		delay(1000);
		if(!key_2||irbyte[2]==0x18)
		{
			while(!key_2||irbyte[2]==0x18)
			{
				if(irdateok==1)
				{
					irdateok=0;
					deal_with();
				}	
			}
			switch(state)					//key1¡¢key2ÔÚÄ£Ê½0£¨ÊÖ¶¯£©ÏÂ£¬µ±state=0´ò¿ª´°Á±
			{
				case 0:
					if(Time[11]==0)				//key1/key2ÔÚÄ£Ê½0£¨ÊÖ¶¯£©ÏÂ£¬µ±state=0´ò¿ª»ò¹Ø±Õ
					{
						now_state=1;
					}	
				break;

		
				case 4:
					Time[2]=(Time[2]+1)%24;					 //state=4Ê±£¬¶ÔĞ¡Ê±½øĞĞ¼ÓÒ»´¦Àí
				break;
																					    
				case 5:
					Time[1]=(Time[1]+1)%60;				    //state=5Ê±£¬¶Ô·ÖÖÓ½øĞĞ¼ÓÒ»´¦Àí
				break;
					 	case 6:	
					Time[7]=(Time[7]+1)%24;					//state=6Ê±£¬¶Ô¶¨Ê±Ä£Ê½ÏÂµÄÏÂÏŞĞ¡Ê±½øĞĞ¼ÓÒ»´¦Àí
				break;

				case 7:
					Time[8]=(Time[8]+1)%60;					//state=7Ê±£¬¶Ô¶¨Ê±Ä£Ê½ÏÂµÄÏÂÏŞ·ÖÖÓ½øĞĞ´¦Àí
				break;

				case 8:
					Time[9]=(Time[9]+1)%24;				   //state=8Ê±£¬¶Ô¶¨Ê±Ä£Ê½ÏÂµÄÉÏÏŞĞ¡Ê±½øĞĞ¼ÓÒ»´¦Àí
				break;

				case 9:
					Time[10]=(Time[10]+1)%60;			   //state=9Ê±£¬¶Ô¶¨Ê±Ä£Ê½ÏÂµÄÉÏÏŞ·ÖÖÓ½øĞĞ¼ÓÒ»´¦Àí
				break;

		
			}
		}
	}
	if(!key_3||irbyte[2]==0x5e)								//¼õ´¦Àí£¬½éÉÜÍ¬ÉÏ
	{
		delay(1000);
		if(!key_3||irbyte[2]==0x5e)
		{
			while(!key_3||irbyte[2]==0x5e)
			{
				if(irdateok==1)
				{
					irdateok=0;
					deal_with();
				}
			}
			switch(state)
			{
				case 0:
					if(Time[11]==0)
					{
						now_state=0;
					}
				break;

				
				case 4:
					if(Time[2]==0) Time[2]=23;
					else Time[2]--;
				break;

				case 5:
					if(Time[1]==0) Time[1]=59;
					else Time[1]--;
				break;
    	

				case 6:
					if(Time[7]==0) Time[7]=23;
					else Time[7]--;
				break;

				case 7:
				
					if(Time[8]==0) Time[8]=59;
					else Time[8]--;
					
				break;

				case 8:
					if(Time[9]==0) Time[9]=23;
					else Time[9]--;
				break;

				case 9:
					if(Time[10]==0) Time[10]=59;
					else Time[10]--;
				break;

			}
		}
	}
		
}

void proc()
{
	uint xiax,shiji,shangx;				   //¶¨ÒåÈı¸ö±äÁ¿£¬xiaxÊÇ¶¨Ê±Ê±¼äÏÂÏŞ	  shangx¶¨Ê±Ê±¼äÉÏÏŞ  shijiÊµ¼ÊÊ±¼ä
	if(now_state==1)			   //ÉèÖÃµç»úµÄĞı×ª
	{
		if(DJ_dat<520)
		{
			P2=P2&0xf0|zz[loop];
			loop=(loop+1)%8;
			delay(150);
			if(loop==0) DJ_dat++;
		
		}else
		{
			P2=P2&0xf0;
			open=1;
		}
	}else
	{
		if(DJ_dat>0)
		{
			P2=P2&0xf0|fz[loop];
			loop=(loop+1)%8;
			delay(150);
			if(loop==0) DJ_dat--;
		
		}else
		{
			P2=P2&0xf0;
			open=0;
		}
	}
	if(Time[11]==1)					//Ä£Ê½1£¨×Ô¶¯Ä£Ê½£©£¬µç»ú×ª¶¯ÉèÖÃ
	{							  
		if(IDR==0)
		{	
			nOw_state?1;			  		}e,se		{
			now_ótite=0;	
I	|		
 }
		if(Tmme[91]<=2)	)			 ?/Ä£Êµ2¡¨¶ˆÊ±Ä¢Ê½£©£¬µä»ø×ª´¯ÉêÖÃ	ÕÛºÏ³É·ÔÖÓ½ø’ĞñÈ½Ï
	{
		iax=TiM`[7]*60+Time[8];		 //–¨Ê±Ê±¼ôÏÂÏŞ
		sHcogx=Tile[9]*71+Timl[1°];		 /+ö¨Ê±êµ¼äÉÏïŞ	‰shiji=Time0]"&0+Tkme[1Y;		 ­/Ê5¼ËÊ±¼ä
		if¨péax>3la.gX)				I //Èç1û¶¨Ê±µDËÃÍŞ´òÓúÉOÏŞ£¬ÒòÎŠÊÇ05Ğ¡ÊñÖÆ ËµÃ÷ÊÇÍí¸äµÄ¶(Ê±
		{
		if(shiji>=xiax||shijis(`nfx)		 //ÊµüÊŠ±¼ä´óÓÚÏÂÏŞ£¬»òÕßĞ¡ÓëÉÏËŞ
	‰;
			nO÷_sdáte½1;=	 /o¹Ø‘Õ´°±		)
	I	}else if(whi*ixiax&fshi*i>=shcnçX)	!o+ÔÚ¨¶¨Ê±Ê±¾äÒÔÍâ£¬´ò¿ª´°Á±
		{
		‰	now_Stat%=0;	
	I	}
		}else iF(shaïe}>xaax					 /ïÏÂÏŞğ¡ÓÒÉÏOŞ£¬0×ìì¶¨Ê±-
I	y
		‰if(shiji¾=xIax6"shiji|shangx)		 //­éÓÚ¶¨Ê±Ê±¼äÖ®¼ä£Œ¹Ø±Õ´°Á±I
		{
			now_sTatE=0;										}else if(3hiji>=shangx||shiji<Xiax+	  //²3ÔÚ¶¨Ê±Ê±”ä£¬Æô¶¯4°Á±
	H	y
				now_state=0;	
			}
		}	
	}I
}
M
void maén(+
{
	tMD=2pq;	`//¶¨Ê±Æ÷¹¤×ô·½Ê½Éè¶¨	
I\H1=0z3c;Ê	TL1=2xb0;M
	D@0=0Hec;‰-/	I	   	ÔH0=0X77;J	EA=1¹  /+ÿ(†Ğ¶Ï
	ET0=5;/'ÔêĞíT0ÖĞ¶ÏJ	TZ0}1;	  //Æğ¶¯T0
	QX0?5;	 /.ÔÊííê²¿×Ğ¶ÏINT_ÖĞöß	,
	ET1½1;	 //ÔÊĞíT1Ö¶Ï*	TR%?1;
	AT2=19	//ÔÊĞé¶¨Ê±Æ÷T0ÖĞ¶Ï
	SSON5 X51;
	@CO=0X00;
	ES =1;	Lnit_DS10:,);
	reAd_t)-E1(+;
//vead_Tkme±();
	LCD5v02]cls(9;
	whIlm1)
	{
)	if(kòdatook==1)M
	{
			i6dateok=0;
		tda,_wit`();
			qing((;
		}M
	‰hey(-»
	I{how();
=)mn(rtate5=0) read_time();
	}}

J
void ind0() interrupt 0				 .'Ïâ²¿ÖĞ²Ï0£ÓÃÓÚ¼ì²âºìÍâĞÁ¸Å
{
tim%_s=0;						
	if(izvlAg==1)
	{	Iid(irtime>36)				  //¼ì²âºìÍÂÒıµ¼Âã
		{
			bitnum=0;
	}
‰	irdate[fitnuoU=irtime?
		IBtyme}0;
		jitnum*;
		if(jitnum=533				   //ºìÍâÔÅºÅ£¼ËãÉÏÒıµ¼ÂéÒ»¹²ÓĞ73Î›-
	I{		bItîum=0;				   //ÇåÁ££¬ÎªÏÂ´ÎÂ¹ÓÃ×ö×¼1¸
		yrdaôeok=1;			`  /?33?Êı¾´æ´âÍê‘£¬ºìÍâ½ÒÊÑÉê±Ï±êÖ¾Î»ÖÃÒ»
	]
	}
Ielse
{
		ipfl!g=1;*		irvile=0;
	}}/J
         `  (0

void init_1,) interrupT 5	) //¶¨Ê±Æ÷1£¬ÓËÓÚÉèÖÃÉÁË¸±ªÖ®Î»šÍ
û
	TH0=0xf6;
	TLx=x3b;	
	ms++;
if(ms%200==0)
	s		s1=!S1;	
	}M
	hf(m36#99)-	{
		ms=0+M

	proc();
}M

voyä Sdrhal(void9 ifTerrwpt ´
{
  stath' char BuFf[0],i;
	if(DI)
	z
	  RI=0;-
	  Buff[i] = RBUF;
		i++;
		if(Buff[	U ½=0'#'!
		k
			
	‰  C5ffKy]"= '\8';
			if(stRstr(#on#*,Cuff) != JU\L)
			{			( TI = 1;
		™	0rintF("Hello$W/rlä"-;*			while(!tI);M
			TI = 0;
‰)‰mJ      i=0;
‰	
		
	}
y
