	   
//						switch (i)
//				{
//					case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=Duan[0];Delay1ms(500); break;
//					case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=Duan[1];Delay1ms(500); break;
//					case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=Duan[2];Delay1ms(500); break;
//					case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=Duan[3];Delay1ms(500); break;
//					case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=Duan[4];Delay1ms(500); break;
//					case 5: ADDR2=1; ADDR1=0; ADDR0=1; i=0; P0=Duan[5];Delay1ms(500); break;
//					default: break;
//				}




数码管不移动显示   修改i的值可以改变数码管延长时间
	  for(i=0;i<6;i++)
 	 {
	     P0=0XFF;
		 P1 &= 0XF8;
		 P1 |= i;
		 P0 = Duan[i];
		 Delay1ms(1);
	 }

数码管移动显示
for(j=0;j<250;j++)
	  {
	      for(i=0;i<300;i++)
		   {
		   P0=0XFF;
			 P1 &= 0XF8;
			 P1 |= n;
			 P0 = Duan[(n+m)%6];
			  Delay1ms(1);
			  n++;
		    if(n == 6)
		       n=0;
		   }
		   m++;
		   if(m == 6)
		   m=0;
	  }