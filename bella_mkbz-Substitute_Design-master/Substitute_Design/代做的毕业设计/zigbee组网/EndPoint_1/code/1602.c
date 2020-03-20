#include <1602.h>

void DelayUs2x(unsigned char t)
{   
 while(--t);
}

void DelayMs(unsigned char t)               //1mS
{
     
 while(t--)
 {
	 DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
              ????
------------------------------------------------*/
bit LCD_Check_Busy(void) 
{ 
	 DataPort= 0xFF; 
	 RS_CLR; 
	 RW_SET; 
	 EN_CLR; 
	 _nop_(); 
	 EN_SET;
	 return (bit)(DataPort & 0x80);    
}
/*------------------------------------------------
              ??????
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
{  
	 while(LCD_Check_Busy()); //????
	 RS_CLR; 
	 RW_CLR; 
	 EN_SET; 
	 DataPort= com;          //
	 _nop_(); 
	 EN_CLR;
}
/*------------------------------------------------
              ??????
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
{ 
	 while(LCD_Check_Busy()); //????
	 RS_SET; 
	 RW_CLR; 
	 EN_SET; 
	 DataPort= Data; 
	 _nop_();
	 EN_CLR;
}

/*------------------------------------------------
                ????
------------------------------------------------*/
 void LCD_Clear(void) 
{ 
	 LCD_Write_Com(0x01); 
	 DelayMs(5);
}
/*------------------------------------------------
              ???????
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)//y???,x????,??????? 
{     
 if (y == 0)    
     LCD_Write_Com(0x80 + x);        
 else if(y == 1)    
     LCD_Write_Com(0xc0 + x); 
 else if(y == 2)    
     LCD_Write_Com(0x90 + x); 
 else if(y == 3)    
     LCD_Write_Com(0xd0 + x);      
	 while (*s) 
	 {     
		 LCD_Write_Data( *s);     
		 s ++;     
	 }
}
/*------------------------------------------------
              ??????
------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
{     
 if (y == 0)    
     LCD_Write_Com(0x80 + x);        
 else if(y == 1)    
     LCD_Write_Com(0xc0 + x); 
 else if(y == 2)    
     LCD_Write_Com(0x90 + x); 
 else if(y == 3)    
     LCD_Write_Com(0xd0 + x);  
 LCD_Write_Data( Data);  
}


 void LCD_Init(void) 
{
   LCD_Write_Com(0x38);   
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);   
   LCD_Write_Com(0x01);   
   LCD_Write_Com(0x06);  
   DelayMs(5); 
   LCD_Write_Com(0x0C);   
}

void LCD_Write_Num(unsigned char x,unsigned char y,unsigned int Num) 
{     
	 if (y == 0)    
			 LCD_Write_Com(0x80 + x);        
	 else if(y == 1)    
			 LCD_Write_Com(0xc0 + x); 
	 else if(y == 2)    
			 LCD_Write_Com(0x90 + x); 
	 else if(y == 3)    
			 LCD_Write_Com(0xd0 + x); 
	 if((Num/100) != 0)
	 {
		 LCD_Write_Data( Num/100 + 0x30); 
		 LCD_Write_Data( Num%100/10 + 0x30); 
		 LCD_Write_Data( Num%10 + 0x30); 
	 }
	 else if((Num/10) != 0)
	 {
		 LCD_Write_Data( ' '); 
		 LCD_Write_Data( Num/10 + 0x30); 
		 LCD_Write_Data( Num%10 + 0x30);
	 }
	 else 
	 {
		 LCD_Write_Data( ' '); 
		 LCD_Write_Data( ' ');
		 LCD_Write_Data( Num%10 + 0x30); 
	 }
}
