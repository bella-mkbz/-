/***********************************************************
功能：DHT22驱动程序
说明：此程序运行硬件环境为STC12C5A60S2，该单片机（1T）与传统
的89C51单片机（12T）指令运行周期不同，使用的时候只需要修改
延时函数部分即可。
************************************************************/

#include <DHT22.h>


// 变量定义
unsigned char Sensor_Data[5]={0x00,0x00,0x00,0x00,0x00};
unsigned char Sensor_Check;		    //校验和

unsigned char Sensor_AnswerFlag;  //收到起始标志位
unsigned char Sensor_ErrorFlag;   //读取传感器错误标志
unsigned int  Sys_CNT;
unsigned int  Tmp,temprature,humidity;
unsigned int  temp_limit = 15;
unsigned char *String;

//字符串定义
#define S_Temp "Temp:"
#define S_RH   "RH:"
#define S_CRCF "Check: Wrong"
#define S_Data "Data: "
#define S_NotS "Sensor Not Connected"


/********************************************\
|* 功能： 延时	晶振为11.0592M时			       *|
|*  该延时是1T指令模式下，传统的89C51需要修改*|
\********************************************/
void Delay500us()		
{
		unsigned char i, j;

		i = 6;
		j = 93;
		do
		{
			while (--j);
		} while (--i);
}
/********************************************\
|* 功能： 延时	晶振为11.0592M时			 *|
|*  该延时是1T指令模式下，传统的89C51需要修改*|
\********************************************/
void Delay30us()		
{
		unsigned char i;

		i = 80;
		while (--i);
}

/********************************************\
|* 功能： 初始化串口					    *|
\********************************************/
void InitUART(void)
{
    PCON &= 0x7F;		
	  SCON = 0x50;		
	  AUXR &= 0xBF;	
	  AUXR &= 0xFE;		
	  TMOD &= 0x0F;	
	  TMOD |= 0x20;		
	  TL1 = 0xFD;		
	  TH1 = 0xFD;		
	  ET1 = 0;		
	  TR1 = 1;		
}  

/********************************************\
|* 功能： 串口发送函数 			            *|
\********************************************/
void UARTSend(char UCHAR)
{
    SBUF=UCHAR;
    while(TI==0);
    TI=0;
}

 /********************************************\
|* 功能： 串口发送子函数 			        *|
\********************************************/
void UART_PutString(unsigned char *buf)
{
	  while(*buf)
      UARTSend(*buf++);
}
void UART_PutStringAndNum(unsigned char *buf ,unsigned int num)
{
		unsigned char a[3],i;
		a[3] = '0'+num%10;
		a[2] = '.';
		a[1] = '0'+num/10%10;
		a[0] = '0'+num/100%10;
		while(*buf)
				UARTSend(*buf++);
		UARTSend(' ');
		for(i=0;i<4;i++)
		{
			UARTSend(a[i]);
		} 
}
void UART_PutStringAnd_Data(unsigned char *buf ,unsigned char *bufdata)
{
		unsigned char a[2],i,j;
		while(*buf)
				UARTSend(*buf++);
		UARTSend(' ');
		for(i=0;i<5;i++)
		{
			a[0] = bufdata[i]/16; 
			a[1] = bufdata[i]%16;
			for(j=0;j<2;j++)
			{
				if(a[j]>9)
				{
					a[j] = (a[j]-10)+'A';
				}
				else
				{
					a[j] = a[j]+'0';
				}
				UARTSend(a[j]);
			}
			UARTSend(' ');
		} 
}
/********************************************\
|* 功能： 串口发送传感器数据函数 			*|
\********************************************/
void UARTSend_Nbyte(void)
{
	   if(Sensor_AnswerFlag == 1)
	   {
	    Sensor_Check = Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3];
		  if(Sensor_Check ==Sensor_Data[4])	 		  //校验成功
		  {
		       String = S_RH;//"RH:";	   
	         Tmp = Sensor_Data[0]*256+Sensor_Data[1];
           humidity = Tmp;				
	         UART_PutStringAndNum(String,Tmp); 
	   		   UARTSend(' ');
	         UARTSend(' ');
	         
			     String = S_Temp;// "Temp:";	   
	       	 Tmp = Sensor_Data[2]*256+Sensor_Data[3];
           temprature =	Tmp;			
	         UART_PutStringAndNum(String,Tmp);
		   	   UARTSend(' ');
	         UARTSend(' ');
		  }
			else	//校验失败 送上读到数据
		  {
             String = S_Data;//"Data: ";
		     UART_PutStringAnd_Data(String,Sensor_Data);
			 UARTSend(' ');	
			 UARTSend(' ');
			 String = S_CRCF;//"Check: Wrong";
		     UART_PutString(String); 
		  }
		}
		else // 传感器未连接
		{
		   String = S_NotS; //"Sensor Not Connected";
 		   UART_PutString(String);
		}	    
	  UARTSend(0x0A); 
    memset(Sensor_Data,0x00,5);
}  

/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
unsigned char Read_SensorData(void)
{
		unsigned char i,cnt;
		unsigned char buffer,tmp;
		buffer = 0;
		for(i=0;i<8;i++)
		{
			cnt=0;
			while(!Sensor_SDA)	//检测上次低电平是否结束
			{
				if(++cnt >= 300)
				 {
					break;
				 }
			}
			//延时Min=26us Max50us 跳过数据"0" 的高电平  
			Delay30us();             //延时30us 
			//判断传感器发送数据位
			tmp =0;
			if(Sensor_SDA)	 
			{
				tmp = 1;
			}  
			cnt =0;
			while(Sensor_SDA)		//等待高电平 结束
			{
					if(++cnt >= 200)
				{
					break;
				}
			}
			buffer <<=1;
			buffer |= tmp;	
		}
		return buffer;
}

/********************************************\
|* 功能： 读传感器              	        *|
\********************************************/
unsigned char Read_Sensor(void)
{
		unsigned char i;
		//主机拉低500us
			Sensor_SDA = 0;
		Delay500us();       //延时500us
		//释放总线 延时(Min=20us Max=40us)
		Sensor_SDA = 1; 	
		Delay30us();        //延时30us
		//主机设为输入 判断传感器响应信号 
		Sensor_SDA = 1; 
					
		Sensor_AnswerFlag = 0;  // 传感器响应标志	 

		//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
		if(Sensor_SDA ==0)
		{
			 Sensor_AnswerFlag = 1;//收到起始信号
			 Sys_CNT = 0;
			 //判断从机是否发出 80us 的低电平响应信号是否结束	 
			 while((!Sensor_SDA))
			 {
				 if(++Sys_CNT>300) //防止进入死循环
			 {
				 Sensor_ErrorFlag = 1;
				 return 0;
				} 
				}
				Sys_CNT = 0;
				//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
				while((Sensor_SDA))
				{
					 if(++Sys_CNT>300) //防止进入死循环
				 {
					 Sensor_ErrorFlag = 1;
					 return 0;
				 } 
				} 		 
				// 数据接收	传感器共发送40位数据 
				// 即5个字节 高位先送  5个字节分别为湿度高位 湿度低位 温度高位 温度低位 校验和
				// 校验和为：湿度高位+湿度低位+温度高位+温度低位
			// 当温度为负时，参考该芯片的说明书查看具体的数据格式
				for(i=0;i<5;i++)
				{
					Sensor_Data[i] = Read_SensorData();
				}
			}
			else
			{
				Sensor_AnswerFlag = 0;	  // 未收到传感器响应	
			}
			return 1;
}

void Beep_Alarm()
{
		if((temprature/10) >= temp_limit)
			 Beep = 0;
		else
			 Beep = 1;
}

void Show()
{
		LCD_Write_String(0,0,"Temp: ");
		LCD_Write_String(0,1,"Humidity: ");
		LCD_Write_Char(6,0,'0'+temprature/100%10);
		LCD_Write_Char(7,0,'0'+temprature/10%10);
		LCD_Write_Char(8,0,'.');
		LCD_Write_Char(9,0,'0'+temprature%10);
		
		LCD_Write_Char(10,1,'0'+humidity/100%10);
		LCD_Write_Char(11,1,'0'+humidity/10%10);
		LCD_Write_Char(12,1,'.');
		LCD_Write_Char(13,1,'0'+humidity%10);
}

