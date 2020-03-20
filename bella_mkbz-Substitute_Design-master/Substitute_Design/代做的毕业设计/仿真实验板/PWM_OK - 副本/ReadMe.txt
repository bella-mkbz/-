

本程序看i用定时器0产生PWM波形，控制LED调光

通过按键逐级地进行调节共有五个挡位

下面的程序实现呼吸灯的功能，即PWM的自动调节
//	if(Flag)
//	{
//	  Flag = 0;
//	  if(Addr == 0)
//	   {
//	   	 Pulse -=10;
//		 if(Pulse == 0)
//		    Addr = 1;
//	   }
//	  else
//	   {
//	     Pulse +=10;
//		 if(Pulse == 100)
//		    Addr = 0;
//	   }
//	}