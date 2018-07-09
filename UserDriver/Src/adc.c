#include "adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
#define TEMP_SENSOR  ADC_Channel_16 //�ڲ��¶ȴ�����AD�ɼ�ͨ��
#define INNER_VOLTAGE	ADC_Channel_17			 //�ڲ��ο���ѹAD�ɼ�ͨ��
#define CURRENT_COLLECTION	ADC_Channel_8  //��ӦIO��  PB8
#define AD_DMA_ENABLE DMA_Cmd(DMA1_Channel1, ENABLE);	
#define AD_DMA_DISABLE DMA_Cmd(DMA1_Channel1, DISABLE);	

#define INNER_VOL_REF	1.24  //�ڲ��ο���ѹֵ��ͨ����ֵ��aDC����ֵ������ô�ʱ��VDDAֵ

#define ADC_SIZE 30
__IO uint16_t ADC_ConvertedValue[ADC_SIZE];  //����DMA��ʽ�� 1-ͨ��9-�ɼ�����  2-ͨ��16-�ɼ��ڲ��¶�  3-ͨ��17-�ɼ��ο���ѹ


/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.01
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC1,����ʱ������������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //PA5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				// PB1,����ʱ������������
}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
		   
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		        // �������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	        // ADת��ֵ����ŵ��ڴ����ַ	�����Ǹ�����ַ��
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                        // ������Ϊ���ݴ������Դ	
  DMA_InitStructure.DMA_BufferSize = ADC_SIZE;                                     // ����ָ��DMAͨ�� DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          // �����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			        // �ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // HalfWord
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		                    //������ѭ��ģʽ��
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                    //�����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		                        //û������Ϊ�ڴ浽�ڴ�Ĵ���
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);											//ENABLE��

  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        //��������ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			                    //��ͨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			            //����ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;       //�������������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//Right
  ADC_InitStructure.ADC_NbrOfChannel = 3;				                    //3��ͨ��ת��
  ADC_Init(ADC1, &ADC_InitStructure);
	
  	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

  /* ADC1 regular channel16 configuration */ 
  //���ò���ͨ��IN16, ���ò���ʱ��
	ADC_RegularChannelConfig(ADC1, CURRENT_COLLECTION, 1, ADC_SampleTime_239Cycles5);	
	ADC_RegularChannelConfig(ADC1, TEMP_SENSOR, 2, ADC_SampleTime_239Cycles5);	
  ADC_RegularChannelConfig(ADC1, INNER_VOLTAGE, 3, ADC_SampleTime_239Cycles5);	

  //ʹ���¶ȴ��������ڲ��ο���ѹ   
  ADC_TempSensorVrefintCmd(ENABLE);                                    

   /* Enable ADC1 DMA */	  
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);			                              
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));                         

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);				                        
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	  
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

/*
 * ��������getInnerTempValue()
 * ����  ����
 * ����  �������õ���ԭʼֵ
 * ���  �����϶�
 * ����  ���ⲿ����
 */
float getInnerTempValue(void)
{
	float c=0.0, v=0.0;
	unsigned char i = 0;
	unsigned int sum = 0;
	float max=0.0,min=0.0;	//ԭʼֵ
	int imax=0,imin=0;			//����acc����ȡ����ֵ
	int iadc = 0;
	int acc = 10;  //����
	
	AD_DMA_DISABLE;
	
	imax = imin = (int)(ADC_ConvertedValue[1]*acc);
	
	for( ; i<ADC_SIZE/3; ++i)
	{
		iadc = (int)(ADC_ConvertedValue[1+i*3]*acc);
		imax = (int)(max*acc);
		imin = (int)(min*acc);
		if(iadc>imax)
		{
			max = ADC_ConvertedValue[1+i*3];
		}
		if(iadc<imin)
		{
			min = ADC_ConvertedValue[1+i*3];
		}
		sum += ADC_ConvertedValue[1+i*3];
	}
	AD_DMA_ENABLE;
	
	sum = sum-max-min;
	v = (((sum/((ADC_SIZE/3.0)-2.0)))/4095) * 3.3;
	c = (V25_TYPE-v)*1000/AVG_SLOPE;
	c += 25;
;	
	return c;
}

/*
 * ��������getInnerVolValue()
 * ����  ����
 * ����  �������õ���ԭʼֵ
 * ���  ���ڲ���ѹ
 * ����  ���ⲿ����
 */
float getInnerVolValue(void)
{
	float v=0.0;
	unsigned char i = 0;
	unsigned int sum = 0;
	float max=0.0,min=0.0;	//ԭʼֵ
	int imax=0,imin=0;			//����acc����ȡ����ֵ
	int iadc = 0;
	int acc = 10;  //����
	
	AD_DMA_DISABLE;
	
	imax = imin = (int)(ADC_ConvertedValue[2]*acc);
	
	for( ; i<ADC_SIZE/3; ++i)
	{
		iadc = (int)(ADC_ConvertedValue[2+i*3]*acc);
		imax = (int)(max*acc);
		imin = (int)(min*acc);
		if(iadc>imax)
		{
			max = ADC_ConvertedValue[2+i*3];
		}
		if(iadc<imin)
		{
			min = ADC_ConvertedValue[2+i*3];
		}
		sum += ADC_ConvertedValue[2+i*3];
	}
	AD_DMA_ENABLE;
	sum = sum-max-min;
	
	v = (INNER_VOL_REF*4095)/(sum/(ADC_SIZE/3.0-2));
	
	return v;
}

/*
 * ��������getCurrentValue()
 * ����  ����
 * ����  �������õ���ԭʼֵ
 * ���  ���ڲ���ѹ
 * ����  ���ⲿ����
 */
float getCurrentValue(void)
{
	float v=0.0;
	unsigned char i = 0;
	unsigned int sum = 0;
	float max=0.0,min=0.0;	//ԭʼֵ
	int imax=0,imin=0;			//����acc����ȡ����ֵ
	int iadc = 0;
	int acc = 10;  //����
	
	AD_DMA_DISABLE;
	max = min = ADC_ConvertedValue[0];
	imax = imin = (int)(ADC_ConvertedValue[0]*acc);
	
	for( ; i<ADC_SIZE/3; ++i)
	{
		iadc = (int)(ADC_ConvertedValue[0+i*3]*acc);
		imax = (int)(max*acc);
		imin = (int)(min*acc);
		if(iadc>imax)
		{
			max = ADC_ConvertedValue[0+i*3];
		}
		if(iadc<imin)
		{
			min = ADC_ConvertedValue[0+i*3];
		}
		sum += ADC_ConvertedValue[0+i*3];
	}
	AD_DMA_ENABLE;
	sum = sum-max-min;
	v = (((sum/((ADC_SIZE/3.0)-2.0)))/4095) * 3.3;
	
	return v;
}

/********************************************
*  ������ת�����ַ�����,�Զ�ȥ���ַ���ǰ���0��������������
*  ������ pc �ַ�����ָ���ַ
*					f  ��Ҫת���ĸ�����
*					intn  ����λ��
*         decn  С��λ��
*******************************************/
void floatToString(char *pc, float f, unsigned char intn, unsigned char decn)
{
	char *pstr = 0;
	int num = 0;
	unsigned char i = 0;
	float dec = 0.0;
	unsigned char j = 0;
	
	if(0==pc || 0==intn)  return;
	
	pstr = pc;
	
	num = (int)f;
	if(num<0)   //����
	{
		*pstr = '-';
		++pstr;
		num = -num;
	}
	num %= (unsigned int)pow(10, intn);
	
	for(i=intn; i>0; --i) //��������
	{
		*pstr = (num/(unsigned int)pow(10,i-1))+0x30;
		if(0x30==*pstr&&0==j)   //ͷ��Ϊ0
		{
			continue;
		}
		j = 1;
		num = num%((unsigned int)pow(10, i-1));
		++pstr;
	}
	
	if(0==j)
	{
		*pstr = '0';
		++pstr;
	}
	
	if(0==decn)					//С�����ֲ�����������
	{
//		*pstr = '\r';
//		++pstr;
//		*pstr = '\n';
//		++pstr;
		*pstr = '\0';
		return;
	}
	
	*pstr = '.';	
	++pstr;
	
	dec = f-(int)f;       //ȡС������
	num = (int)(dec*pow(10, decn));
	if(num<0)
	{
		num = -num;
	}
	
	for(i=decn; i>0; --i)		//����С������
	{
		*pstr = (num/pow(10,i-1))+0x30;
		num = num%((unsigned int)pow(10, i-1));
		++pstr;
	}	
//	*pstr = '\r';
//	++pstr;
//	*pstr = '\n';
//	++pstr;
	*pstr = '\0';
	
	
}
