#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "IIC.h"

#define EEPROM_TYPE     AT24C02 //ʵ�����õ���EEPROM����
/*  ���Թ�ѡ���EEPROM���� */
#define AT24C01     128
#define AT24C02     256
#define AT24C04     512
#define AT24C08     1024 
#define AT24C16     2048

/*  AT24CXX�豸��ַ */
#define AT24CXX_ADDRESS     0xA0 //��Ӳ���������й�ϵ
#define AT24CXX_READ        AT24CXX_ADDRESS|1    //��bit��Ч
#define AT24CXX_WRITE       AT24CXX_ADDRESS|0    //дbit��Ч
#define AT24CXX_DELAY(time)		delay(time)

/*	����ԭ�� */
void AT24CXX_Init(void);	
unsigned char AT24CXX_ReadOneByte(unsigned short int ReadAddr);
unsigned int AT24CXX_ReadLenByte(unsigned short int ReadAddr, unsigned char Len);

void AT24CXX_WriteOneByte(unsigned short int WriteAddr,unsigned char DataToWrite);
void AT24CXX_WriteLenByte(unsigned short int WriteAddr,unsigned int DataToWrite,unsigned char Len);

unsigned char AT24CXX_Check(void);

void AT24CXX_Read(unsigned short int ReadAddr,unsigned char *pBuffer,unsigned short int NumToRead);
void AT24CXX_Write(unsigned short int WriteAddr,unsigned char *pBuffer,unsigned short int NumToWrite);


#endif
