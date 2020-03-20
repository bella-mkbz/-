#include "iic.h"
#include<intrins.h>
#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}

uchar ChipAdress = 0x90;
/* ����������ʼ�ź� */
void I2CStart()
{
    I2C_SDA = 1; //����ȷ��SDA��SCL���Ǹߵ�ƽ
    I2C_SCL = 1;
    I2CDelay();
    I2C_SDA = 0; //������SDA
    I2CDelay();
    I2C_SCL = 0; //������SCL
}
/* ��������ֹͣ�ź� */
void I2CStop()
{
    I2C_SCL = 0; //����ȷ��SDA��SCL���ǵ͵�ƽ
    I2C_SDA = 0;
    I2CDelay();
    I2C_SCL = 1; //������SCL
    I2CDelay();
    I2C_SDA = 1; //������SDA
    I2CDelay();
}
/* I2C����д������dat-��д���ֽڣ�����ֵ-�ӻ�Ӧ��λ��ֵ */
bit I2CWrite(unsigned char dat)
{
    bit ack;  //�����ݴ�Ӧ��λ��ֵ
    unsigned char mask;  //����̽���ֽ���ĳһλֵ���������

    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        if ((mask&dat) == 0)  //��λ��ֵ�����SDA��
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        I2CDelay();
        I2C_SCL = 1;          //����SCL
        I2CDelay();
        I2C_SCL = 0;          //������SCL�����һ��λ����
    }
    I2C_SDA = 1;   //8λ���ݷ�����������ͷ�SDA���Լ��ӻ�Ӧ��
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    ack = I2C_SDA; //��ȡ��ʱ��SDAֵ����Ϊ�ӻ���Ӧ��ֵ
    I2CDelay();
    I2C_SCL = 0;   //������SCL���Ӧ��λ��������ס����

    return (~ack); //Ӧ��ֵȡ���Է���ͨ�����߼���
                   //0=�����ڻ�æ��д��ʧ�ܣ�1=�����ҿ��л�д��ɹ�
}
/* I2C���߶������������ͷ�Ӧ���źţ�����ֵ-�������ֽ� */
unsigned char I2CReadNAK()
{
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;  //����ȷ�������ͷ�SDA
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        I2CDelay();
        I2C_SCL = 1;      //����SCL
        if(I2C_SDA == 0)  //��ȡSDA��ֵ
            dat &= ~mask; //Ϊ0ʱ��dat�ж�Ӧλ����
        else
            dat |= mask;  //Ϊ1ʱ��dat�ж�Ӧλ��1
        I2CDelay();
        I2C_SCL = 0;      //������SCL����ʹ�ӻ����ͳ���һλ
    }
    I2C_SDA = 1;   //8λ���ݷ����������SDA�����ͷ�Ӧ���ź�
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    I2CDelay();
    I2C_SCL = 0;   //������SCL��ɷ�Ӧ��λ��������ס����

    return dat;
}
/* I2C���߶�������������Ӧ���źţ�����ֵ-�������ֽ� */
unsigned char I2CReadACK()
{
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;  //����ȷ�������ͷ�SDA
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        I2CDelay();
        I2C_SCL = 1;      //����SCL
        if(I2C_SDA == 0)  //��ȡSDA��ֵ
            dat &= ~mask; //Ϊ0ʱ��dat�ж�Ӧλ����
        else
            dat |= mask;  //Ϊ1ʱ��dat�ж�Ӧλ��1
        I2CDelay();
        I2C_SCL = 0;      //������SCL����ʹ�ӻ����ͳ���һλ
    }
    I2C_SDA = 0;   //8λ���ݷ����������SDA������Ӧ���ź�
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    I2CDelay();
    I2C_SCL = 0;   //������SCL���Ӧ��λ��������ס����

    return dat;
}
/* ��һ���ڴ�����ת��Ϊʮ�����Ƹ�ʽ���ַ�����
   str-�ַ���ָ�룬src-Դ���ݵ�ַ��len-���ݳ��� */
void MemToStr(unsigned char *str, unsigned char *src, unsigned char len)
{
    unsigned char tmp;

    while (len--)
    {
        tmp = *src >> 4;           //��ȡ��4λ
        if (tmp <= 9)              //ת��Ϊ0-9��A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        tmp = *src & 0x0F;         //��ȡ��4λ
        if (tmp <= 9)              //ת��Ϊ0-9��A-F
            *str++ = tmp + '0';
        else
            *str++ = tmp - 10 + 'A';
        *str++ = ' ';              //ת����һ���ֽ����һ���ո�
        src++;
    }
	*str = '\0';                   //����ַ���������
}
/* E2��ȡ������buf-���ݽ���ָ�룬addr-E2�е���ʼ��ַ��len-��ȡ���� */
void E2Read(unsigned char *buf, unsigned char addr, unsigned char len)
{
    do {                       //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д����
        I2CStart();
        if (I2CWrite(ChipAdress)) //Ӧ��������ѭ������Ӧ���������һ�β�ѯ
        {
            break;
        }
        I2CStop();
    } while(1);
		
    I2CWrite(addr);            //д����ʼ��ַ
    I2CStart();                //�����ظ������ź�
    I2CWrite(ChipAdress|0x01);  //Ѱַ����������Ϊ������
    while (len > 1)            //������ȡlen-1���ֽ�
    {
        *buf++ = I2CReadACK(); //����ֽ�֮ǰΪ��ȡ����+Ӧ��
        len--;
    }
    *buf = I2CReadNAK();       //���һ���ֽ�Ϊ��ȡ����+��Ӧ��
    I2CStop();
}
/* E2д�뺯����buf-Դ����ָ�룬addr-E2�е���ʼ��ַ��len-д�볤�� */
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
    while (len--)
    {
        do {                       //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д����
            I2CStart();
            if (I2CWrite(ChipAdress)) //Ӧ��������ѭ������Ӧ���������һ�β�ѯ
            {
                break;
            }
            I2CStop();
        } while(1);
        I2CWrite(addr++);  //д����ʼ��ַ
        I2CWrite(*buf++);  //д��һ���ֽ�����
        I2CStop();         //����д�������Եȴ�д�����
    }
}