#include "reg52.h"
#include "intrins.h"
#include "iic.h"
#include "stdio.h"
#include "lcd1602.h"
#include "button.h"

#define uchar unsigned char

sbit in  = P1^0;
sbit out = P1^1;

bit in_last = 1;
bit out_last= 1;
bit Flag = 0;

sbit CS  = P1^2; 
sbit CLK = P1^3;
sbit DIO = P1^4;


unsigned char lcd[20] = {0};
unsigned char dac_out = 0;
unsigned char speed_time = 0;


int set_speed = 30; 
unsigned char current = 0;
unsigned char light_get = 0;
unsigned char current_last = 0;
unsigned char current_geding = 0;
unsigned char Key_Value = 0;

int speed_kp = 30;
int speed_ki = 8;

int speed_error;
int speed_errorlast;
int speed = 0;

int current_kp = 200;
int current_ki = 30; 

int current_error;
int current_errorlast;

unsigned char Get_ADC(bit ch);
void timer_init();
unsigned char speed_pi();
unsigned char current_pi(int set_current);

void main()
{
  ad_send(0x40,255);
  timer_init();
  lcdinit();
	sprintf(lcd,"speed:%03d",speed);
	lcd_display_L(lcd);
	sprintf(lcd,"current:%03d",0);
	lcd_display_H(lcd);
  while(1)
  {
		Key_Value = Button_Value();
		switch (Key_Value)
    {
    	case 1:
				Flag = 1;
    		break;
    	case 2:
				set_speed = set_speed + 4;
    		break;
    	case 3:
				set_speed = set_speed - 4;
    		break;
    	case 4:
				Flag = 0;
    		break;
    	default:
    		break;
    }
		if(Flag)
		{
			EA = 1;
			light_get = Get_ADC(0); 
			sprintf(lcd,"speed:%03d",speed);
			lcd_display_L(lcd);
			sprintf(lcd,"current:%03d",(int)current);
			lcd_display_H(lcd);		
		}
		else
		{
			EA = 0;
			light_get = Get_ADC(0);
			ad_send(0x40,255);
			speed = 0;
			sprintf(lcd,"speed:%03d",speed);
			lcd_display_L(lcd);
			current = 0;
			sprintf(lcd,"current:%03d",current);
			lcd_display_H(lcd);				
		}
		sprintf(lcd,"set:%02d",set_speed);
		lcd_display_Set(lcd);
  }
}

void timer0_irq() interrupt 1
{
	TH0 = (65535-25000)>>8;
	TL0 = 65535-25000;

	if(++speed_time==2)
	{
		speed_time=0;
		speed=TH1;
		speed<<=8;
		speed=TL1;
		TH1=0;
		TL1=0;		 
		current_geding=speed_pi();
	}

	current = Get_ADC(0)/2+current_last/2;	
	current_last = current;

	dac_out=current_pi(current_geding);
	ad_send(0x40,255-dac_out);
}

unsigned char current_pi(int set_current)	
{
  static unsigned long pwm=0; 

	unsigned char pwm_u8=0;
	
	current_error=set_current -(int)current;

	pwm+=(current_error-current_errorlast)*current_kp+current_error*current_ki;

	current_errorlast=current_error;

  if(pwm<0) 
		pwm=0;
	else if(pwm>65536) 
		pwm=65536;

  pwm_u8=(unsigned char)(pwm>>8);

	return pwm_u8;
}


unsigned char speed_pi()	   
{
  static unsigned long pwm=0; 

	unsigned char pwm_u8=0;
	
	speed_error=set_speed - speed;

	pwm+=(speed_error-speed_errorlast)*speed_kp+speed_error*speed_ki;

	speed_errorlast=speed_error;

  if(pwm<1) 
		pwm=1;
	else if(pwm>32767) 
		pwm=32767;

  pwm_u8=(unsigned char)(pwm>>8);

	return pwm_u8;
}


void timer_init()
{
	TMOD=0x51;
	TH1=0;
	TL1=0;
	EA=0;
	ET0=1;
	TH0=(65535-25000)/256;
	TL0=(65535-25000)%256;
	TR0=1;
	TR1=1;
}


unsigned char Get_ADC(bit ch)
{
  unsigned char i,dat1 = 0,dat2 = 0;
  
  CS = 0; CLK = 0;
  DIO = 1; _nop_(); _nop_();
  CLK = 1; _nop_(); _nop_();
  CLK = 0; DIO = 1; _nop_(); _nop_();
  CLK = 1;          _nop_(); _nop_();
  CLK = 0; DIO = ch; _nop_(); _nop_();
  CLK = 1;          _nop_(); _nop_();
  CLK = 0; DIO = 1; _nop_(); _nop_();

  for(i = 0; i < 8; i++)
  {
    CLK = 1; _nop_(); _nop_();
    CLK = 0; _nop_(); _nop_();
    dat1 = (dat1 << 1)|DIO;
  }
  
  for(i = 0; i < 8; i++)
  {
    dat2 = dat2|((unsigned char)DIO<<i);
    CLK = 1; _nop_(); _nop_();
    CLK = 0; _nop_(); _nop_();
  }
  CS = 1;
  
  return dat1;
}