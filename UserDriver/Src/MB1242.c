/* ============================================================================================
* Maxbotic MB1242 I2C library for ARM STM32F103xx Microcontrollers - Main header file 
* Has bit, byte and buffer I2C R/W functions
*  
*  Changelog:
           
** ========================================================================================= */

/* Includes */
#include "MB1242.h"


/*******************************************************************************
* Function Name  : IIC_Wait_Eeprom
* Description    : �ȴ�IIC���߿��У�ͬʱ�ȴ��豸���У���������������ȫû�б�Ҫ�ӣ����������������Ƶ�ϸ���д����������ģ�
                   �Ǹ��жϺ���while����Ķ�����������ģ����ı����ǵ�ADDR=1��������д���������𣿲��԰�
* Input          : �ӻ���ַ

*******************************************************************************/
void IIC_Wait_IfBusy(u8 slaveAddr)
{
    vu16 SR1_Tmp = 0;

    do
    {
        /* Send START condition */
        I2C_GenerateSTART(I2C1, ENABLE);
        /* Read I2C1 SR1 register */
        SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
        /* Send EEPROM address for write */
        I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter); 
    }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

    /* Clear AF flag */
    I2C_ClearFlag(I2C1, I2C_FLAG_AF);	
}

/**
* @brief  Writes one byte to the  MB1242.
* @param  slaveAddr : slave address MB1242_DEFAULT_ADDRESS
* @param  pBuffer : pointer to the buffer  containing the data to be written to the MB1242.
* @param  writeAddr : address of the register in which the data will be written
* @return None
*/
void MB1242_I2C_ByteWrite(u8 slaveAddr, u8 writeByte, u8 writeAddr)
{

  /* Send START condition */
  I2C_GenerateSTART(MB1242_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MB1242 address for write */
  I2C_Send7bitAddress(MB1242_I2C, slaveAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the MB1242's internal address to write to */
  I2C_SendData(MB1242_I2C, writeAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(MB1242_I2C, writeByte);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STOP condition */
  I2C_GenerateSTOP(MB1242_I2C, ENABLE);

 // EXT_CRT_SECTION();

}

/**
* @brief  Reads a block of data from the MB1242.
* @param  slaveAddr  : slave address MB1242_DEFAULT_ADDRESS
* @param  pBuffer : pointer to the buffer that receives the data read from the MB1242.
* @param  readAddr : MB1242's internal address to read from.
* @param  NumByteToRead : number of bytes to read from the MB1242 ( NumByteToRead >1  only for the Mgnetometer readinf).
* @return None
*/

void MB1242_I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead)
{
 // ENTR_CRT_SECTION();

  /* While the bus is busy */
  
  while(I2C_GetFlagStatus(MB1242_I2C, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(MB1242_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MB1242 address for write */
  I2C_Send7bitAddress(MB1242_I2C, slaveAddr, I2C_Direction_Transmitter); 

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(MB1242_I2C, ENABLE);

  /* Send the MB1242's internal address to write to */
  I2C_SendData(MB1242_I2C, readAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */
  I2C_GenerateSTART(MB1242_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MB1242 address for read */
  I2C_Send7bitAddress(MB1242_I2C, slaveAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* While there is data to be read */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(MB1242_I2C, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(MB1242_I2C, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(MB1242_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* Read a byte from the MB1242 */
      *pBuffer = I2C_ReceiveData(MB1242_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(MB1242_I2C, ENABLE);
//  EXT_CRT_SECTION();

}

/** ��ʼһ�γ���������ת����һ����Ҫ100ms���ȡ��
 */
bool MB1242_StartRangeMeasure(void) 
{ 
    //MB1242_I2C_ByteWrite(MB1242_I2C_ADDRESS, MB1242_COMMAND_READ_RANGE, 0); 
    Single_Write(MB1242_I2C_ADDRESS,0,MB1242_COMMAND_READ_RANGE);  
}


/** ��ȡһ�γ��������ݣ�һ����Ҫ���100ms��
 * @param range ��ȡ���ݵĸ�8λ�͵�8λ
 */
void MB1242_TakeRangeReading(u8* range) 
{ 
    //MB1242_I2C_BufferRead(MB1242_I2C_ADDRESS, range, 0, 2); 
    Mult_Read(MB1242_I2C_ADDRESS, 0, range, 2);    
}

/** ��ȡһ�γ��������ݣ�һ����Ҫ���100ms��
 * @param range ��ȡ���ݵĸ�8λ�͵�8λ
 */
//void MB1242_TakeRangeReading(u8* range) 
//{ 
//    //MB1242_I2C_BufferRead(MB1242_I2C_ADDRESS, range, 0, 2); 
//    Mult_Read(MB1242_I2C_ADDRESS, 0, range, 2);    
//}


 /* end of group MB1242_Library */

