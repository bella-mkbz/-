#include"lcd1602.h"

//LCD上行显示函数
   void lcd_display_L(unsigned char *a)
   {
      unsigned char i=0;
	  writecom(0x80);
	  for(i=0;a[i];i++)
	  writedat(a[i]);
   }

	void lcd_display_Set(unsigned char *a)
   {
      unsigned char i=0;
	  writecom(0x8d);
	  for(i=0;a[i];i++)
	  writedat(a[i]);
   }

  //LCD下行显示函数
  void lcd_display_H(unsigned char *b)
  {
      unsigned char i=0;
	  writecom(0x80+0x40);
	  for(i=0;b[i];i++)
	  writedat(b[i]);
  }

    //LCD延时函数
   void delay10ms(unsigned char i)
   {
	  for(i=0;i<100;i++);
   }

   //LCD写命令
   void writecom(unsigned char a)
	  {
	    rs=0;
		rw=0;
		P0=a;
		delay10ms(100);
		en=1;
		delay10ms(100);
		en=0;
	  }

   //LCD写数据
	void writedat(unsigned char a)
	{
	    rs=1;
		rw=0;
		P0=a;
		delay10ms(1);
		en=1;
		delay10ms(1);
		en=0;
	}

   //LCD初始化
	void lcdinit()
	{
	  writecom(0x38);
      writecom(0x0c);
      writecom(0x06);
	  writecom(0x01); 
	}
