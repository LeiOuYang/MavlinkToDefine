
/**********************************************************************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��ADI��̬�ɼ���
 * Ӳ�����ӣ�------------------
 *          | PB10  - SCL      |
 *          | PB11  - SDA      |
 *           ------------------
 * ��汾  ��ST3.5.0
 * ����    ��RAN_TT
 * ����    ��2013-05-21
**********************************************************************************/
#include <stdbool.h>
#include "sim_i2c.h"


/***************************************************************
 * ��������I2C_Config
 * ����  ��I2C����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 ***************************************************************/

void I2C2_Config(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure; 
  
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  
     //����I2C��SCL���ź�SDA����
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
     SCL_H;SDA_H;    //�����ͷ�
}


/******************************************************************************
/ ��������:Simulation IIC Timing series delay
/ �޸�����:none
/ �������:none
/ �������:none
/ ʹ��˵��:none
******************************************************************************/
__inline void I2C_delay(void)
{
        
   u8 i=I2C_DELAY_VAL; //��������Ż��ٶ�,��������͵�5����д��
   while(i) 
   { 
     i--; 
   }  
}


/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather  Start
****************************************************************************** */
bool I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if(!SDA_read)return false;  //SDA��Ϊ�͵�ƽ������æ,�˳�
    SDA_L;
    I2C_delay();
    if(SDA_read) return false;  //SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
    SDA_L;
    I2C_delay();
    return true;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{   
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{   
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather  Reserive Slave Acknowledge Single
****************************************************************************** */
bool I2C_WaitAck(void)   //����Ϊ:=1��ACK,=0��ACK
{
    SCL_L;
    I2C_delay();
    SDA_H;          
    I2C_delay();
    SCL_H;
    I2C_delay();
    if(SDA_read)
    {
      SCL_L;
      I2C_delay();
      return false;
    }
    SCL_L;
    I2C_delay();
    return true;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
uint8_t I2C_RadeByte(void)  //���ݴӸ�λ����λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;              
    while(i--)
    {
        ReceiveByte<<=1;      
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();  
        if(SDA_read)
        {
          ReceiveByte|=0x01;
        }
    }
    SCL_L;
    return ReceiveByte;
} 
/******************************************************************************
/ ��������:���ֽ�д��
/ �޸�����:none
/ �������:
/   @arg SlaveAddress   ��������ַ
/   @arg REG_Address    �Ĵ�����ַ
/   @arg REG_data       ��д����ֽ�����
/ �������: �������ֽ�����
/ ʹ��˵��:��ʱһ�������ĵ��ֽڶ�ȡ����
******************************************************************************/
bool Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    //I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return false;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();  
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    //delay5ms();
    return true;
}

/******************************************************************************
/ ��������:���ֽ�д��
/ �޸�����:none
/ �������:
/   @arg SlaveAddress   ��������ַ
/   @arg REG_Address    �Ĵ�����ַ
/   @arg REG_data       ��д����ֽ�����
/ �������: �������ֽ�����
/ ʹ��˵��:��ʱһ�������ĵ��ֽڶ�ȡ����
******************************************************************************/
bool Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
	//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return false;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();  
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    return true;
}



/******************************************************************************
/ ��������:���ֽ�д��
/ �޸�����:none
/ �������:
/   @arg SlaveAddress   ��������ַ
/   @arg REG_Address    �Ĵ�����ַ
/ �������: �������ֽ�����
/ ʹ��˵��:��ʱһ�������ĵ��ֽڶ�ȡ����
******************************************************************************/
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address)
{   
    uint8_t REG_data;       
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress); 
    //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop();return false;}
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

    REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return true;
    return REG_data;
}

/******************************************************************************
/ ��������:���ֽڶ�������
/ �޸�����:none
/ �������:
/   @arg SlaveAddress   ��������ַ
/   @arg REG_Address    �Ĵ�����ַ
/   @arg ptChar         �������
/   @arg size           ���������ݸ���,size�������=1
/ �������: �ɹ�ʧ�ܱ��
/ ʹ��˵��:none
******************************************************************************/
bool Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size)
{
    uint8_t i;
    
    if(size < 1)return false;
    if(!I2C_Start())return false;
    I2C_SendByte(SlaveAddress);
    if(!I2C_WaitAck()){I2C_Stop();return false;}
    I2C_SendByte(REG_Address);    
    I2C_WaitAck();
    
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();
    
    for(i=1;i<size; i++)
    {
        *ptChar++ = I2C_RadeByte();
        I2C_Ack();
    }
    *ptChar++ = I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    return true;    
}




