
#include "W25Qx.h"

/******************************************
* ��ȡSPI_FLASH��״̬�Ĵ���
* BIT7  6   5   4   3   2   1   0
* SPR   RV  TB BP2 BP1 BP0 WEL BUSY
* SPR: Ĭ��Ϊ 0,״̬�Ĵ�������λ�����WPʹ��
//TB,BP2,BP1,BP0: FLASHд��������
//WEL: дʹ������
//BUSY: æ���λ(1,æ;0,����)
//Ĭ��: 0x00
********************************************/
unsigned char SPI_W25Qx_ReadSR(void)
{ 
	unsigned char byte=0;
	W25Qx_FLASH_CS_LOW();	//Ƭѡ                          
	SPI_Flash_SendByte(W25X_CMD_ReadStatusReg1); 	//���Ĵ��� 1 ָ��   
	byte=SPI_Flash_ReadByte(W25X_DUMMY_BYTE);  		//��ȡ����   
	W25Qx_FLASH_CS_HIGH();  
	return byte;  
}

//*************************************************************************
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д
void SST25_Page_Program_SR(unsigned char sr)
{  
	SPI_W25Qx_Write_Enable();  //дʹ��
	                       
	W25Qx_FLASH_CS_LOW();	//Ƭѡ
	SPI_Flash_SendByte( W25X_CMD_WriteStatusReg ); //����д״̬�Ĵ���ָ��
	SPI_Flash_SendByte( sr );	//д��һ���ֽ�
	W25Qx_FLASH_CS_HIGH();	            
}  

/********************************************
*   W25QX FlashоƬ��ȡ�豸ID
*
********************************************/
unsigned int SPI_ReadDeviceID(void)
{
	unsigned int hb, lb;
	
	W25Qx_FLASH_CS_LOW();	//Ƭѡ
	
	SPI_Flash_SendByte(W25X_ManufactDeviveID);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	
	hb = SPI_Flash_ReadByte(W25X_DUMMY_BYTE);
	lb = SPI_Flash_ReadByte(W25X_DUMMY_BYTE);
	
	W25Qx_FLASH_CS_HIGH();		
	
	return ( hb<<8 | lb );	
}

/********************************************
*   W25QX FlashоƬдʹ��
*
********************************************/
void SPI_W25Qx_Write_Enable(void)
{
	W25Qx_FLASH_CS_LOW();	//Ƭѡ
	
	SPI_Flash_SendByte(W25X_CMD_WriteEnable);
	
	W25Qx_FLASH_CS_HIGH();	
}

/********************************************
*   W25QX FlashоƬдʧ��
*
********************************************/
void SPI_W25Qx_Write_Disable(void)
{
	W25Qx_FLASH_CS_LOW();	//Ƭѡ
	
	SPI_Flash_SendByte(W25X_CMD_WriteDisable);
	
	W25Qx_FLASH_CS_HIGH();	
}

/********************************************
*   W25QX �ȴ��������
*
********************************************/
void SPI_W25Qx_WaitForWriteEnd(void)
{
	unsigned int j;
	while(1)
	{	
		if( (SPI_W25Qx_ReadSR() & 0x01) == 0x00 )
		{
			return;
		}
		for( j=0; j<1000; ++j );
	}
}

/********************************************
*   W25QX ҳд����� 
*   �����ַ��Ч����0���ɹ�����1
********************************************/
unsigned char SPI_W25Qx_WritePage(unsigned char* pBuffer, unsigned int WriteAddr, unsigned int NumByteToWrite)
{
	if( (WriteAddr+NumByteToWrite)>W25X_SPI_SIZE || NumByteToWrite==0 )
	{
		return 0;		//��ַ��Ч�򷵻�0 
	}
	/* Enable the write access to the FLASH */
	SPI_W25Qx_Write_Enable();

	/* Select the FLASH: Chip Select low */
	W25Qx_FLASH_CS_LOW();
	/* Send &quot;Write to Memory &quot; instruction */
	SPI_Flash_SendByte(W25X_CMD_PageProgram);
	/*Send WriteAddr high nibble address byte to write to */
	SPI_Flash_SendByte( (unsigned char)((WriteAddr&0xFF0000)>>16) ) ;
	/* Send WriteAddr medium nibble address byte to write to */
	SPI_Flash_SendByte( (unsigned char)((WriteAddr&0xFF00)>>8) );
	/* Send WriteAddr low nibble address byte to write to */
	SPI_Flash_SendByte( (unsigned char)(WriteAddr& 0xFF) );

	/* while there is data to be written on the FLASH */
	while (NumByteToWrite--)
	{
		/* Send the current byte */
		SPI_Flash_SendByte(*pBuffer);
		/* Point on the next byte to be written */
		pBuffer++;
	}

	/*Deselect the FLASH: Chip Select high */
	W25Qx_FLASH_CS_HIGH();

	/* Wait the end of Flash writing */
	SPI_W25Qx_WaitForWriteEnd();
	
	return 1;
}

/********************************************
*   W25QX ��������
*   �����ַ��Ч����0���ɹ�����1
********************************************/
unsigned char SPI_W25Qx_ReadBuffer(unsigned char* pBuffer, unsigned int ReadAddr, unsigned int NumByteToRead)
{
	if( (ReadAddr+NumByteToRead)>W25X_SPI_SIZE || NumByteToRead==0 )
	{
		return 0;		//��ַ��Ч�򷵻�0 
	}
	/*Select the FLASH: Chip Select low */
	W25Qx_FLASH_CS_LOW();	 
	/*Send &quot;Read from Memory &quot; instruction */
	SPI_Flash_SendByte( W25X_CMD_FastRead );	 
	/*!&lt; Send ReadAddr high nibble address byte to read from */
	SPI_Flash_SendByte( (ReadAddr&0xFF0000)>>16 );
	/*!&lt; Send ReadAddr medium nibble address byte to read from */
	SPI_Flash_SendByte( (ReadAddr&0xFF00)>>8 );
	/*!&lt; Send ReadAddr low nibble address byte to read from */
	SPI_Flash_SendByte( ReadAddr&0xFF );
	 
	while (NumByteToRead--) /* while there is data to be read */
	{
		/*!&lt; Read a byte from the FLASH */
		*pBuffer = SPI_Flash_SendByte(W25X_DUMMY_BYTE);
		/*!&lt; Point to the next location where the byte read will be saved */
		pBuffer++;
	}
	 
	/*!&lt; Deselect the FLASH: Chip Select high */
	W25Qx_FLASH_CS_HIGH();
	return 0;
}

/********************************************
*    ������Ƭ Flash ����
*
********************************************/
void SPI_W25Qx_EraseChip(void)
{
	/* Send write enable instruction */
	SPI_W25Qx_Write_Enable();
	SPI_W25Qx_WaitForWriteEnd();
	/* Bulk Erase */
	/* Select the FLASH: Chip Select low */
	W25Qx_FLASH_CS_LOW();
	/* Send Bulk Erase instruction */
	SPI_Flash_SendByte(W25X_CMD_ChipErase);
	/* Deselect the FLASH: Chip Select high */
	W25Qx_FLASH_CS_HIGH();
	 
	/* Wait the end of Flash writing */
	SPI_W25Qx_WaitForWriteEnd();
}

/********************************************
*    ����һ������������  
*    Sector_Dst_Addr�� 0-511
********************************************/
void SPI_W25Qx_SectorErase(unsigned int Sector_Dst_Addr)
{
	unsigned int add = W25X_SPI_SECTORSIZE*Sector_Dst_Addr;
	/* Send write enable instruction */
	SPI_W25Qx_Write_Enable();
	SPI_W25Qx_WaitForWriteEnd();
	/* Bulk Erase */
	/* Select the FLASH: Chip Select low */
	W25Qx_FLASH_CS_LOW();
	/* Send Bulk Erase instruction */
	SPI_Flash_SendByte(W25X_CMD_SectorErase);
	/*!&lt; Send ReadAddr high nibble address byte to read from */
	SPI_Flash_SendByte( (add&0xFF0000)>>16 );
	/*!&lt; Send ReadAddr medium nibble address byte to read from */
	SPI_Flash_SendByte( (add&0xFF00)>>8 );
	/*!&lt; Send ReadAddr low nibble address byte to read from */
	SPI_Flash_SendByte( add&0xFF );
	/* Deselect the FLASH: Chip Select high */
	W25Qx_FLASH_CS_HIGH();
	 
	/* Wait the end of Flash writing */
	SPI_W25Qx_WaitForWriteEnd();
}
