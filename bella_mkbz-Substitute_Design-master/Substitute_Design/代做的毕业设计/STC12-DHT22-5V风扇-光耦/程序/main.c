#include  "STC12C5Axx.h"
#include  "DHT22.h"
#include  "uart.h"
#include	"delay.h"
#include  "button.h"

unsigned int temprature, humidity, Set_temprature = 200, Set_humidity = 250;

sbit Motor_1 = P3^7;
sbit Motor_2 = P3^6;

unsigned char Key_Value = 0, Key_Flag = 0;

void main(void)
{
		unsigned int Fd = 0;
		Uart1Init();
	  while(1)
		{
			Key_Value = Button_Value();
			switch (Key_Value)
      {
      	case 1:
					Key_Flag = ~Key_Flag;
      		break;
      	case 2:
					if(Key_Flag)
						Set_temprature = Set_temprature - 20;
					else
						Set_humidity = Set_humidity - 20;
      		break;
      	case 3:
					if(Key_Flag)
						Set_temprature = Set_temprature + 20;
					else
						Set_humidity = Set_humidity + 20;				
      		break;				
      	default:
      		break;
      }
			Fd = Read_SensorData(&temprature, &humidity);
			if(Fd == 1)
			{
				if(temprature >= Set_temprature)
					Motor_1 = 0;
				else
					Motor_1 = 1;
			  if(humidity >= Set_humidity)
					Motor_2 = 0;
				else
					Motor_2 = 1;
			}
			UART1_SendData(humidity >> 8);
			UART1_SendData(humidity & 0xff);
		}
		
}