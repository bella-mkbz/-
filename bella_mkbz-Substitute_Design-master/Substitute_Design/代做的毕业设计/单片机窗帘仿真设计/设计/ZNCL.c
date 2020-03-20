#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_dat P0			 		 //1602���ݿ�


sbit LCD1602_rs=P2^5;					 //1602 I/O ����
sbit LCD1602_rw=P2^6;					 //1602 I/O ����
sbit LCD1602_e=P2^7;

sbit SCLK=P3^4;	                        //DS1302ʱ��
sbit SDA=P3^5;	                    	//DS1302����
sbit RST=P3^6;	                    	//DS1302Ƭѡ

sbit key_1=P1^0;						//���ð���
sbit key_2=P1^1;						//
sbit key_3=P1^2;						//
sbit key_4=P1^3;						//�л�ģʽ����
										 
sbit IDR=P1^4;							//�����������ӿ�


uchar zz[]=				   //��ת����
{
	0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09
};


uchar fz[]=				   //��ת����
{
	0x09,0x08,0x0c,0x04,0x06,0x02,0x03,0x01
};




uchar irdate[33];						  //�洢���յ��ĺ����ź�
uchar irbyte[4];						  //�洢�Խ����źŴ���������
uchar irtime,irflag,bitnum,irdateok;	  //������պ�������Ҫ�ı���
uint time_s;
uint DJ_dat;

uint ms;
uchar Time[12];	                            //�洢ʱ������     
uchar state,loop;
bit s1,open,now_state;
bit Serial_Flag;

void delay(uint T)							   //��ʱ����
{
	while(T--);
}

//��ʱд����
void DS1302WriteByte(uchar dat)
{
	uchar i;
	SCLK=0;					 // ��ʼʱ������Ϊ 0 0 0 0
	delay(20);
	for(i=0;i<8;i++)			 // ��ʼ���� 8 8 8 8  ���ֽڵ�����
	{
		SDA=dat&0x01;			 // ȡ���λ��ע��  2 DS1302 �����ݺ͵�ַ���Ǵ����λ��ʼ�����
		delay(20);
		SCLK=1;					 // ʱ�������ߣ����������أ� SDA  �����ݱ�����
		delay(20);
		SCLK=0;					 // ʱ�������ͣ�Ϊ��һ����������׼��
		dat>>=1;				 // ��������һλ��׼��������һλ����
	}
}
   //��ʱ������
uchar DS1302ReadByte()
{
	uchar i,dat;
	delay(20);
	for(i=0;i<8;i++)
	{
		dat>>=1;				 // Ҫ���ص���������һλ
		if(SDA==1)				 // ��������Ϊ��ʱ��֤����λ����Ϊ 1 1 1 1
		dat|=0x80;			 // Ҫ�������ݵĵ�ǰֵ��Ϊ , 1, ������, , , , ��Ϊ 0 0 0 0
		SCLK=1;                 //����ʱ����
		delay(20);
		SCLK=0;					 // �����½���
		delay(20);
	}
	return dat;				 // ���ض�ȡ��������
}


uchar DS1302Read(uchar cmd)
{
	uchar dat;
	RST=0; 					// ��ʼ  CE  ����Ϊ 0 0 0 0
	SCLK=0;					 // ��ʼʱ������Ϊ 0 0 0 0
	RST=1; 					// ��ʼ  CE  ��Ϊ 1 1 1 1 �����俪ʼ
	DS1302WriteByte(cmd);		 // ���������֣�Ҫ��ȡ��ʱ��
	dat=DS1302ReadByte(); 		// ��ȡҪ�õ���ʱ��
	SCLK=1;					 // ʱ��������
	RST=0;					 // ��ȡ������ CE ��Ϊ 0 0 0 0 ���������ݵĴ���
	return dat; 				// ���صõ���ʱ��
}

void DS1302Write(uchar cmd, uchar dat)
{
	RST=0; 					 // ��ʼ  CE  ����Ϊ 0 0 0 0
	SCLK=0; 				 // ��ʼʱ������Ϊ 0 0 0 0
	RST=1; 						 // ��ʼ  CE  ��Ϊ 1 1 1 1 �����俪ʼ
	DS1302WriteByte(cmd); 
	dat=(dat/10)*16+dat%10;
	DS1302WriteByte(dat);  		// д��Ҫ�޸ĵ�ʱ��
	SCLK=1; 				 // ʱ��������
	RST=0; 					 // ��ȡ������ CE  ��Ϊ 0 0 0 0 ���������ݵĴ���
}


void Init_DS1302(void)
{
	if (DS1302Read(0xc1)!=3)
	{
		DS1302Write(0x8e,0x00);			// д������
		DS1302Write(0xc0,3);
		DS1302Write(0x80,0); //��			 // ��ʼ��ֵΪ 0 
		DS1302Write(0x82,18);//��			 // ��ʼ����ֵΪ  0
		DS1302Write(0x84,6);//ʱ 		 // ��ʼΪ 24 ʽ Сʱģʽ  ��ʼʱ��Ϊ 0��
		DS1302Write(0x88,3);
		DS1302Write(0x8c,15);
		DS1302Write(0x8a,3);
		DS1302Write(0x90,0x01); 		 // ���
		DS1302Write(0x8e,0x80);
	}else
	{ 	
		DS1302Write(0x8e,0x00);			// д������
		DS1302Write(0x90,0x01); 		 // ���
		DS1302Write(0x8e,0x80);	
	}
}

void read_time1()						 //ʵʱ��ȡDS1302�е�ʱ������
{
	uchar i,j;
	for (j=0;j<7;j++)					//���ζ�����.��.Сʱ �浽Time����
	{
		i=DS1302Read(0x81+j*2);
		i=(i/16)*10+i%16;
		Time[j]=i;
	}
	
	for (j=0;j<5;j++)
	{
		i=DS1302Read(0xc3+j*2);			 //���浽Ds1302��̬RAM������
		i=(i/16)*10+i%16;
		Time[7+j]=i;
	}
	if(Time[7]>23||Time[9]>23||Time[8]>59||Time[10]>59)	 //�����������ݲ��ϳ�����ʼ��ֵ
	{
		Time[7]=6;	                     //�洢���Ƕ�ʱʱ������Сʱ
		Time[8]=30;						 //�洢���Ƕ�ʱʱ�����޷�
		Time[9]=8;						 //�洢���Ƕ�ʱʱ������Сʱ
		Time[10]=0;						 //�洢���Ƕ�ʱʱ�����޷�
	}
	if(Time[11]>2)Time[11]=0;			 //�洢���Ǵ�������ģʽ��0�ֶ�ģʽ   1�Զ�ģʽ      2��ʱģʽ��
}

void read_time()						 //��ȡDS1302 �ĵ�ǰʱ��
{
	uchar i;

	i=DS1302Read(0x81+0*2);
	i=(i/16)*10+i%16;
	Time[0]=i;
	
/*	if(Time[0]==0)
	{
		i=DS1302Read(0x81+1*2);			//0x83�������Ƿ�
		i=(i/16)*10+i%16;				//��������ʮ������BCD��ת��Ϊʮ����
		Time[1]=i;						//�洢
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
   
void write_DS1302(void)						//  �����úõ�ʱ�䡢���͵�DS1302   ��
{										
	DS1302Write(0x8e,0x00);					//ȥ����
	DS1302Write(0xc4,Time[8]); 		
	DS1302Write(0xc6,Time[9]);
	DS1302Write(0xc8,Time[10]);
	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}

void write_SYS_mode(void)						//  �����úõ�ʱ�䡢 ���͵�DS1302   ��
{										
	DS1302Write(0x8e,0x00);
	DS1302Write(0xcA,Time[11]);

	DS1302Write(0x90,0x01); 		 		 
	DS1302Write(0x8e,0x80);
}



void LCD1602_write(uchar order,dat)				  //1602 һ���ֽ�  ����
{
    LCD1602_e=0;
    LCD1602_rs=order;
    LCD1602_dat=dat;
    LCD1602_rw=0;
    LCD1602_e=1;
    delay(1);
    LCD1602_e=0;																								     
}

void LCD1602_writebyte(uchar *prointer)				   //1602 �ַ���    ����
{
    while(*prointer!='\0')
    {
        LCD1602_write(1,*prointer);
        prointer++;
    }
}

void LCD1602_cls()									 //1602 ��ʼ��
{
	LCD1602_write(0,0x01);     //1602 ���� ָ��
	delay(1500);
	LCD1602_write(0,0x38);     // �������� 8λ��5*7����
	delay(1500);
	LCD1602_write(0,0x0c);     //���� ���   ����ʾ���ء�����ʾ��ꡢ�ַ�����˸
	LCD1602_write(0,0x06);
	LCD1602_write(0,0xd0);
	delay(1500);
}

void show()
{
	LCD1602_write(0,0x80);					 //��1602��һ����ʾ
	if(state==1&&s1==1)						 //��˸����
	{
		LCD1602_writebyte("    ");
	}else

	LCD1602_writebyte(" ");
	if(state==4&&s1==1)						//��ʾСʱ
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[2]/10%10);
		LCD1602_write(1,0x30+Time[2]%10);
	}
	if(state==0&&s1==1)	                     //ð����С������ÿһ����˸һ��
	{
		LCD1602_writebyte(" ");
	}else
	{
		LCD1602_writebyte(":");
	}
	if(state==5&&s1==1)						//��ʾ��
	{
		LCD1602_writebyte("  ");
	}else
	{
		LCD1602_write(1,0x30+Time[1]/10%10);
		LCD1602_write(1,0x30+Time[1]%10);
	}
  

	LCD1602_write(0,0xc0);					  //1602�ڶ��У�c0=80+40
	if(Time[11]==2&&state>5)
	{
		LCD1602_writebyte("O:");			  //��ʱģʽ�µģ���ʱʱ������
		if(state==6&&s1==1)					  //��˸����
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[7]/10%10);//��ʾ��ʱʱ������ ��Сʱ
			LCD1602_write(1,0x30+Time[7]%10);
		}
		LCD1602_writebyte(":");
		if(state==7&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[8]/10%10); //��ʾ��ʱʱ������ �ķ���
			LCD1602_write(1,0x30+Time[8]%10);
		}

		LCD1602_writebyte("  C:");				 //��ʾʱ������
		if(state==8&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[9]/10%10);  //Сʱ
			LCD1602_write(1,0x30+Time[9]%10);
		}
		LCD1602_writebyte(":");
		if(state==9&&s1==1)
		{
			LCD1602_writebyte("  ");
		}else
		{
			LCD1602_write(1,0x30+Time[10]/10%10);  //����
			LCD1602_write(1,0x30+Time[10]%10);
		}
	}else
	{
		switch(Time[11])
		{
			case 0:
				LCD1602_writebyte(" Manual");		//�ֶ�
			break;
	
			case 1:
				LCD1602_writebyte(" Auto  ");	  //�Զ�
			break;
	
			case 2:
				LCD1602_writebyte(" Timing");		//��ʱ
			break;
		} 											//״̬
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


void qing()			                          //��ս��յ��ĺ�������
{
	uint i;
	for(i=0;i<33;i++)
	{
		irdate[i]=0;
	}
}


void deal_with()							//�������ݴ�������������ʮ������
{
	uchar i,j,k,temp;
	k=1;									//K=1,��Ϊ����irdate��������յ��ĵ�һλ�� �����룬����Ҫ
	for(j=0;j<4;j++)						//һ֡�������ݰ�����һλ�����룬��λ�û���
	{
		for(i=0;i<8;i++)					//ѭ��8�Σ������һλ����
		{	
			temp=temp>>1;
			if(irdate[k]>7)					 //ͨ�������ֲᣬ��֪�����ܵ���������4���ң���0��    8������1��ȡ�м�ֵ�������ߣ���Ϊ1
			{
				temp=temp|0x80;	
			}
			k++;
		}
		irbyte[j]=temp;						  //�����������ݴ洢  �����ݵĸ�ʽ��16����
	}
}

void key()									  //�������
{
	if(!key_4||irbyte[2]==0x08)				  //�������KEY4�а��£����ߺ�����յ��ź�0x08,��ô��ִ��������
	{
		delay(1000);
		if(!key_4||irbyte[2]==0x08)
		{
			while(!key_4||irbyte[2]==0x08)
			{
				qing();						   //��մ洢���������
				deal_with();				   //�������ݴ�������������ʮ������
			}
			Time[11]=(Time[11]+1)%3;		   //ģʽ��־λ��һ  
			state=0;						   //���ѡ���־λ
			write_SYS_mode();				   //�����úõ�ʱ�䷢�͵�DS1302   ��
		}
	}

	if(!key_1||irbyte[2]==0x0c)					//���ð��I���£����߼t����յ���������0x0c��ִ���������
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
			switch(state)					//key1��key2��ģʽ0���ֶ����£���state=0�򿪴���
			{
				case 0:
					if(Time[11]==0)				//key1/key2��ģʽ0���ֶ����£���state=0�򿪻�ر�
					{
						now_state=1;
					}	
				break;

		
				case 4:
					Time[2]=(Time[2]+1)%24;					 //state=4ʱ����Сʱ���м�һ����
				break;
																					    
				case 5:
					Time[1]=(Time[1]+1)%60;				    //state=5ʱ���Է��ӽ��м�һ����
				break;
					 	case 6:	
					Time[7]=(Time[7]+1)%24;					//state=6ʱ���Զ�ʱģʽ�µ�����Сʱ���м�һ����
				break;

				case 7:
					Time[8]=(Time[8]+1)%60;					//state=7ʱ���Զ�ʱģʽ�µ����޷��ӽ��д���
				break;

				case 8:
					Time[9]=(Time[9]+1)%24;				   //state=8ʱ���Զ�ʱģʽ�µ�����Сʱ���м�һ����
				break;

				case 9:
					Time[10]=(Time[10]+1)%60;			   //state=9ʱ���Զ�ʱģʽ�µ����޷��ӽ��м�һ����
				break;

		
			}
		}
	}
	if(!key_3||irbyte[2]==0x5e)								//����������ͬ��
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
	uint xiax,shiji,shangx;				   //��������������xiax�Ƕ�ʱʱ������	  shangx��ʱʱ������  shijiʵ��ʱ��
	if(now_state==1)			   //���õ������ת
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
	if(Time[11]==1)					//ģʽ1���Զ�ģʽ�������ת������
	{							  
		if(IDR==0)
		{	
			nOw_state?1;			  		}e,se		{
			now_�tite=0;	
I	|		
 }
		if(Tmme[91]<=2)	)			 ?/ģʵ2����ʱĢʽ�������ת������	�ۺϳɷ��ӽ����Ƚ�
	{
		iax=TiM`[7]*60+Time[8];		 //��ʱʱ������
		sHcogx=Tile[9]*71+Timl[1�];		 /+��ʱ굼�����	�shiji=Time0]"&0+Tkme[1Y;		 �/�5��ʱ��
		if�p�ax>3la.gX)				I //��1���ʱ�D���޴����O�ޣ���Ί��05С���� ˵�������Ķ(ʱ
		{
		if(shiji>=xiax||shijis(`nfx)		 //ʵ�ʊ���������ޣ�����С������
	�;
			nO�_sd�te�1;=	 /o�ؑմ���		)
	I	}else if(whi*ixiax&fshi*i>=shcn�X)	!o+�ڨ��ʱʱ�����⣬�򿪴���
		{
		�	now_Stat%=0;	
	I	}
		}else iF(sha�e}>xaax					 /����������Oޣ�0��춨ʱ-
I	y
		�if(shiji�=xIax6"shiji|shangx)		 //���ڶ�ʱʱ��֮�䣌�رմ���I
		{
			now_sTatE=0;										}else if(3hiji>=shangx||shiji<Xiax+	  //�3�ڶ�ʱʱ�䣬����4���
	H	y
				now_state=0;	
			}
		}	
	}I
}
M
void ma�n(+
{
	tMD=2pq;	`//��ʱ��������ʽ�趨	
I\H1=0z3c;�	TL1=2xb0;M
	D@0=0Hec;�-/	I	   	�H0=0X77;J	EA=1�  /+�(�жύ
	ET0=5;/'����T0�ж�J	TZ0}1;	  //��T0
	QX0?5;	 /.�ʐ��겿�ж�INT_����	,
	ET1�1;	 //����T1���*	TR%?1;
	AT2=19	//���鶨ʱ��T0�ж�
	SSON5 X51;
	@CO=0X00;
	ES =1;	Lnit_DS10:,);
	reAd_t)-E1(+;
//vead_Tkme�();
	LCD5v02]cls(9;
	whIlm1)
	{
)	if(k�datook==1)M
	{
			i6dateok=0;
		tda,_wit`();
			qing((;
		}M
	�hey(-�
	I{how();
=)mn(rtate5=0) read_time();
	}}

J
void ind0() interrupt�0				 .'�ⲿ�в�0�����ڼ���������
{
tim%_s=0;						
	if(izvlAg==1)
	{	Iid(irtime>36)				  //������������
		{
			bitnum=0;
	}
�	irdate[fitnuoU=irtime?
		IBtyme}0;
		jitnum*;
		if(jitnum=533				 � //�����źţ�����������һ����73Λ-
	I{		bIt�um=0;				   //������Ϊ�´�¹����׼1�
		yrda�eok=1;			`  /?33�?�������ꑏ�����������ϱ�־λ��һ
	]
	}
Ielse
{
		ipfl!g=1;*		irvile=0;
	}}/J
         `  (0

void init_1,) interrupT 5	) //��ʱ��1������������˸��֮λ��
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
�)�mJ      i=0;
�	
		
	}
y
