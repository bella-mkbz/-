/***************************
������ʵ������·���������
����ʹ�ñ�������ע��ת��֮��
***************************/
#include "Sound_Wave.h"
const float csbc = 0.034;     //�������ٴ��浥Ԫ  
/*******************************************************************************
* ��������		   : �󷽳��������
* ��    ��       : �󷽲��Ծ���
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
* ��������		   : �󷽳��������
* ��    ��       : �󷽲��Ծ���
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
* ��������		   : �ҷ����������
* ��    ��       : �ҷ����Ծ���
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
* ��������		   : ǰ�����������
* ��    ��       : ǰ�����Ծ���
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
* ��������		   : ��ʱ��һ�жϴ�����
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