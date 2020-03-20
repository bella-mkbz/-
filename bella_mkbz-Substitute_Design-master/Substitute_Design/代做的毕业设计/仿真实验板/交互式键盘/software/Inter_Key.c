#include "Inter_Key.h"
#include "delay.h"

unsigned char Keyboard(void)
{
   unsigned char temp;
   static unsigned char Value=0;
    /***第一列拉低***/
    P2 = 0XFE;			       
	Delay1ms(1);
	temp = P2;
	if(temp!=0XFE)
	{
	  Delay1ms(10);
	  temp = P2;
	  if(temp!=0XFE)
	    {
		  switch (temp)
		  {
		  	case 0xfc:return Value=1;
				break;
			case 0xfa:return Value=2;
				break;
			case 0xf7:return Value=3;
				break;
			case 0xee:return Value=4;
				break;
			case 0xde:return Value=5;
				break;
			case 0xbe:return Value=6;
				break;
			case 0x7e:return Value=7;
				break;
			default:
				break;
		   }
		}
	}
    /***************/
	/***第二列拉低***/
	P2 = 0XFD;
    Delay1ms(1);
	temp = P2;
	if(temp!=0XFD)
	 {
	  Delay1ms(10);
	  temp = P2;	   
       if(temp!=0XFD)
	    {
	      switch (temp)
		  {
		  	case  :
				break;
			case  : 
				break;
		  	case  :
				break;
			case  : 
				break;
		  	case  :
				break;
			case  : 
				break;
		  	case  :
				break;
			default:
				break;
		  }
	    }
	 }
	/***************/
   return Value;
}



















