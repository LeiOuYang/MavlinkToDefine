#include "AT24CXX.H"

/*****************************
**  ��ʼ��AT24Cxx�� IIC�ӿ�
**  
******************************/
void AT24CXX_Init(void)
{
    IIC_IOInit();
}

/*****************************
**  �� AT24CXX ָ����ַ����һ������
**  ReadAddr:��ʼ�����ĵ�ַ
**  ����ֵ :����������
******************************/
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr)
{
    unsigned char temp=0;
    IIC_Start();
    if(EEPROM_TYPE>AT24C16)
    {
        IIC_Send_Byte(AT24CXX_WRITE); //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ 
    }
    else IIC_Send_Byte(AT24CXX_WRITE); //����������ַ 0XA0,д����
    IIC_Wait_Ack();
    
    IIC_Send_Byte(ReadAddr%256); //���͵͵�ַ
    IIC_Wait_Ack();
    
    IIC_Start();
    IIC_Send_Byte(AT24CXX_READ); //�������ģʽ
    IIC_Wait_Ack();
    
    temp=IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����
    return temp;
}

/*****************************
**  �� AT24CXX ָ����ַд��һ������
**  WriteAddr :д�����ݵ�Ŀ�ĵ�ַ
**  DataToWrite:Ҫд�������
******************************/
void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite)
{
    IIC_Start();
    if(EEPROM_TYPE>AT24C16)
    {
        IIC_Send_Byte(AT24CXX_WRITE); //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
    }else 
        IIC_Send_Byte(AT24CXX_WRITE); //����������ַ 0XA0,д����
    
    IIC_Wait_Ack();
    
    IIC_Send_Byte(WriteAddr%256); //���͵͵�ַ
    IIC_Wait_Ack();
    
    IIC_Send_Byte(DataToWrite); //�����ֽ�
    IIC_Wait_Ack();
    
    IIC_Stop();//����һ��ֹͣ����
    AT24CXX_DELAY(10000);   //�ӳ�10ms
}

/*******************************************************************
**  �� AT24CXX �����ָ����ַ��ʼд�볤��Ϊ Len ������
**  �ú�������д�� 16bit ���� 32bit ������.
**  WriteAddr :��ʼд��ĵ�ַ
**  DataToWrite:���������׵�ַ
**  Len :Ҫд�����ݵĳ��� 2,4
********************************************************************/
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len)
{
    unsigned char t;
    for(t=0; t<Len; t++)
    {
        AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);    //��λд��͵�ַ
    }
}

/*******************************************************************
**  �� AT24CXX �����ָ����ַ��ʼ��������Ϊ Len ������
**  �ú������ڶ��� 16bit ���� 32bit ������.
**  ReadAddr :��ʼ�����ĵ�ַ
**  ����ֵ :����
**  Len :Ҫ�������ݵĳ��� 2,4
********************************************************************/
unsigned int AT24CXX_ReadLenByte(unsigned short int ReadAddr, unsigned char Len)
{
    unsigned char t;
    unsigned int temp=0;
    for(t=0;t<Len;t++)
    {
        temp<<=8;
        temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);    //����ʱ�ȶ���λ
    }
    return temp;
}

/*******************************************************************
**  ��� AT24CXX �Ƿ�����
**  �������� 24XX �����һ����ַ(255)���洢��־��.
**  ��������� 24C ϵ��,�����ַҪ�޸�
**  ���� 1:���ʧ��
**  ���� 0:���ɹ�
********************************************************************/
unsigned char AT24CXX_Check(void)
{
    unsigned char temp;
    temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����д AT24CXX
    if(temp==0X0d)  return 0;
    else//�ų���һ�γ�ʼ�������
    {
        AT24CXX_WriteOneByte(255,0X0d);
        temp=AT24CXX_ReadOneByte(255);
        if(temp==0X0d)  return 0;
    }
    return 1;
}

/*******************************************************************
**  �� AT24CXX �����ָ����ַ��ʼ����ָ������������
**  ReadAddr :��ʼ�����ĵ�ַ �� 24c02 Ϊ 0~255
**  pBuffer :���������׵�ַ
**  NumToRead:Ҫ�������ݵĸ���
**  
********************************************************************/
void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

/*******************************************************************
**  �� AT24CXX �����ָ����ַ��ʼд��ָ������������
**  WriteAddr :��ʼд��ĵ�ַ �� 24c02 Ϊ 0~255
**  pBuffer :���������׵�ַ
**  NumToWrite:Ҫд�����ݵĸ���
**  
********************************************************************/
void AT24CXX_Write(unsigned short int WriteAddr,unsigned char *pBuffer,unsigned short int NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}




