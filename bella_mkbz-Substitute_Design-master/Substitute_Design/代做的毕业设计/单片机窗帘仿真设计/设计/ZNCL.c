#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_dat P0			 		 //1602数据口


sbit LCD1602_rs=P2^5;					 //1602 I/O 定义
sbit LCD1602_rw=P2^6;					 //1602 I/O 定义
sbit LCD1602_e=P2^7;

sbit SCLK=P3^4;	                        //DS1302时钟
sbit SDA=P3^5;	                    	//DS1302数据
sbit RST=P3^6;	                    	//DS1302片选

sbit key_1=P1^0;						//设置按键
sbit key_2=P1^1;						//
sbit key_3=P1^2;						//
sbit key_4=P1^3;						//切换模式按键
										 
sbit IDR=P1^4;							//定义光敏电阻接口


uchar zz[]=				   //正转数据
{
	0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09
};


uchar fz[]=				   //反转数据
{
	0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01
};




uchar irdate[33];						  //存储接收到的红外信号
uchar irbyte[4];						  //存储对接收信号处理后的数据
uchar irtime,irflag,bitnum,irdateok;	  //定义接收红外所需要的变量
uint time_s;
uint DJ_dat;

uint ms;
uchar Time[12];	                            //存储时间数据     
uchar state,loop;
bit s1,open,now_state;
bit Serial_Flag;

void delay(uint T)							   //延时程序
{
	while(T--);
}

//定时写数据
void DS1302WriteByte(uchar dat)
{
	uchar i;
	SCLK=0;					 // 初始时钟线置为 0 0 0 0
	delay(20);
	for(i=0;i<8;i++)			 // 开始传输 8 8 8 8  个字节的数据
	{
		SDA=dat&0x01;			 // 取最低位，注意  2 DS1302 的数据和地址都是从最低位开始传输的
		delay(20);
		SCLK=1;					 // 时钟线拉高，制造上升沿， SDA  的数据被传输
		delay(20);
		SCLK=0;					 // 时钟线拉低，为下一个上升沿做准备
		dat>>=1;				 // 数据右移一位，准备传输下一位数据
	}
}
   //定时读数据
uchar DS1302ReadByte()
{
	uchar i,dat;
	delay(20);
	for(i=0;i<8;i++)
	{
		dat>>=1;				 // 要返回的数据左移一位
		if(SDA==1)				 // 当数据线为高时，证明该位数据为 1 1 1 1
		dat|=0x80;			 // 要传输数据的当前值置为 , 1, 若不是, , , , 则为 0 0 0 0
		SCLK=1;                 //拉高时钟线
		delay(20);
		SCLK=0;					 // 制造下降沿
		delay(20);
	}
	return dat;				 // 返回读取出的数据
}


uchar DS1302Read(uchar cmd)
{
	uchar dat;
	RST=0; 					// 初始  CE  线置为 0 0 0 0
	SCLK=0;					 // 初始时钟线置为 0 0 0 0
	RST=1; 					// 初始  CE  置为 1 1 1 1 ，传输开始
	DS1302WriteByte(cmd);		 // 传输命令字，要读取的时间
	dat=DS1302ReadByte(); 		// 读取要得到的时间
	SCLK=1;					 // 时钟线拉高
	RST=0;					 // 读取结束， CE 置为 0 0 0 0 ，结束数据的传输
	return dat; 				// 返回得到的时间
}

void DS1302Write(uchar cmd, uchar dat)
{
	RST=0; 					 // 初始  CE  线置为 0 0 0 0
	SCLK=0; 				 // 初始时钟线置为 0 0 0 0
	RST=1; 						 // 初始  CE  置为 1 1 1 1 ，传输开始
	DS1302WriteByte(cmd); 
	dat=(dat/10)*16+dat%10;
	DS1302WriteByte(dat);  		// 写入要修改的时间
	SCLK=1; 				 // 时钟线拉高
	RST=0; 					 // 读取结束， CE  置为 0 0 0 0 ，结束数据的传输
}


void Init_DS1302(void)
{
	if (DS1302Read(0xc1)!=3)
	{
		DS1302Write(0x8e,0x00);			// 写保护关
		DS1302Write(0xc0,3);
		DS1302Write(0x80,0); //秒			 // 初始秒值为 0 
		DS1302Write(0x82,18);//分			 // 初始分钟值为  0
		DS1302Write(0x84,6);//时 		 // 初始为 24 式 小时模式  初始时间为 0点
		DS1302Write(0x88,3);
		DS1302Write(0x8c,15);
		DS1302Write(0x8a,3);
		DS1302Write(0x90,0x01); 		 // 充电
		DS1302Write(0x8e,0x80);
	}else
	{ 	
		DS1302Write(0x8e,0x00);			// 写保护关
		DS1302Write(0x90,0x01); 		 // 充电
		DS1302Write(0x8e,0x80);	
	}
}

void read_time1()						 //实时读取DS1302中的时间数据
{
	uchar i,j;
	for (j=0;j<7;j++)					//依次读出秒.分.小时 存到Time数组
	{
		i=DS1302Read(0x81+j*2);
		i=(i/16)*10+i%16;
		Time[j]=i;
	}
	
	for (j=0;j<5;j++)
	{
		i=DS1302Read(0xc3+j*2);			 //读存到Ds1302静态RAM的数据
		i=(i/16)*10+i%16;
		Time[7+j]=i;
	}
	if(Time[7]>23||Time[9]>23||Time[8]>59||Time[10]>59)	 //若读出的数据不合常理，初始赋值
	{
		Time[7]=6;	                     //存储的是定时时间下限小时
		Time[8]=30;						 //存储的是定时时间下限分
		Time[9]=8;						 //存储的是定时时间上限小时
		Time[10]=0;						 //存储的是定时时间上限分
	}
	if(Time[11]>2)Time[11]=0;			 //存储的是处于那种模式（0手动模式   1自动模式      2定时模式）
}

void read_time()						 //读取DS1302 的当前时间
{
	uchar i;

	i=DS1302Read(0x81+0*2);
	i=(i/16)*10+i%16;
	Time[0]=i;
	
/*	if(Time[0]==0)
	{
		i=DS1302Read(0x81+1*2);			//0x83读出的是分
		i=(i/16)*10+i%16;				//读出的是十六进制BCD，转换为十进制
		Time[1]=i;						//存储
		if(Time[1]==0)
		{
			i=DS1302Read(0x81+2*2);
			i=(i/16)*10+i%16;
			Time[2]=i;
			if(Time[2]==0)
			{
				i=DS1302Read(0x81+3*2);
				i=(i/16)*10+i%16;
				Time[3]=i;
				i=DS1302Read(0x81+5*2);
				i=(i/16)*10+i%16;
				Time[5]=i;
				if(Time[3]==1)
				{
					i=DS1302Read(0x81+4*2);
					i=(i/16)*10+i%16;
					Time[4]=i;
					if(Time[4]==1)
					{
						i=DS1302Read(0x81+6*2);
						i=(i/16)*10+i%16;
						Time[6]=i;
					}
				}	
			}	
		}	
	}	*/	
}
   
void write_DS1302(void)						//  将设置好的时间、发送到DS1302   内
{										
	DS1302Write(0x8e,0x00);					//去保护
	DS1302Write(0xc4,Time[8]); 		
	DS1302Write(0xc6,Time[9]);
	DS1302Write(0xc8,Time[10]);
	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}

void write_SYS_mode(void)						//  将设置好的时间、 发送到DS1302   内
{										
	DS1302Write(0x8e,0x00);
	DS1302Write(0xcA,Time[11]);

	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}



void LCD1602_write(uchar order,dat)				  //1602 一个字节  处理
{
    LCD1602_e=0;
    LCD1602_rs=order;
    LCD1602_dat=dat;
    LCD1602_rw=0;
    LCD1602_e=1;
    delay(1);
    LCD1602_e=0;																								     
}

void LCD1602_writebyte(uchar *prointer)				   //1602 字符串    处理
{
    while(*prointer!='\0')
    {
        LCD1602_write(1,*prointer);
        prointer++;
    }
}

void LCD1602_cls()									 //1602 初始化
{
	LCD1602_write(0,0x01);     //1602 清屏 指令
	delay(1500);
	LCD1602_write(0,0x38);     // 功能设置 8位、5*7点阵
	delay(1500);
	LCD1602_write(0,0x0c);     //设置 光标   不显示开关、不显示光标、字符不闪烁
	LCD1602_write(0,0x06);
	LCD1602_write(0,0xd0);
	delay(1500);
}

void show()
{
	LCD1602_write(0,0x80);					 //在1602第一行显示
	if(state==1&&s1==1)						 //闪烁处理
	{
		LCD1602_writebyte("    ");
	}else

	LCD1602_writebyte(" ");
	if(state==4&&s1==1)						//显示小时
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[2]/10%10);
		LCD1602_write(1,0x30+Time[2]%10);
	}
	if(state==0&&s1==1)	                     //冒号做小数处理，每一秒闪烁一次
	{
		LCD1602_writebyte(" ");
	}else
	{
		LCD1602_writebyte(":");
	}
	if(state==5&&s1==1)						//显示分
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[1]/10%10);
		LCD1602_write(1,0x30+Time[1]%10);
	}
  

	LCD1602_write(0,0xc0);					  //1602第二行，c0=80+40
	if(Time[11]==2&&state>5)
	{
		LCD1602_writebyte("O:");			  //定时模式下的，定时时间下限
		if(state==6&&s1==1)					  //闪烁处理
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[7]/10%10);//显示定时时间下限 的小时
			LCD1602_write(1,0x30+Time[7]%10);
		}
		LCD1602_writebyte(":");
		if(state==7&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[8]/10%10); //显示定时时间下限 的分钟
			LCD1602_write(1,0x30+Time[8]%10);
		}

		LCD1602_writebyte("  C:");				 //显示时间上限
		if(state==8&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[9]/10%10);  //小时
			LCD1602_write(1,0x30+Time[9]%10);
		}
		LCD1602_writebyte(":");
		if(state==9&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[10]/10%10);  //分钟
			LCD1602_write(1,0x30+Time[10]%10);
		}
	}else
	{
		switch(Time[11])
		{
			case 0:
				LCD1602_writebyte(" Manual");		//手动
			break;
	
			case 1:
				LCD1602_writebyte(" Auto  ");	  //自动
			break;
	
			case 2:
				LCD1602_writebyte(" Timing");		//定时
			break;
		} 											//状态
		LCD1602_writebyte("  WIN:");
		if(open==0)
		{
			LCD1602_writebyte("OFF");
		}else
		{
			LCD1602_writebyte(" ON");	
		}
	}	
}


void qing()			                          //清空接收到的红外数据
{
	uint i;
	for(i=0;i<33;i++)
	{
		irdate[i]=0;
	}
}


void deal_with()							//红外数据处理，红外数据是十六进制
{
	uchar i,j,k,temp;
	k=1;									//K=1,因为存在irdate数组里接收到的第一位是 引导码，不需要
	for(j=0;j<4;j++)						//一帧红外数据包括，一位引导码，四位用户码
	{
		for(i=0;i<8;i++)					//循环8次，提出来一位数据
		{	
			temp=temp>>1;
			if(irdate[k]>7)					 //通过技术手册，可知，接受到的数据在4左右，是0，    8左右是1，取中间值，大于七，即为1
			{
				temp=temp|0x80;	
			}
			k++;
		}
		irbyte[j]=temp;						  //读出来的数据存储  ，数据的格式是16进制
	}
}

void key()									  //按键检测
{
	if(!key_4||irbyte[2]==0x08)				  //如果按键KEY4有按下，或者红外接收的信号0x08,那么就执行这个语句
	{
		delay(1000);
		if(!key_4||irbyte[2]==0x08)
		{
			while(!key_4||irbyte[2]==0x08)
			{
				qing();						   //清空存储红外的数组
				deal_with();				   //红外数据处理，红外数据是十六进制
			}
			Time[11]=(Time[11]+1)%3;		   //模式标志位加一  
			state=0;						   //清空选择标志位
			write_SYS_mode();				   //将设置好的时间发送到DS1302   内
		}
	}

	if(!key_1||irbyte[2]==0x0c)					//设置按鍵按下，或者紅外接收到的数据是0x0c就执行这条语句
	{											
		delay(1000);
		if(!key_1||irbyte[2]==0x0c)
		{
			while(!key_1||irbyte[2]==0x0c)
			{
				qing();
				deal_with();
			}
			if(Time[11]==2)
			{
				state=(state+1)%10;	
			}else
			{
				state=(state+1)%6;
			}

			if(state==0)
			{
				write_DS1302();
			}
		}
	}
	
	if(!key_2||irbyte[2]==0x18)				   
	{
		delay(1000);
		if(!key_2||irbyte[2]==0x18)
		{
			while(!key_2||irbyte[2]==0x18)
			{
				if(irdateok==1)
				{
					irdateok=0;
					deal_with();
				}	
			}
			switch(state)					//key1、key2在模式0（手动）下，当state=0打开窗帘
			{
				case 0:
					if(Time[11]==0)				//key1/key2在模式0（手动）下，当state=0打开或关闭
					{
						now_state=1;
					}	
				break;

		
				case 4:
					Time[2]=(Time[2]+1)%24;					 //state=4时，对小时进行加一处理
				break;
																					    
				case 5:
					Time[1]=(Time[1]+1)%60;				    //state=5时，对分钟进行加一处理
				break;
					 	case 6:	
					Time[7]=(Time[7]+1)%24;					//state=6时，对定时模式下的下限小时进行加一处理
				break;

				case 7:
					Time[8]=(Time[8]+1)%60;					//state=7时，对定时模式下的下限分钟进行处理
				break;

				case 8:
					Time[9]=(Time[9]+1)%24;				   //state=8时，对定时模式下的上限小时进行加一处理
				break;

				case 9:
					Time[10]=(Time[10]+1)%60;			   //state=9时，对定时模式下的上限分钟进行加一处理
				break;

		
			}
		}
	}
	if(!key_3||irbyte[2]==0x5e)								//减处理，介绍同上
	{
		delay(1000);
		if(!key_3||irbyte[2]==0x5e)
		{
			while(!key_3||irbyte[2]==0x5e)
			{
				if(irdateok==1)
				{
					irdateok=0;
					deal_with();
				}
			}
			switch(state)
			{
				case 0:
					if(Time[11]==0)
					{
						now_state=0;
					}
				break;

				
				case 4:
					if(Time[2]==0) Time[2]=23;
					else Time[2]--;
				break;

				case 5:
					if(Time[1]==0) Time[1]=59;
					else Time[1]--;
				break;
    	

				case 6:
					if(Time[7]==0) Time[7]=23;
					else Time[7]--;
				break;

				case 7:
				
					if(Time[8]==0) Time[8]=59;
					else Time[8]--;
					
				break;

				case 8:
					if(Time[9]==0) Time[9]=23;
					else Time[9]--;
				break;

				case 9:
					if(Time[10]==0) Time[10]=59;
					else Time[10]--;
				break;

			}
		}
	}
		
}

void proc()
{
	uint xiax,shiji,shangx;				   //定义三个变量，xiax是定时时间下限	  shangx定时时间上限  shiji实际时间
	if(now_state==1)			   //设置电机的旋转
	{
		if(DJ_dat<520)
		{
			P2=P2&0xf0|zz[loop];
			loop=(loop+1)%8;
			delay(150);
			if(loop==0) DJ_dat++;
		
		}else
		{
			P2=P2&0xf0;
			open=1;
		}
	}else
	{
		if(DJ_dat>0)
		{
			P2=P2&0xf0|fz[loop];
			loop=(loop+1)%8;
			delay(150);
			if(loop==0) DJ_dat--;
		
		}else
		{
			P2=P2&0xf0;
			open=0;
		}
	}
	if(Time[11]==1)					//模式1（自动模式），电机转动设置
	{							  
		if(IDR==0)
		{	
			nOw_state?1;			  		}e,se		{
			now_髏ite=0;	
I	|		
 }
		if(Tmme[91]<=2)	)			 ?/模实2〃秷时蘑式），典圾转疮申置	折合成吩钟进捫袢较
	{
		iax=TiM`[7]*60+Time[8];		 //枿时时剪下限
		sHcogx=Tile[9]*71+Timl[1癩;		 /+雳时甑间上镛	塻hiji=Time0]"&0+Tkme[1Y;		 �/�5妓时间�
		if╬閍x>3la.gX)				I //如1ㄊ钡D嗣娃打愈蒓限，因螉是05小蜀制 说明是晚镐的�(时
		{
		if(shiji>=xiax||shijis(`nfx)		 //实姳间大于下限，或者小与上宿
	�;
			nO鱛sd醫e�1;=	 /o关懻窗伇		)
	I	}else if(whi*ixiax&fshi*i>=shcn鏧)	!o+在ǘㄊ笔本湟酝猓蚩傲�
		{
		�	now_Stat%=0;	
	I	}
		}else iF(sha飁}>xaax					 /锵孪摒∮疑螼蓿�0嘴於ㄊ�-
I	y
		塱f(shiji�=xIax6"shiji|shangx)		 //于定时时间之间关闭窗帘I
		{
			now_sTatE=0;										}else if(3hiji>=shangx||shiji<Xiax+	  //�3在定时时斾，启动4傲�
	H	y
				now_state=0;	
			}
		}	
	}I
}
M
void ma閚(+
{
	tMD=2pq;	`//定时器工佐方式设定	
I\H1=0z3c;�	TL1=2xb0;M
	D@0=0Hec;�-/	I	   	訦0=0X77;J	EA=1�  /+�(喰断�
	ET0=5;/'躁许T0中断J	TZ0}1;	  //起动T0
	QX0?5;	 /.允愴黻部仔断INT_中鲞	,
	ET1�1;	 //允许T1�断*	TR%?1;
	AT2=19	//允虚定时器T0中断
	SSON5 X51;
	@CO=0X00;
	ES =1;	Lnit_DS10:,);
	reAd_t)-E1(+;
//vead_Tkme�();
	LCD5v02]cls(9;
	whIlm1)
	{
)	if(k騞atook==1)M
	{
			i6dateok=0;
		tda,_wit`();
			qing((;
		}M
	塰ey(-�
	I{how();
=)mn(rtate5=0) read_time();
	}}

J
void ind0() interrupt�0				 .'镶部中蚕0用于检测红外辛概
{
tim%_s=0;						
	if(izvlAg==1)
	{	Iid(irtime>36)				  //检测红吐引导裸
		{
			bitnum=0;
	}
�	irdate[fitnuoU=irtime?
		IBtyme}0;
		jitnum*;
		if(jitnum=533				 � //红外耘号＜算上引导麻一共有73螞-
	I{		bIt顄m=0;				   //清粒，为下次鹿用做准1�
		yrda鬳ok=1;			`  /?33�?数�存粹完憦，红外揭恃申毕标志位置一
	]
	}
Ielse
{
		ipfl!g=1;*		irvile=0;
	}}/J
         `  (0

void init_1,) interrupT 5	) //定时器1，铀于设置闪烁豹之位毻
�
	TH0=0xf6;
	TLx=x3b;	
	ms++;
if(ms%200==0)
	s		s1=!S1;	
	}M
	hf(m36#99)-	{
		ms=0+M

	proc();
}M

voy� Sdrhal(void9 ifTerrwpt �
{
  stath' char BuFf[0],i;
	if(DI)
	z
	  RI=0;-
	  Buff[i] = RBUF;
		i++;
		if(Buff[	U �=0'#'!
		k
			
	�  C5ffKy]"= '\8';
			if(stRstr(#on#*,Cuff) != JU\L)
			{			( TI = 1;
		�	0rintF("Hello$W/rl�"-;*			while(!tI);M
			TI = 0;
�)塵J      i=0;
�	
		
	}
y
