/**********************************************************************************
 * 工程名  :
 * 描述    :通过手机发送短信控制LED
 * 实验平台:STC12XX
 * 库版本  :
 * 作者    :泥人通信模块开发平台团队 
 * 博客    :http://nirenelec.blog.163.com
 * 淘宝    :http://shop105683814.taobao.com
**********************************************************************************/

#include "string.h"
#include "delay.h"
#include "uart.h"
#include "temp.h"

#define Buf1_Max 60 					  //串口1缓存长度
#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
sbit Delay_1  			= P2^0;					//继电器1
sbit Delay_2  			= P2^1;					//继电器1

/*************  本地变量声明	**************/
xdata u8 Uart1_Buf[Buf1_Max];
xdata u8 Uart2_Buf[Buf2_Max];
unsigned char Password[5] = "0806";
xdata unsigned char phone[14 ] = "\"18539863403\"";
static unsigned char *content = "Temperature: ";
static unsigned char *SMS_Num="\"+8613010761500\"";
unsigned char Datas_Buff[8]= {0, 0, 0, 0, 0, 0, 0,'\0'};
u8 Times=0,First_Int = 0,shijian=0;
//u8 Time_count=0;
bdata u8 Flag;//定时器标志位
sbit Timer0_start =Flag^0;	//定时器0延时启动计数器
sbit Uart2_Start  =Flag^1;	//串口2开始接收数据
sbit Uart2_End		=Flag^2;	//串口2接收数据结束


/*************	本地函数声明	**************/
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
/*************  外部函数和变量声明*****************/


/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口2负责与GPRS模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
void main(void)
{
	GPIO_config();
	Uart1Init();
	Uart2Init();
	Timer0Init();
	EA=1;	//开总中断
	UART1_SendString("GPRS模块短信测试程序\r\n");
	UART1_SendString("GPRS模块在注册网络\r\n");
	Wait_CREG();
	UART1_SendString("GPRS模块注册成功\r\n");
	UART1_SendString("GPRS模块短信模式设置\r\n");
	Set_Text_Mode();
	UART1_SendString("设置成功，短信模式：TEXT\r\n");
	
	UART1_SendString("初始化完成\r\n");
	
	
	while(1)
	{
		//Check_New_Message();
		Ds18b20ReadTemp();
	}
	
}

/*******************************************************************************
* 函数名 : Uart1 
* 描述   : 串口1中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
    }
}

/*******************************************************************************
* 函数名 : Uart2
* 描述   : 串口2中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart2() interrupt 8
{
		IE2  &= ~0x01;   //关闭串口2中断
    if (S2CON & S2RI)
    {
      S2CON &= ~S2RI;         //清除S2RI位
			Uart2_Buf[First_Int] = S2BUF;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (S2CON & S2TI)
    {
      S2CON &= ~S2TI;         //清除S2TI位
    }
		IE2  |= 0x01;   //使能串口2中断
}
/*******************************************************************************
* 函数名 : Timer0_ISR
* 描述   : 定时器0中断服务入口函数,20ms中断一次
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	TR0=0;//关定时器
	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	TR0=1;//开定时器
}
/*******************************************************************************
* 函数名 : GPIO_config
* 描述   : IO口配置函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void	GPIO_config(void)
{
		P3M1 &= 0XC3;  //配置P32~P35为推挽输出
		P3M0 |=	~0XC3;
}
void Timer0Init(void)		//20毫秒@22.1184MHz
{
	AUXR &= 0x7F;	//12T模式
	TMOD &= 0xF0;	//设置定时器模式 16位重载
	TL0 = 0x00;		//设定定时器初值
	TH0 = 0x70;		//设定定时器初值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
	ET0 = 1;    	//使能定时器0中断
}
/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(u8 *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
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
* 函数名 : Set_Text_Mode
* 描述   : 设置短信为TEXT文本模式
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_Text_Mode(void)
{
//	unsigned char temp[50]="AT+CSCA=";									
	Sec_AT_Command("ATE0","OK",3);	                        //取消回显	  					
	Sec_AT_Command("AT+CNMI=3,2,0,0,0","OK",3);	            //新短信直接输出						
	Sec_AT_Command("AT+CMGF=1","OK",3);	                  	//TEXT模式	
	Sec_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);  //所有操作都在SIM卡中进行	
//	strcat(temp,SMS_Num);
//	Sec_AT_Command(temp,"OK",3);                            //设置短信中心号码
}

/*******************************************************************************
* 函数名 : Check_Message_rec
* 描述   : 检查是否有新信息，并执行信息内容指令
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Check_New_Message(void)
{
	u8 temp=0,i;
	if(strstr(Uart2_Buf,"+CMT")!=NULL)   		//若缓存字符串中含有"+CMT"就表示有新的短信
	{
		delay_ms(50);//等待数据全部接收完成
		UART1_SendString("收到新短信:\r\n");
		UART1_SendString(Uart2_Buf);			//把模块返回的信息原样输出
		if(strstr(Uart2_Buf,Password)!=NULL)
		{
			if(strstr(Uart2_Buf,"Switch1:on")!=NULL)
			{
				UART1_SendString("收到新短信:Switch1:on\r\n");
        Delay_1 = 0;
			}
			if(strstr(Uart2_Buf,"Switch1:off")!=NULL)
			{
				UART1_SendString("收到新短信:Switch1:off\r\n");
        Delay_1 = 1;
			}
			if(strstr(Uart2_Buf,"Switch2:on")!=NULL)
			{
				UART1_SendString("收到新短信:Switch2:on\r\n");
				Delay_2 = 0;
			}
			if(strstr(Uart2_Buf,"Switch2:off")!=NULL)
			{
				UART1_SendString("收到新短信:Switch2:off\r\n");
				Delay_2 = 1;
			}
			if(strstr(Uart2_Buf,"Check")!=NULL)
			{
				UART1_SendString("收到新短信:Check\r\n");
				UART1_SendString(&Uart2_Buf[12]);
				for(i=0;i<11;i++)
				    phone[1+i] = Uart2_Buf[12+i];
				Send_Text_Sms();
			}
			if(strstr(Uart2_Buf,"Alter")!=NULL)
			{
				UART1_SendString("收到新短信:Alter\r\n");
        UART1_SendString(&Uart2_Buf[First_Int-6]);
				Password[0] = Uart2_Buf[First_Int-6];
				Password[1] = Uart2_Buf[First_Int-5];
				Password[2] = Uart2_Buf[First_Int-4];
				Password[3] = Uart2_Buf[First_Int-3];
				UART1_SendString(Password);
				UART1_SendString("新密码\r\n");
			}
	  }
		CLR_Buf2();
		Sec_AT_Command("AT+CMGD=1,1","OK",3);//删除短信
	}
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
		UART1_SendString("注册中.....");
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
	  if(temp< 0)				//当温度值为负数
  	{
			Buff[0] = '-';
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由�0.5，还是在小数点后面。
  	}
 	else
  	{	
    Buff[0] = '+';			
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	  }
	Buff[1] = temp / 10000 + 0x30;
	Buff[2] = temp % 10000 / 1000 + 0x30;
	Buff[3] = temp % 1000 / 100 + 0x30;
	Buff[4] = '.';	
	Buff[5] = temp % 100 / 10 + 0x30;
	Buff[6] = temp % 10 + 0x30;
}

