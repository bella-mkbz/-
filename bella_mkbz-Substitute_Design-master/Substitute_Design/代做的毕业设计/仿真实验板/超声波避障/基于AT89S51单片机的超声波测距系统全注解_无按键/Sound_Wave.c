/***************************
本程序实现了四路超声波测距
如需使用本程序请注明转载之处
***************************/
#include "Sound_Wave.h"
const float csbc = 0.034;     //超声波速储存单元  
/*******************************************************************************
* 函数功能		   : 后方超声波测距
* 输    出       : 后方测试距离
*******************************************************************************/	
unsigned int csbcj_1()					
{
	static unsigned int jsz,t,i;
	TH0=0x00;				       
	TL0=0x00;              
	csbint_1=1;               
	csbout_1=1;
	Delay12us();
	csbout_1=0;
	Delay12us();
	csbout_1=1;               
	TR0=1;					       		 
	i=250;		              
	while(i--)             
	{
	}
	i=0;
	while(csbint_1)			
	{
		i++;
		if(i>=3300)        		
		csbint_1=0;           
	}
	TR0=0;                 
	t=TH0;					       
	t=t*256+TL0;           
	csbint_1=1;               
	jsz=t*csbc;				      
	jsz=jsz/2;              
	return jsz;
}
/*******************************************************************************
* 函数功能		   : 左方超声波测距
* 输    出       : 左方测试距离
*******************************************************************************/
unsigned int csbcj_2()					
{
	static unsigned int jsz,t,i;
	TH0=0x00;				       
	TL0=0x00;              
	csbint_2=1;              
	csbout_2=1;
	Delay12us();
	csbout_2=0;
	Delay12us();
	csbout_2=1;            
	TR0=1;					      	 
	i=250;		       		
	while(i--)          
	{
	}
	i=0;
	while(csbint_2)			
	{
		i++;
		if(i>=3300)       		
		csbint_2=0;          
	}
	TR0=0;               
	t=TH0;					       
	t=t*256+TL0;           
	csbint_2=1;              
	jsz=t*csbc;				    
	jsz=jsz/2;             
	return jsz;
}
/*******************************************************************************
* 函数功能		   : 右方超声波测距
* 输    出       : 右方测试距离
*******************************************************************************/	
unsigned int csbcj_3()					
{
	static unsigned int jsz,t,i;
	TH0=0x00;				      
	TL0=0x00;               
	csbint_3=1;              
	csbout_3=1;
	Delay12us();
	csbout_3=0;
	Delay12us();
	csbout_3=1;              
	TR0=1;					        		 
	i=250;		           
	while(i--)            
	{
	}
	i=0;
	while(csbint_3)			
	{
		i++;
		if(i>=3300)        		
		csbint_3=0;         
	}
	TR0=0;                  
	t=TH0;					     
	t=t*256+TL0;          
	csbint_3=1;             
	jsz=t*csbc;				     
	jsz=jsz/2;             
	return jsz;
}
/*******************************************************************************
* 函数功能		   : 前方超声波测距
* 输    出       : 前方测试距离
*******************************************************************************/	
unsigned int csbcj_4()					
{
	static unsigned int jsz,t,i;
	TH0=0x00;				      
	TL0=0x00;              
	csbint_4=1;              
	csbout_4=1;
	Delay12us();
	csbout_4=0;
	Delay12us();
	csbout_4=1;               
	TR0=1;					       		 
	i=250;		            	
	while(i--)            
	{
	}
	i=0;
	while(csbint_4)			
	{
		i++;
		if(i>=3300)         		
		csbint_4=0;          
	}
	TR0=0;                  
	t=TH0;					        
	t=t*256+TL0;           
	csbint_4=1;              
	jsz=t*csbc;				   
	jsz=jsz/2;             
	return jsz;
}

void Delay12us()		     
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
/*******************************************************************************
* 函数功能		   : 定时器一中断处理函数
*******************************************************************************/
void Timer_1(void) interrupt 3
{
	static unsigned int temp;
	Show_String(0x80,"F : ");
	Show_String(0x88,"B : ");
	Show_String(0xC0,"L : ");
	Show_String(0xC8,"R : ");
  TH1 = 0X3C;
  TH0 = 0XB0;
	temp = csbcj_1();
	Show_Dec_Number(0x8D,temp);
	temp = csbcj_2();
	Show_Dec_Number(0xc4,temp);
	temp = csbcj_3();
	Show_Dec_Number(0xcD,temp);
	temp = csbcj_4();
	Show_Dec_Number(0x84,temp);
}