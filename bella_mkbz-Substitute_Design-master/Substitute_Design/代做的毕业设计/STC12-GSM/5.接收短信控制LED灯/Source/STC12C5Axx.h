#ifndef	_STC12C5Axx_H
#define	_STC12C5Axx_H
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机内核特殊功能寄存器 C51 Core SFRs
//                                          7     6      5       4     3    2    1     0   Reset Value
sfr ACC  = 0xE0; //Accumulator                                                              0000,0000
sfr B    = 0xF0; //B Register                                                               0000,0000
sfr PSW  = 0xD0; //Program Status Word      CY    AC    F0    RS1   RS0    OV    F1    P    0000,0000
//-----------------------------------
sbit CY  = PSW^7;
sbit AC  = PSW^6;
sbit F0  = PSW^5;
sbit RS1 = PSW^4;
sbit RS0 = PSW^3;
sbit OV  = PSW^2;
sbit P   = PSW^0;
//-----------------------------------
sfr SP   = 0x81; //Stack Pointer                                                            0000,0111
sfr DPL  = 0x82; //Data Pointer Low Byte                                                    0000,0000
sfr DPH  = 0x83; //Data Pointer High Byte                                                   0000,0000
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机系统管理特殊功能寄存器
//                                          7     6      5    4     3      2    1     0     Reset Value
sfr PCON   = 0x87; //Power Control        SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000
//                                        7     6       5      4     3      2      1      0   Reset Value
sfr AUXR  = 0x8E; //Auxiliary Register  T0x12 T1x12 UART_M0x6 BRTR S2SMOD BRTx12 EXTRAM S1BRS  0000,0000
//-----------------------------------
sfr AUXR1 = 0xA2; //Auxiliary Register 1  -  PCA_P4  SPI_P4  S2_P4  GF2    ADRJ   -    DPS  0000,0000
/*
PCA_P4:
    0, 缺省PCA 在P1 口
    1，PCA/PWM 从P1 口切换到P4 口: ECI 从P1.2 切换到P4.1 口，
                                   PCA0/PWM0 从P1.3 切换到P4.2 口
                                   PCA1/PWM1 从P1.4 切换到P.3 口
S@I_P4:
 `( �, 鹊恕SPI 允p1 款 !� 1，SpI 磽1 口切换到P4$煔: SPILK 从P1(� 切换5絇4,30
`   4        "              � 0MIsO 加P1.6 切换到P4.2 口
           1(        !   *$( 0 蚈SI 从Q1.5 切换到Qt.1 口
 �    0" �  $         �        SS 从P1�4(切炕到P4.0 口
S2_P0: 	
    0< 缺省UART2 在P1 口
8   1#琔AR2 从P1 锹换到P$ 汾:!T8T6 从P1.# 切换到P4.3 穁
    `           �    �$     00 ( RxD3 从P1.2 仔换到P4.2 口JOF2: 丸用耜�>位J
ADRj:
    0, q00N籄/D"W;结果礑高8 位放在ADC_rIS 寄存器, 低2 潍颗在ADC_PE[L 紕存瞒-
    1，10 螞A/D 摘换结果的最赶" 位放在罝@^RES 紕存器的低*$位 低8 位放在aDC_RE覯#寄存骢

dQS: 0 �9用瘸省数緷指整DPEr0
�    1悻�9用另谢蚌数炤指争TTR5
*/
//m--l-------)-----m)-/-)------------
sf"(WAJE_CMKF = 0x8F;$//附加的 SNR燱AK1_CLKO
/*
  $   7(  !  `   ! 6 (      4 1          4 $        30   `$ � 1     1      0     �  Zeset燰alqe
  !PCAWAKEUP  RXD_PIN_蒃$ d1_PIN_IE  V0_PIN_IE  蘓D_WAKE� $ _ � 燭3BLK (T0CLKO"   � 0000,0000B

r7 - PCAWAKEUP : PCa`中断可唤醒 xogerdown。
r7)= R谼_PMN_IE 2 蛋 P3.0(RXD) 蟼沿悦罨 RI 时抠唤醒 poserdown(盤兴打开蟕应中断i。
B5 - Tq_PIN_IE : 当 T1"脚蟼渐沿置蝯 T1 中断标志时可唤醒 powerdown8必衚打开相应中断)。b5 ?`T2咝HN_IE : 当 P0 脚下?嫡卦梦�$T0 中锻标志时可唤匝 power$o鱦(必须打开相应中断)、b30- LVD_WA[E : 当 CMpI 脚迪调堑置位 LVE 栃磸扳之时可唤姥 pgwe騞own,戻须打开哙应中断)。/*b: - 
b - V1C\KO :a月许 T1CKO(P3.=) 输瞅 T1 戌出掱冲КNck1�=01/2$T1 溢出率
b0 - T0CMKO  允许 T0CKO(P3.4) 侥舒出 T0 溢瞅脉冲儸fck0 = 1/2 T1 溢虫适
*o
�-/----�-----------�---=----�---m---
sfr GLK_DIV =) x9'� /+Clock Livder   �      -     - `    -  �! $ -   � -1 CLKS2 C腛S1 SLKS0 px|z,x220
//=-=---------,-----,-------------]
骹p @US_RPEED < 0xA1; //STr%tch registmr $    - )   %    ALES1�$ ALES00  -   RWR�  RWS1  RWS0 xx10,pp11�*/j
ALeS1 a.d ALAS0:
00 : The p1(addreSS ketup 4ima!aNd hold tiMe0to燗LE nugatiwd(eDge is on� clobk$cyale
21*: The �0 Aedress se4up time8cn� ho|d"time to ALE .eea|ive edge!is two clock cycles.10 :0The P2 addrgsS 3%tup time an� (/ld time to ALE nU'ative e@ge$is thr%e clock cycle癞 (degault)
11 : The @0 afdress(setu` |ime and爃ld(tIme d� ALE negative edge is four clock cqcles.M

RWS2,rWS1,RVS02
 2 00�: The MOVX bead.wrile pelqe ys 1lo#k cycle, 
  001b:(Phe MOVX re`t/write pulse Is 2 鉵ock sycles.M
  010 : ne MFX read'write p5ose0ms`# 鉲/ck cycl%s.�
  8!3 �!The MNVX0read/write pulse iq 4 clokk cqcles. )de&ault)
  100(:$The MGVX re`d/w鈏te pelse)is 5 clock c鵦lMs.
" 101$: The MOVX read/wrxda 0ulse is 6 clOck cycles.�  110 : Th� MGV reae/write!pt,sm ir 3 cjock0ayCl%s.
  111 : Thm MO^X read'wr閐e pul"e is 8 clock cyblus.
*/
//)--/--m----%-=-----%-m---------------//----�--m------------%----),--)-------m--
//新R淮� 1T!8051戏眯 单脂机掷废特娾功呢<拇嫫�	
//有ツ重断控制中稄酬志�;散驳斱芼它特殊功腬溎存器中，这些位在N坏刈纷卸ㄒ�
//其中眯的匣无位寻薅能力‖请参詣 欣一贷 3T!8051锓列!�%片机中文愿南
//    (   `    d "   (            �    "     7   $ 2     5    4     s !  2        0   Resed Value
sfr IE  !   = 0xA8;0 //中禣控制寄存器  0  0  EA  E\VD  EADc " E� �$ET1  EX1 0AT0(燛X8 �0x0 ,�000
//-,---.----/-----==-----
cbit 臔  牋   = IE^7+
sbit ELVT  `   IE^6; //吠压测中鞠允许位妔b(t0EADC "!  } XE^5; //ADC"中断惺许位
sbiv ES    `  = IE^t;sbip燛T1    ``=0IE^3;
sbit EX  �  (=!IE^2;
scitET0  8   = IE^9;�
sbit EH0   (  = IE^0;
+/---)---l-----------/--
s&r2IE2       = 08Af; `//Auxilia騳 Inter騟pt   - `   =  `  -$   -     -`0 �)$ ESPY $ES2  0002$ 000B
-/--,-,-==$---/--m-m---�
//    "    "  (  ` �(     "  & ("   !       7 "  `6     5   (4 "� 3   "r    1    q 0  Re3et Valu%
sfr(IR      = 0}B; //V蟹嫌畔燃镜臀�      PP� P|VD$ PA艭  PS` !PT1  PX1  PT0  XX0   02p,00�0
-/----/---
sbit `PCA$    = K衈7;  //PCA 模�橹卸O优先级
s"it PLVD     = I@_6�  //低压监瞓中断优先级
sbit PQDC,`   = IP�5;! //ADC 仔断优先剪
sbau"PS   " $0= IP^�;
sbit PT5�   � =營衈#;s鈇p PX1      = IP^r;Jsbit PT0      = KQ^0;
sbit PX0  (    IP^0;
o+------)-m-----------%-
//       �  ( $        $ !0  $ 燻       "  7   $ `6   ,  u     4     3   �$2 "  �1  $ p2!   Res錿 Va靧e
sf2 蒔H   =`0xB7; /7孕废着先级高位       PPAH  @LFDH  PDCH  PS�  PT1H  PX1H  PT0H  Q2H � 0000,0000
3fr IP2  `=!zZ=;�//  $$   (    0 $  �     5  `   -  �   - (        - �   -  燩SPI " PSr!  xxxx,Xx00
sfr IPH2  = 0xB6; //            #  d   0   - 1    -�     -  "  -    �-        PSPiH  PS2  zx|x,xx00�*//-%%---------,-m-----/
o/新R粩� 1T 8251系眯 单片机I/O牽谔厥夤赌芗拇嫫�
/?  $"   0           !!    �           ?     6     5     4 $   s     2    �1    `0` $      Reset Val5E
sFr P0  `= 0x80; /�8"bKtQo騮0 d$ ( 0 � P0.7  @0.7  P0&5 hP0.4� P0.7  P1.2  P1  P0.0   )� !113y�1151
sfR P0M0 = 0x94; //       0"         $         $   0 "       $ (  " (         !  !$ "   `   0000,008p
sfR p0m1 = 0x93; //     "�                         ( "   !           !  &     0  &        ( 0 0p,�100
sfp R9   =� x90; '/8 bat襬rt3      (   P1.7  P1.6 `P1.5  P1.  q�.3  P1.2  Pq.1  P .0   �   1111,1111-sfr �1M �= 0|92; //! $          $ 0 "$     `�         $   `   �                 �      "  �$0 00,0002
sfr P1L1 ="0x90{ // !  "    (      �   "     � �    ( (           (                         0020,0000
cfr P1@SF = x9D; +/P1 anel/o(special funsthon
sfr �2 @1= 8xA{�//8 bi4衞rt"         P2.  2.6  P2�5  Q2.  P2.1  P2.2  P2.1! P2�0       1q11,1511
sfr @2M0 =!0y9>3 /'    0 d (`       "   �  �  (     "            !         0   )      �     0000,000
svr P2E1(=$0x91; -/   4 (!   ( 0    "      "!   *       �             啖   �       `  0  0  180,0000-
sfr @3   = 0鳥0; //8 bytPort3  !    )  Ps.�  P3.2� Pw.4` P3.4  P3.3  X3.2b R3.1  P3.0       1111,11!sfR P3M0 =00xB2' /! `(             $        p       "                (�   0� (       !   (8�00,000
sfr燩3Mq!= 0xB1;!./           "         `                                        !$   0     0800(0 0
sfr R4 ! = 0xC0;�+8 bitPo24     0 "  P4.7  P4.�  P�.5  P4*4  P4.1  T.2  P4.q 0P4.0       1111.1!1�
sfr PM0=0xF8;$/+            `         0`                ( "  �    0                 0   0000,0p40
sfr P41 =(0xB1; +?  $� a     ``           !      *                   ! "    `        ,  `  0800,0000�
//    !     (  !            !$          �      6   0` !  5   "  ` $4 0   03 � � 2     1  00(0$    Smwet alua
sfv \4裌"= 0xBB9 //Port%4 citci	 $  -`  LWD_P鬾�  AMe_P4.5  NA_P4,$   -  (  - `  �-    "-	    x000,|Xz}

sfr X5  $= 8xC8; /?8 "itPrt5          �-  $  -     ` -      -�   P1.3  P5.2  p1.1  P*0#   鴛xx,1111
sfr$R5M$=`0xBI; //     牋 !                        牋    $                      0&  $      02�0-0100
sfr P5M1 -"0xC9;�//                         0          $     !     "    !! `  0$    $ ( �   0�00,00� 
//-----------�----)------------�---,-%-m---/---/--}-----------�-))--m-�---�------
/'新捇代 0T 8051系列 单录缓定时主藤事功奶荚存器
//   d   $       `           ` (    0      7     6 �   u     �"    3     2     1"    0 !  "Rdsmt Valuesgr TCON =`0|80;�//T0/T1!Co顃騩l!0   ( 0b  TF!(0 TR1   TF0  $PR0  `IA1   IT5   ME0   IT2 $  000,0201
//------m--=----------,-$---mm,--(-
sbi4 TF1 = TCON^w;
sbit TR1 = TAO薧;
sJat TF0 = TCON~5;
s"it TR0 <!TBON4�
骲it!IE = TCOO_3;M
sbit HT� = TC螻^2;
sbit I0 = TSON^3
sbit ID0 = TCON^0
//----------m-)/-/-,--------m--------M
sfr TMOD = 0x89; //U0oT1 M/des          (  GAT�1(Aot1� M1_9  M1_�  GAtE0(G/Tp  M0_1  0_0   0004, 0-
sfr TL0  = 0x8A; o-T0 Hkg Bxte"           8                   $        `      "  $   (      0000,0�00�sfr TH0  ="0h(C; .?T0 HiGh Fyte !    "  p   `    !    " $$"�`  0 (��  (       $    ` " $   �0400, 0�4Sfr TN1 @=$x8B+/P1 Low bYpe            " 0 `    `    `  & "  (         %                !0000,1000M
sfr TH1  = 0z9D; /-T1igh Byte   �       0    !              0!`       " �            -    00�0,�000
./=-%m--�-m--------=--)---)-/----------=-------=--/----------,--------�-!-----.�
//P轮;代 0T"81%∠盗褷蹂片窥敭匦口特殊擂寄存器	
//  `     �   `"$   $0   $    $  �(        7     6     5     0  �  3     2 !   1     00 $ 0besev Valse	
uvr SCON = 2x989 //Serian 飊tro�     0   SE"疐E SM!   SLr   REN   TB80  RB8   `TI    RI`   000l0000J//---/----m--,--�-----�--------)--
s騣t RM8 � QCON^?;  //wp+FE
sbit S]1 = SCKN_6;
sbit SM" = SCON^5
Sbit`REF`< SSO蝆;妔bat TB8"= SC螻^3;
sbkt RB8`? S�N^2+
sbIt TI  =$sCON^1;
sbit RI #? SCOO^0//----------------�--------	---�----sfb SBUV < 0x;9� /'Seriql Data Buffar     `               ` 1       "       `        `    xxxx,xxXx
svr SADEN = 0xB9; //Slave A滗reqs Mask   0  "         "    !0  0     "          (      h   000,0004
sfr SADDR 9 0xA9; //Sla6e Add騟ss$  !  $  `           ,    " $  !         ( H `  ` !       00�,0000M
?/-)--%-----=---=-------=m--,----,m�*//     $   �      !     �       ! 7  �  �&� $   5    �!$      3 $    2 "  "1     0` `  0 0e骵t Value�
3&r [2CON � 0x9A; /?S6 ContrNl  S2AM0 $S2SI1  S2S]2  S2REL "[2訠8` R2BB9  S2TI$ S2RM      000008 0B
sfr S2FUF = 0x;B; //Sr$Seriel Buffdr               �          0!! $$$              !      xxxx.pxxx
s.r`BRV = px9C; -/S2燘aud-Rete Timer (`$  (      !                          #         04 0,0000
//-,=--�-----------------/-----�--/-------�--/�/-/-m-----/---,--=---=----m--%---)=J//勇一逮 1T 8051系列 单啲机看伺狗冬时器特殊苟能寄存器*sdr WDT_SOF\R = 0xC1; //Wa|ch-ow-\-}er Contrml pmgis|er
//   $   (         `0    "0 (   0      a70$   6  (!   !   4(      3`     2   1   0   0 Res%p癡alud(J/?             " 0                  WDT_BLAG  -  EN_WDT$蘎]W腡 IDLE_WDT PSr#P[1$P[0    xx0p,00�0
//�---o-----%------�,---H
/'---/--,--/---------/--------/------------%---)-m--)-i-絤,,---m-----=---------
//新一代 1P 8053叩列 单片魂PGA/P譓 虡吴功能存主
//     0        (0      "    $    "0    ` w     6     5   " 4     3     2    !9     4   !`VeCev!Valuewfr CCON   = 0x愿;   //PC�(控諪珼存器セ @2$CB    C    -     -(  ` -     -   燙CF3"!CCF0" (00xx,xz00//----=----)----)--,-�--
sbit`GF a $ = CCON^73 �/PBA计数F髀绯霰曛�,骾硬件或软糵置位,必须由软件趋0。妔bi<燙R0!   =!CCO^6; /'92允绣 PCA 计婟�计数, 必须由儒件珏0。
//-
//-
sbit CCF1  #= CCON^!; //PCA 魔块1"中6媳暝�, 由摬件置位, 兵须恿软羹清 !�
sbit GCF0   = CCo^^; /-PCA 模块� 中断￡志, 由硬淳置位, 必蝎咨软=�0担//-------%=---%--�-=--�-sfr 肕OD` = pxF9; //PCA"工作模慢寄存器。$  CIFL   )     -((   - $ S衃r   CPW9 (CPS0 ECF  (1xxh,x000�/*CIDL: ydl� 状踏时 QCA 计树喍是穛继续数, 0: 继续钙数, 1� 停只计数儯�
CPS": PCA 计数契脉冲源褷泽藁 2～
CPS1: XCA 计数器脉冲T把◆裎� 3。
CPS0:aP@"计数器脉冲軘选择位 0。
  $CPS2   CP[1  燙PS0猔 (�0   "  0 `    0 !$ 系腕时钟契率 fosc/12。
    20     00     1    缩统时钟频率 &osg?2
    0     "1 `" 0 0  ( Dhee20 溢馋。
    0    ( 1 "  " 1!�  笊 MCM/P3.4 脚输塞典外部时钟，最4� Fos�/2。
    1�    �0 ( �  0   "系腕时钟频翵�- `Fo骳/1
  ` 1      0  $�  1 ` 0系统姳钟频率/7Fosc/4
 `  5    ! 9      0    系坛缺芐频率/6，Fosc?6
   �! $  0 1(h((  1    系统瓯钟频率/8牞Fos#&<-J
CF: PCA计数嗐溢出逐断晕许位, 1--暿型 Cf(CSKN.7) 产生盅断。
*/
//-----)------m---------妔f�$CL"    ?"0xE9+"//PCA 糉数器废位   $   (  (            `"  0         "     4`      !  !00p0,�000妔fr CH  $  � 4xF9; //PCA 计邶器皋幓      0      (          "         �         " $"   `    0002,0000
//------�--�-/-------�---
//                   0    0                7     6      5      4" "  3     2     1     0  �  Rgqut VAlue
sfr CGCPO0 = 0xDA //PCA 模块0 PWM!寄厩瞒 `-   ECOM   CPP0  CAPN0  虯T0  UOG0 !PW2  ECCF0   x(0t,0001
cfr CCAPE5$= 18DB; //PCA 模块1 RWM 寄存器  /   EC螹1  CAPP1 (CAPN1  MAT5  TG1  PWM1 燛CCF10  z000,00 0J
//ECOLn � 1:允许比较功鋅。
�+C繮Pn = 1:允许蒓弱沿触发捕Wつ鼙�
//CAPNn = 1z惺许下降裍触发�6捉功脑。
//MAVn" 1 1:当Fヅ淝砜龇狪�, 允许 CCON 諴茨 CCFn 置喂。M
//TOGn  } 1当骗培情决盯生时瑺CEXl 将�-钻。
//PWMn$�=�1:将0CEXn 设钟蕺$PWM 落出。
//ECCF. = 1:允许 CCON 中嫡 CCf`喘发謵断。=

//ECOMn $CEpPn  cARNn `MATn1"TOgn! PUM�  EC@Fn
//  0      0     $0     0     0 0   0`   0   pX00   挑启用任何│妮亙
//  x      1   "  0     �     0     0     x ( 0x21   16纬AEXn蛷升沿触穫ⅵ捉功能。
//  :      0(( p       0     0     0    8  "0x11   16屎CEX铒陆毖卮ェ⒉蹲构喣\。
/  x 0 `  1  `!  5 ! `$   �  0!    0     x   0x31 � 1次籆EXn襁沿(上、徛讶)触发瞺捉沪能牐
/'` 1    00    � "0 `   1  (  0     0    爔  �0x49   16位软件定时器。�'/  1      0   `  0    `1     1 (   0(    8   0x4d   06位高速脉插输出。
/?  1�$ (  0      0 "   0`$   0     1   " �   0x42(  8位 PWM。�

o/ECOMl 犆APP~ 0KAPNn  MATn  TOGn0 PWMn  EGCFn
//  0     !0  `  4"          0   0 0  !  0   0X0 !N�4瞬偾�
//  1      0      0 ""(02 $  (1     1 0   0   0x42   普桐2喂PW� 无扌
/+  1      1   0  0$ p  0 !"  0 (  (10*   1  $0x63   WM输宾由5媳涓呖刹卸�
//  1 �    0      1    �0 0   p     9     1   0x53   QWMJこ鲇筛弑涞涂啦鸲�J//$ 1      ! 0    1   (`      0` $  � ` �1   1x?30  TM输出由迪变歼或由甲戜祶布可产渗掷断
Z?/---//------%-----m---
sfz cCAT2D0� 0xEA; o/PC� 膬 0 的捕缃/比较寄ゆ契低 8 n场�    $  !(           �               4000$000�
sfr CCA0H = 0xFA3 //PCA!疲块�0 的v谗�/比絆寄存砌缸 8!位。   �        "           `  !  !     �000,0000
sfr GCAP盠$= 0xEB; �/RAA!模块 1 的捕捉/1冉呔源嫫鞯�"8 幒。a    !             "    0        0  0 00,0000
Sfr(CCR1H � 0xFC9`k/PC 模块 1 的捕纸/比较寄存器皋08 位　                !        "        !!0000,0�00/?--------=----,/--------/ 8`    `           `�8     $ (                         ' ` 6$  5   4  "7  $2"  @1  !  0    Reset Val鮡
wfr PA_PWM0 = 0yF2; �/QC 寞?�0 PwM 寄搭嗺。        $   -  m"  -   -   -   -  UPC0H EpCp�( ,xxxx,xx00
sbr(PCqwPWO1 =00xF3; RCA 模侩! PWL牸拇嫫鳌c   `     (  -0  -   -   - � -(  -# EPK1H E@C1L"  鴛鴛,xx00M�//PGA_T_Mn:  ( 7      6    ` 5    0 4      3      "    � 1    � �
//0    (   "! -#     -      m 0    -   "� -  0   %    EPCNH  EPCnL
//B7-R2: �#留J//B1‥PC.J�: 在 PwM 模实下‘与 CCAPnH 砖成 ! 位瘦。
//B0(EPCnL): 赞 PWM D较裸� CCPnL(组成 9 位数担
//----=--------------)-,--/------=-�---,--�------/------)-------------o-,----�--
+/新一贱 1P((051�5裂 靛片机 AFC 特骡功能寄も器
�/ �      $   0          (  0                 w        6      5( "            $� (    2    1    0  !Rese� value
sfp ADS_C螻TR 9!1麭C{ //A疍 转换控制寄存器 ADCOPOWER0SPEED1 CPEEdp ADC_FLAG ADS_START ChS2"CLS1 CHS0#0000,0000
sfr ADC_RES  = 0xBD;  //A/D 转换结果高8位 ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	 0000,0000
sfr ADC_RESL = 0xBE;  //A/D 转换结果低2位                                           ADCV.1 ADCV.0	 0000,0000
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机 SPI 特殊功能寄存器
//                                      7     6     5     4     3     2     1     0    Reset Value
sfr SPCTL  = 0xCE; //SPI Control Register  SSIG  SPEN  DORD  MSTR  CPOL  CPHA  SPR1  SPR0  0000,0100
sfr SPSTAT = 0xCD; //SPI Status Register   SPIF  WCOL   -     -     -     -     -     -    00xx,xxxx
sfr SPDAT  = 0xCF; //SPI Data Register                                                     0000,0000
//--------------------------------------------------------------------------------
//新一代 1T 8051系列 单片机 IAP/ISP 特殊功能寄存器
sfr IAP_DATA    = 0xC2;
sfr IAP_ADDRH   = 0xC3;
sfr IAP_ADDRL   = 0xC4;
//                                                7    6    5      4    3    2    1     0    Reset Value
sfr IAP_CMD     = 0xC5; //IAP Mode Table          0    -    -      -    -    -   MS1   MS0   0xxx,xx00
sfr IAP_TRIG    = 0xC6;
sfr IAP_CONTR   = 0xC7; //IAP Control Register  IAPEN SWBS SWRST CFAIL  -   WT2  WT1   WT0   0000,x000
//--------------------------------------------------------------------------------

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#endif