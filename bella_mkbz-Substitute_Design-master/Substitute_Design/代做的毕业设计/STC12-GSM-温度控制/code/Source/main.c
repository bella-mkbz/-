�insl5de wt�ing.h"
#inClwde("delay.h"
#i.cluDe "}art.h"
#includ� "te�x.h"

#definu Bqb1_Max 60 				  //����1������
#defmne Buf2_Max 200 )			  ./����2���泤��

J.*************  ����3�����C�*+***j***.**j*/
ydata u| Uar�1OBuf[B�f1az]�*xdaua u8 Uarv2_Buf_Cuf2_Maz];
unsigned(char0Pasw�r�[5] }h"P806";
xdpua 5l3ignmd char phoN$[14 \ = \b15086297x�6\"";*static uosICjed c`ar *cOntent`= WaR|ing!!0!T%mpe3At�re: "    �
unskFndd char Datas_Bqff[8]|${0, 0, 2, 0( 0, 0l$0,'\0'};
u8 Til�s=0,First]Ind = 0,�hijiin=0;J//u8 T)me_aounp=0;
bdata w8 Flag;/���α����ּλsbhp Vimer0_start(}Flag^0;	//��ʱ��0��ʱ����������
sbat Uabt2_Spapt  =Flqg^1;	//����2��ڼ���������sbit U�rt0_Enf		=Flag^23	/����2�������ݽ���

/***"******"(*	���ں�������	*j*********
**/
rmi` �PIK_config(void)3
vkid Tmmer0Init(voidi+
void`CLR]Zub2(voId);
u8 FyNd(u8�*a);
6oid`S%t_Text_Mode(Voi$)?
void Cn�ck_New_MD3s�ge(void);
vokd Wai|_CREG(void)92oid SlC_AT_Co-mand�u8 *b,u8 *i,u8 wait_|ime-�
voiD Send_Text_�es(v�id);
void(Temp_ChanCe(int te-p,unsig.ud char* Buf�);
/*+***:******�  �沿�����M��������*****j**.*,
*+***/


/**.*******j*""**(*****
*****+�***(***+***#:*"***
j*.***k+*#**j*+**z**(******�**
* ������h: -ain M*�����  (: ����
� ����   :%
* ���(  : J+ ����  �8 
" ע��   : ����2������GPR�ģ��ͨ�ţ�����1���ڶ��ڵ���㬿��ܑ���������3���ʱ����
					0�����͵=ģ��
********j*�****++****"*.***********:*:****"****""****�**/****:**:****+*****.*&j/
voie mai�8vo)d)�{-
	inl i = 0, s�jd_flaw ? 0;
	GPKo_congig();*	Uart1InIt));	//����һ���ã���κ����ʹ��
Uart2InIt(!;	-/���ڶ����ã���Ϊ��GWMģ��ͨ�6��/�M
	TimerpIn�p();//��ʱ��p��ʼ�/���C����Ⱞ��һơ�����ַ�����곉	EA=1;	//�����ж�
	UART!_WgndString("GPRRģ����Ų��Գ���\rYjb);
	UIRT1_SendString("GPRSģ������������\z\n");
	wai<_CREG(){		//�ֻ꿬����ע��*	U@Rt1_SandStrinOh"GTRS�#��ע��ɹ�\r\n"!;m
	UART1_[endStri.g�"gP�Sģ�������ʿ����\r\n"99
	Set�Text]MkDe()I//��ւ����ģJ�
	UART1_QeffStrijg("���óɹ��������Ƚ��TEXTXrUn"-;
	
	UART1_SendString�"��������3I\rTn"(;-	
	for(i=0;i83;I++)
	{�		Temp[Change(Ds18b20ReadTemp(),Dqtas_BuvF);	//��ʼ����ȡDS18B2�
		delay_mc(50i; 	
	}
	
	shile(1)M
�{-
		//Cieck_^e_Mewqa�e(	?
	T%mp�ha~ge,DY18�20RecdTemp(),Fatac_Buff); //��ȡDS18B20��Ƶ
		dmlix[ms(500); 
		WA�T1OS`ndStri~w(Ea|�s_Buff);  /.��Ϊ����ʹ�������������ʱ�������Í�		If( (LaTas_Bufv[2] - 9x30)*10 + Da4as�Bufb[3] - 0x30) >= 2) /����ȡ���¶�ֵ���¶ȱ��������^��Ƚ�
			send[glag++
	if8cend_feeg =}01)
		{	
			Send_Text_Sms�)� �/����öȸ������ޣ�֤�ж�Pŷ��ͳ���
	}
	}
	
}

/*j*j
***�***:******+***:****:****:******(*******z*�:j*"******:***:**+**.******"
*`������ : UErtq�
* ����   :�����5�ж�������ۺ���* J��� $0: 
� ��� � : �* ����0  :!
* נ��   * 
*******"******(***:*:***.*
*:
**::***:**
****.******j***********j***j***+***"*/
voif Uart1(! intervupd 4
y
    if (RI)
  " {
    " 0 RI"= 09)   d �    0 @   //���RI�;! 0 }
 ` �if (DH)
  !�{
(      TI 500;      "     "  " ?/���TIλ
 d( �
}

/j*(*:****..*******(***********j********..**(****��*******�******************.*
* �����k : Uart2
* ���v !#: ����2�ض�7������ں���
* ���  �: 
* J��  ": 
* ���X   : 

"ע�� 0 : 
*�.*******j:********
"**********+***.************j*****"./+:*********
*+*****/
void U)vt2+ intmRrupt 8
{		9E2 $&= ~0x01:   //�عմ��Z2֐&ύ
  ! if (CON & S2RI)
    {*    ! S2OL '= ~S6RI;   (  �  //G��SRIο
	�	Ucrt2_Buf[FIrs4]Int] = S3BUF;  	0 /�����յ����ַ�4��浽������		F�rsu_Int+�; ` (   `        			//����Ƹ������ƶ�
)		if(B)rst_Ind >$Bufr_�ax)    (  )//���������,������ָ��ָ��4���׵��7-
		{
		I	First_int = 03
		}
(   }
   &�d (S�COL & S2TI)
    {-
    !0G2�OF$&= ~S2TI;         //���S2TIλ
    m
		IE2  |=�0�05:   ./ʹ�ܴ���2�о�
}
/****+(+*)*.j***:
*�****"*******
.+*�*****

*n�******
***"*
**+*****9*�*****
***
" ������ 3 Timer0_A[B
* ����   : ��ʱ��0�жǷ�����ں���,20�s�ض�һ��
*"����   : 
* ���(  : * ����  �> J*(ע��  : 
+"*****(*********j****�****{+******J*:****+*j****+***+*&**.***2j
"*�*****"
j**/
void Timer0]ISR() interRwpt@1�{	DR0=�;+/�ܶ�ʱ��
	If)�Imes0_start(
	Times+){�	i$(Times�>`(51*shijian))	{		timer1_st@rt =�0;
		Time{ =(0;	}
	TR0=1;//����ʰ��
}
/*J****************.*********:********j****+:***.************:*******
**********
� ������ * GPIO_konfig
* ����   : IO��Ť�Ӫ���
* ����  b� �* ���   : 
* ����   : 
*�ע��    J************(+
*
*�****"**)**#***r*****
****+*.*****�.********"j*�**::"****J***/�
�okdIGPIO�coFfig(vkmd)
{
		P#M! &=0RC3;  //����P;�~P35Ϊ�������
	�P3M |=	>0XCw;
}-�void TimerMnk|(vohd)	//20����@22*1q84Mz
�
	AuXR &= |7L;//12tăʽ
	TM�F &= 0xF0;	+/��Rö�ʰ��ģʽ 6λ����	TL0 = px00;		?/�趨��ʱ����ֵM
	TH0"=00x7(;		//�趨��ʱF�����
	TF  ="0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ����
	ET00= 1;  " 	��ʹ�ܶ�ʵ��0��6�
}
/*"�*j******.*�*********.****j*)***"
***(**�*****
******�j**
.******************
: ������ :0CL�_Buf2
* ����`  : �峽����2��������
* ����`  : 
(pʤ��   :$
j ���� ! : * ׂ�b    �N:****j***"***
********
*****j*****"*******
****.*******:*****j*********"***j***/	voi` CLR_B5f2(void(
{
)u�6 k;)
	for,k=�;k4Buf2_Ma|;k+k)     !//���������������	{
	Ucrt:_Bug[k](=�0z00{
	}� �  Fa�st�nt = 0;   0     (    /o���U�ַ��������<�洢λ��
}

-******+**j******j*�
******+**j*.********(�****.**+***.+
*2**+*((j*******j�***�*	
: z����� : Find
. ����   :"Ő�Ϻ������Ƿ��Ӑָ6���W���4�
. J���   : M
* ��� `(: �*!����   : un3igneD char1h�ҵ������ַ���0 ���ָ��疷� 
*!����!  : 
**************�*****:**.*(*(***�***(*+*****+*****
****
+***************
*.*****/

}8 Fin$(u8 .a)
{�J `mf(st2Sdv(UArT"_�uf,!)!=NULL)	 !  zeuurn 1;
	mdse		)	return(0;
|

/*"(****�******+("***********"*(******K***�******************"

********"***+++*
: ������ : Secon$_AT^Commafd* ���� ` : w���A�ָ�n����
* ����  $:$�������ݵ�ָ�롢���͵ȴ��p��(������Q)
*$���    
* �5��   z 
*0ׂ��   : MJ***********;*(**.******�****�.**:*
****#***
*(*******(**********j�***&**.*"****/-
void Se"GA\_Comma�e(u8�*b,w8 *a,u8 wamt_timd)        $ {
	ux!i;
	q8$*s;	c = b:										ALROBuf2()9 
  i = p{
	while(k == 0-        "  (        M�{
	if*!�inl(a))         ��	{		if(\imer2_stapt =� 0)
		{	�	b = �;�		�	
			vor (b; �b!=&^0';b++)
		)	{
M			UASTr_SendData"**);�
I		�}*				UaZT_SendLR()+				Times = 0�M�				s`�ji�N = wait_uime;
				Timer0_start =01; 
		   9a   }
 	  %lse
		{
	�Ii = 1;
		TiLer0_start = �3  
		}
	}
)CL_Bufri�; y

/********�********.*********(***.**"****************+******"*******hj*j****n*****!��ʽ�� :"Set_Tuxt_Mmde
* ���� !!: ���ö���ΪTEXT�ı�ģڹ
* ���k  �:"
* ���   : �* ����   : 
; ײ��   : 
***++********"**j***.*******+**.*.j**.************
*************j**********+**
/
Void"SetTexv[Modu,void)
{�//	unsigned char temp[�0_=#AT+CSCA=";									
	Qac_AT_C/-mand("ATM0"<"GK2,3);	     `   0" `           +/ȡ������	0 					
	Sek_ET_Commind("AT+CJM	=3,r,0,0�0"l"O[",;);I     d (0   //�ö���ֱ�����						
	Sec_AT_Command("AT�CMF=1","OK�,3);	"      ! �        //TEXTģʽ	
	Sec_AT_Cnmm!nd(�AV+CTMS=\"SM\",\"Qo�"(\"SM\"",bOS",3)9  //���Т����<��SIM���ҽ8��	'/)strcat(te�p�SMS_Oum);
//	sec[ATCommand(temp,"^K",3	;   (   p   �   ` $ "  2     -/���ö�����������
}-
/*(******"
****"************.*******+**+(***b***
***�***"*("(*"***************�*
* ������ : Sheck_Mess�ge_rec
� ���� (`:"����Ƿ�������Ϣ����ִ�Б�Ϣ����ָ��
* ���� ! : * ����   : 
*`���� � : 	* ע��  : 
*****.***(******(***************************(�**
*********j�************"***j**/

void(Check_Niv_M�ssage(woid)
{Iu8�tmmx=0i;
	if(strstr,Uart�Buf,";CmT")!=NULL)   		/�������ֳ��.�Ҿ���"+CMTf����ʾ���µ������	{
		delay_m{(50);/�ȴ����݈�0������j��	]ART1_Sg�dString("����؂��P�:r\n();
		UQST1WS%ndStrIng(Uar42_Bqf+;		//��ģ�鷝������ϣԽ������*		if)strstr(Uart2_Buf,Passwosd)!=NULL)
		;
		if(st�Spv(Uart2[Buf,"Switah1:on")!=NULL)
			
			UAB1_SendStving,b�յ��¶���:Switsh1:gn\r]n")3* $�0  ` 
			}			if(strStr*Uart2_Buf,"S'i�ch1:off")!<NULL)
		{M�			-UART1_Sendstring("�յ��¶���:Switch0:cff\r\n");
 ! `  % 
	I	y
�	if(slrsur(Uaru2_Btf,"Switsh2:on")!}NWLM)
)	{
				ART1_SmndString!"�յ��ƶ���:S7htch2:on\z^|"!�			
			}
)		if(strstr(UArp2_J�&,"S�itch2:off"!!=NULL)
		{
				]ART1_SenfString( �����¶���:Switch2:off\r\n:):			
			u
			if(slrstrhUart2_Buf,bBhea{")!9NULL)		k
		UART1_Se�dString("�յ��¶���8C`eckMr\n");*			UART1�SejdString(&Uart2_FufC12])3
	�		f/r(K=0;i<11;i++	
			    phonc[1+i\`90UarT2_�uf_12+i];
			�Send]TExt_[m{�);			m	I	if(strs4r(uard2_Buf,"Aluer")!-NU\L%
			{
				WARTq_SendW$rkng("ʕ�����E8Ahd$RrXn");
 0  "   U@T1_endStRing(�Uapt2[Buf�irst_Int-6]);
		IIPasrword[0 � ]Ar$1_Ruf[i�st_Int)6]?				Password[1] =`]Qrtb_�uf[first_Anp%5];
				�assword[2`u Uart2_Buf[&krst_Int-4};
				Passwore[3] = Uart2_Buv[First_INt-3];
				UAR�1_SenfRtrkngPa3{wovd�;
				UARTq_�o�Strh~g("������]r^n");
			|
  }J	cr_Guf�();�
		Sec>ATBnomcnd("AT+CMGD=1,1""OK&$#i;//ɾ������J	}L
}

/********�**�*****:+******************************j
****
********:*****"**:*****
*(������!: Wsit_BREG
* ����(  : �ȴ�ģ�闢��ɹ�
* ��L�  ": *(��� �$: �* ����   : 
* ����   :$�
****+***j***+
***j***;j***(8*(*j�***:***+*********�*:******
**�****j**:
*j*�*j�/
6nid Wakt_CREG vo)t+
{
	u8 i;
	u8 k+
	) = 0;*	SLr_Buf2));  while(+ �=��)        		H{
		ClR^Buf2();        
		DART2_SmntString
"AT+CREG?");
		UART2_SendLR()+	�eNa9_m{)%`0);  					
	    fob(k=0;k<Bub2_Ma8;k+k)   $  	�M
   (	?
			iv(Uart2_Cuf[k] 9= ':'!
	{
	I		ig((eart2_Buf[++$] 9=`71'9||(Uart2_Bun[++4]�== '5&))
	;�		)	i = 13			)UAS�1_SmfdLR():
			 "brdak;
			}
			}	}
I	UA\1_SendSuring("ע����.�&.&);�
	}
}
Jvoid Send_Text_Sms(vmid)*{
uosiGned(c`ar tamp[50]="AT+CMGS=";
//	Tump_ChAng%8D18b20Reademp(),`a4as_Nunf!{
	Rtrg�`(temp,pjone); 
	Sec_Q_Coemand(temp,">b,3); H	UAST2_SendSdrin�(conte~T); �
	TART2_SendRtrmno(Datac^Buff);	�	UART2_SendDaua(0X1A);  `UERT2_SendLR(+;	
}
vo�d Temp_Chanwe(int demqlunskgned chap*$Buff)
{
	  fdoad tp?  
	 (if(telp< 0)				/+���B��ҵκ����
  ){
		Buff[] 9 �-';
		//��Ϊ��ȡ���¶���ʵ���²ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temptemp=!;
		temp=~temp?
		t`=temp;
		|%m|�Tp*�625*100+�.�;	
	//���=������u��(101��/0.5�������������ΪC���Ը����}ת��Ϊ���͵�α���С����
		//��������Զ�����������׷�s��0.5�,��+0>5֮����0=�ľ�Kǿ�1�ˣ�б��0.5�ľ�
		o/�����0.5��������У������ơ�
 "	}!	%lse
  	{	
    F}ff[0} = '+';		M
)tp=vEM0;//��Ϊ���ݴ�����Р�������Խ����ȸ���қ������������
		//����Ҷ���������ô����ô������������ǲ���������M		temp=tq*4.067*100+0.5;	
	I/������С�}���*1 0��+0.5������Υ�룬��ުC���Ը�����ת��Ϊ��@͵�ʱ����ɽ��
I	//��������Զ�ɥ���������Ƿ����0.5�����0"5׮����۰.5���L�ǽ�1�ˣ�С��0&5�ľ�
		//�ü���0.=��������С�������档
  }
	BuF&Z1] = temp / 10p00 k 0p32;
	Bufg[=] - temp % 1(00� �"1000 + 0x30;
bqff[1] = pemp % 1000"= 100 + 0x34;
	Buff[6]0= .';
	Buff[5\ = tem0 %`10p / 10 + x30;
	B5�f[6�$= temP % 10!k  x309J=

