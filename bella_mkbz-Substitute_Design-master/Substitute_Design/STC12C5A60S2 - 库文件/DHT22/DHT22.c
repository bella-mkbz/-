/***********************************************************
功能：DHT22驱动程序
说明：此程序运行硬件环境为STC12C5A60S2，该单片机（1T）与传统
的89C51单片机（12T）指令运行周期不同，使用的时候只需要修改
延时函数部分即可。
************************************************************/
#include "DHT22.h"

// 变量定义
unsigned char Sensor_Data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char Sensor_Check; //校验和

unsigned char Sensor_AnswerFlag; //收到起始标志位
unsigned char Sensor_ErrorFlag;  //读取传感器错误标志
unsigned int  Sys_CNT;
unsigned int  Tmp;

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
		while (--j)
			;
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
	while (--i)
		;
}

/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
unsigned char Read_SensorWord(void)
{
	unsigned char i, cnt;
	unsigned char buffer, tmp;
	buffer = 0;
	for (i = 0; i < 8; i++)
	{
		cnt = 0;
		while (!Sensor_SDA) //检测上次低电平是否结束
		{
			if (++cnt >= 300)
			{
				break;
			}
		}
		//延时Min=26us Max50us 跳过数据"0" 的高电平
		Delay30us(); //延时30us
		//判断传感器发送数据位
		tmp = 0;
		if (Sensor_SDA)
		{
			tmp = 1;
		}
		cnt = 0;
		while (Sensor_SDA) //等待高电平 结束
		{
			if (++cnt >= 200)
			{
				break;
			}
		}
		buffer <<= 1;
		buffer |= tmp;
	}
	return buffer;
}

/********************************************\
|* 功能： 读传感器              	         *|
\********************************************/
void Read_Sensor(void)
{
	unsigned char i;
	//主机拉低500us
	Sensor_SDA = 0;
	Delay500us();   //延时500us
	Sensor_SDA = 1; //释放总线 延时(Min=20us Max=40us)
	Delay30us();	//延时30us

	Sensor_SDA = 1; //主机设为输入 判断传感器响应信号

	Sensor_AnswerFlag = 0; // 传感器响应标志

	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	if (Sensor_SDA == 0)
	{
		Sensor_AnswerFlag = 1; //收到起始信号
		Sys_CNT = 0;
		//判断从机是否发出 80us 的低电平响应信号是否结束
		while ((!Sensor_SDA))
		{
			if (++Sys_CNT > 300) //防止进入死循环
			{
				Sensor_ErrorFlag = 1;
			}
		}
		Sys_CNT = 0;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while ((Sensor_SDA))
		{
			if (++Sys_CNT > 300) //防止进入死循环
			{
				Sensor_ErrorFlag = 1;
			}
		}
		// 数据接收	传感器共发送40位数据
		// 即5个字节 高位先送  5个字节分别为湿度高位 湿度低位 温度高位 温度低位 校验和
		// 校验和为：湿度高位+湿度低位+温度高位+温度低位
		// 当温度为负时，参考该芯片的说明书查看具体的数据格式
		for (i = 0; i < 5; i++)
		{
			Sensor_Data[i] = Read_SensorWord();
		}
	}
	else
	{
		Sensor_AnswerFlag = 0; // 未收到传感器响应
	}
}

/********************************************\
|* 功能： 读取传感器完整数据函数 			       *|
|* 输入： 待存放的温度、湿度数据地址 		     *|
|* 输出：  1  读取数据成功       	     	     *|
|*         0  传感器未连接                   *|
|*		     2  数据校验失败                   *|
\********************************************/
unsigned char Read_SensorData(unsigned int *temprature, unsigned int *humidity)
{
	Read_Sensor();
	if (Sensor_AnswerFlag == 1)
	{
		Sensor_Check = Sensor_Data[0] + Sensor_Data[1] + Sensor_Data[2] + Sensor_Data[3];
		if (Sensor_Check == Sensor_Data[4]) //校验成功
		{

			Tmp = Sensor_Data[0] * 256 + Sensor_Data[1];
			*humidity = Tmp;

			Tmp = Sensor_Data[2] * 256 + Sensor_Data[3];
			*temprature = Tmp;
		}
		else //校验失败 送上读到数据
		{
			memset(Sensor_Data, 0x00, 5); //把校验失败的数据删除
			return 2;
		}
	}
	else // 传感器未连接
	{
		return 0;
	}
	memset(Sensor_Data, 0x00, 5);
	return 1;
}
