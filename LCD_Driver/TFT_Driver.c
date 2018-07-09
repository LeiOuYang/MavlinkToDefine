
#include "TFT_Driver.h"

void TFT_Delay(unsigned int val)
{
	val=val*5000;
	while(val--);
}

void TFT_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p; 


	p.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ��
	p.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
	p.FSMC_DataSetupTime = 0x05;		 //���ݽ���ʱ��
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	//FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;  

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	/* ʹ�� FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
}
void TFT_IO_Init(void)
{
	/* ����һ��GPIO_IintTypeDef���͵ı��� */
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ʹ��FSMCʱ��*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	/* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	/* ����FSMC���� */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// ����Ϊ50M
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;// ����Ϊ�����������ģʽ
	/* ѡ��GPIOD�ڵ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | 
	                              GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | 
	                              GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 
	                              GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* ѡ��GPIOE�ڵ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | 
	                              GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | 
	                              GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void TFT_Init(void)
{
	TFT_IO_Init();
	/* ���ú�FSMC���� */	
	TFT_FSMC_Init();
	TFT_Delay(200);
	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30);
	
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 	
	
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);
	
	LCD_WR_REG(0x11); //Exit Sleep
	TFT_Delay(120);
	LCD_WR_REG(0x29); //display on	
}

void TFT_DrawPoint(int x, int y, int color)
{
	LCD_WR_REG(XPOSITION);		//�е�ַ  
	LCD_WR_DATA(x>>8);    
	LCD_WR_DATA(x&0xff);	//��ʼ������λ��    	

	LCD_WR_REG(YPOSITION);  //�е�ַ
	LCD_WR_DATA(y>>8); 
	LCD_WR_DATA(y&0xff);	//��ʼ������λ��
	
	LCD_WR_REG(0x2C);
	
	LCD_WR_DATA(color);
}

unsigned short int  TFT_ReadPoint(int x, int y)
{
	return 0;
}
