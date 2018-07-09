/**************************************************************
*		Ӳ����������
*		����1�� 9600, 8, N, 1
*		����2�� NONE
*	  ����3�� 9600��8��N, 1  �л�������֧�ֶ��в���
**************************************************************/

#include "usart.h"
#include "task.h"

/* �ⲿ�������� */
extern xQueueHandle com1_tx_queue_handle;
extern xQueueHandle com1_rx_queue_handle;
extern xQueueHandle com3_rx_queue_handle;
extern xQueueHandle com3_tx_queue_handle;
extern xQueueHandle com2_tx_queue_handle;
extern xQueueHandle com2_rx_queue_handle;

/* ���������жϡ�DMA�Ⱥ궨�� */
#define USART1_SEND_DMA_ENABLE 	0
#define USART1_REC_DMA_ENABLE 	0
#define USART2_SEND_DMA_ENABLE 	0
#define USART2_REC_DMA_ENABLE   0
#define USART3_SEND_DMA_ENABLE 	0
#define USART3_REC_DMA_ENABLE		0

#if USART1_SEND_DMA_ENABLE
	#define USART1_TX_BUFF_LEN 256
	u8 usart1_tx_dma_buff[USART1_TX_BUFF_LEN]; 
#endif

#if USART2_SEND_DMA_ENABLE
	#define USART2_TX_BUFF_LEN 256
	u8 usart2_tx_dma_buff[USART1_TX_BUFF_LEN]; 
#endif

#if USART3_SEND_DMA_ENABLE
	#define USART3_TX_BUFF_LEN 256
	u8 usart3_tx_dma_buff[USART3_TX_BUFF_LEN]; 
#endif



/*************************************************************
* ����1����:
* 				ʱ��ʹ�ܡ��������á��ж����ȼ�����
**************************************************************/
void usart1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef nvicStruct;
	
	/* ʱ�ӳ�ʼ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	
	/* IO���ų�ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ��ʼ�����ڲ��� */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	
	USART_Init(USART1, &USART_InitStructure);
	
	/* ����1�ж� */
	nvicStruct.NVIC_IRQChannel = USART1_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 9;
	nvicStruct.NVIC_IRQChannelSubPriority = 0;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct); 
	
	/* ����1����DMAͨ��  ͨ��DMA1-5 */
	#if USART1_REC_DMA_ENABLE
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel5);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);	 				//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart1_rx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART1_RX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   											//������ѭ������ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;										//ͨ��5λ�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��5�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		/* DMA1-5 �ж� */
		nvicStruct.NVIC_IRQChannel = DMA1_Channel5_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
		nvicStruct.NVIC_IRQChannelSubPriority = 0;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);		//ʹ�ܴ���USART1��DMA
		DMA_Cmd(DMA1_Channel5,DISABLE); 
	#endif
	
		/***  ����1����DMAͨ��  ͨ��DMA1-4  ***/
	#if USART1_SEND_DMA_ENABLE==1																							//����2ʹ��DMA��������
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel4);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);					//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart1_tx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART1_TX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	   												//������һ��ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;									//ͨ��4�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��4�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);			
		/*�ж�*/
		nvicStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel4,DISABLE); 
	#endif	
	
	#if USART1_REC_DMA_ENABLE!=1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	#endif
	USART_Cmd(USART1, DISABLE);
	
}

/*---------------------------------------------------------------------
 * USART2 ����,115200 8-N-1	   
 */
void usart2_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef nvicStruct;
	
	/* ʱ�ӳ�ʼ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* IO���ų�ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ���ڲ�����ʼ�� */  
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	USART_Init(USART2, &USART_InitStructure);
	
	/* ����2�ж� */
	nvicStruct.NVIC_IRQChannel = USART2_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 8;
	nvicStruct.NVIC_IRQChannelSubPriority = 0;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct); 
	
	/* ����2����DMAͨ��  ͨ��DMA1-6 */
	#if USART2_REC_DMA_ENABLE
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);	 				//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart2_rx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART2_RX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   											//������ѭ������ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;										//ͨ��5λ�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��5�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);
		/* DMA1-5 �ж� */
		nvicStruct.NVIC_IRQChannel = DMA1_Channel6_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);		//ʹ�ܴ���USART2��DMA
		DMA_Cmd(DMA1_Channel6,DISABLE); 
	#endif
	
		/***  ����1����DMAͨ��  ͨ��DMA1-4  ***/
	#if USART2_SEND_DMA_ENABLE==1																							//����2ʹ��DMA��������
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel7);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART2->DR);					//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart2_tx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART2_TX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	   												//������һ��ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;									//ͨ��4�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��4�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel7, &DMA_InitStructure);			
		/*�ж�*/
		nvicStruct.NVIC_IRQChannel = DMA1_Channel7_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 1;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel7,DISABLE); 
	#endif	
	
	#if USART2_REC_DMA_ENABLE!=1
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	#endif
	USART_Cmd(USART2, DISABLE);	
	   
}

/*************************************************************
* ����3����:
* 				ʱ��ʹ�ܡ��������á��ж����ȼ�����
**************************************************************/
void usart3_config(void)
{
	USART_InitTypeDef usartStruct;
	GPIO_InitTypeDef GPIOInit;
  NVIC_InitTypeDef nvicStruct;
	DMA_InitTypeDef DMA_InitStructure;
	
	/* ʱ�ӳ�ʼ�� */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE );	
	
	/* ����IO���� */
	GPIOInit.GPIO_Pin = GPIO_Pin_11;						
	GPIOInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 	
	GPIO_Init(GPIOB, &GPIOInit);
	
	GPIOInit.GPIO_Pin = GPIO_Pin_10; 					
	GPIOInit.GPIO_Mode = GPIO_Mode_AF_PP; 				
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;				
	GPIO_Init(GPIOB, &GPIOInit);
			
	/*** ��ʼ�����ڲ���	***/
	usartStruct.USART_BaudRate = 115200;    								
	usartStruct.USART_WordLength = USART_WordLength_8b;    	
	usartStruct.USART_StopBits = USART_StopBits_1;					
	usartStruct.USART_Parity = USART_Parity_No;							
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	usartStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						  		
	USART_Init(USART3, &usartStruct);	
		
	/* ����3�ж� */
	nvicStruct.NVIC_IRQChannel = USART3_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 7;
	nvicStruct.NVIC_IRQChannelSubPriority = 0;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct); 
	
		/* ����3����DMAͨ��  ͨ��DMA1-3 */
	#if USART3_REC_DMA_ENABLE
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel3);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);	 				//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart3_rx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART3_RX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	   											//������ѭ������ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;										//ͨ��3λ�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��3�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);
		/* DMA1-3 �ж� */
		nvicStruct.NVIC_IRQChannel = DMA1_Channel3_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 1;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);		//ʹ�ܴ���USART1��DMA
		DMA_Cmd(DMA1_Channel3,DISABLE); 
	#endif
	
		/***  ����3����DMAͨ��  ͨ��DMA1-2  ***/
	#if USART3_SEND_DMA_ENABLE==1																							//����2ʹ��DMA��������
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    
		DMA_DeInit(DMA1_Channel2);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART3->DR);					//����Ĵ�����ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart3_tx_dma_buff; 				//DMA�ڴ����ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;        							//������Ϊ���ݵ���Դ
		DMA_InitStructure.DMA_BufferSize = USART3_TX_BUFF_LEN;			  					//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����Ĵ�����ַ������
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			  					//�ڴ��ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ��Ϊ8λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ��Ϊ8λ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	   												//������һ��ģʽ��
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;									//ͨ��2�е����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//ͨ��2�������ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel2, &DMA_InitStructure);			
		/*�ж�*/
		nvicStruct.NVIC_IRQChannel = DMA1_Channel2_IRQn;	
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 1;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvicStruct); 
		
		DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);		//����DMA1��������ж�
		USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel2,DISABLE); 
	#endif
	
	#if USART3_REC_DMA_ENABLE!=1
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	#endif
	USART_Cmd(USART3, DISABLE);

}
//////////////////////////////////////////////////////////////
//	���ڲ���DMA������غ���
//               
/////////////////////////////////////////////////////////////

/*************************************************************
* ���ô��ڷ������ݸ�����DMA��ʽ����
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
* 			 countΪ���ݸ���
**************************************************************/
void insertDataToBuff(unsigned char serial_dma, const char* buff, unsigned int len)
{
	unsigned int* pi = 0;
	unsigned char* pc = 0;
	unsigned int i = 0;
  unsigned int j = 0;
	
	while(i<5000)
	{
		pc = getSendEnableStatus(serial_dma);   //�ж��Ƿ�������
		if(*pc)  //���������ݣ����򻺳�����д������
		{
			if(1==serial_dma)
			{
				#if USART1_SEND_DMA_ENABLE==1		//����1ʹ��DMA��������
					copyToBuff(usart1_tx_dma_buff, buff, len);		//�����ݸ��Ƶ�������	
				#endif
			}else if(2==serial_dma)
			{
				#if USART2_SEND_DMA_ENABLE==1
					copyToBuff(usart2_tx_dma_buff, buff, len);		//�����ݸ��Ƶ�������	
				#endif
			}else if(3==serial_dma)
			{
				#if USART3_SEND_DMA_ENABLE==1
					copyToBuff(usart3_tx_dma_buff, buff, len);		//�����ݸ��Ƶ�������	
				#endif
			}
			serialSendData(serial_dma,len);					//��������
			break;
		}
		++i;
	}
}

/*************************************************************
* ���ô��ڷ������ݸ�����DMA��ʽ����
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
* 			 countΪ���ݸ���
**************************************************************/
void setSendDataCount(unsigned char serial_dma, unsigned int count)
{
	unsigned int* pi = 0;
	if(serial_dma>=4) return;
	pi = getSendDataCount(serial_dma);
	if(0==pi) return;
	
	*pi = count;
}

/*************************************************************
* ��ȡ��Ҫ���ڷ������ݵĸ�����DMA��ʽ����
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
**************************************************************/
unsigned int* getSendDataCount(unsigned char serial_dma)
{
	static unsigned int dma_count[3] = {0};   /*��ӦDMAͨ����Ҫ���͵����ݸ���*/
	if(serial_dma>=4) return 0;
		
	return &dma_count[serial_dma-1];
}

/*************************************************************
* ���ô��� DMAͨ��ʹ�ܱ�־
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
* 			 countΪ���ݸ���
**************************************************************/
void setSendEnableStatus(unsigned char serial_dma, unsigned char enable)
{
	unsigned char* pi = 0;
	if(serial_dma>=4) return;
	pi = getSendEnableStatus(serial_dma);
	if(0==pi) return;
	
	*pi = enable;
}

/*************************************************************
* ��ȡ��Ҫ���ڷ������ݵĸ�����DMA��ʽ����
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
**************************************************************/
unsigned char* getSendEnableStatus(unsigned char serial_dma)
{
	static unsigned char serial_dma_enbale[3] = {1,1,1}; //��������DMA���ͱ�־��Ĭ�������ʹ��; 
	
	if(serial_dma>=4) return 0;
		
	return &serial_dma_enbale[serial_dma-1];
}

/*************************************************************
* ���ô��ڷ������ݸ�����DMA��ʽ����
* ������ serial_dmaΪ���ڶ�Ӧ��ͨ��  1��2��3�ֱ��Ӧ����1��2��3
**************************************************************/
void serialSendData(unsigned char serial_dma,unsigned int len)
{	
	if(1==serial_dma)
	{
		#if USART1_SEND_DMA_ENABLE==1
			DMA_SetCurrDataCounter(DMA1_Channel4,len); 
			DMA_Cmd(DMA1_Channel4,ENABLE); 						
		#endif
	}else if(2==serial_dma)
	{
		#if USART2_SEND_DMA_ENABLE==1
			DMA_SetCurrDataCounter(DMA1_Channel7,len); 
			DMA_Cmd(DMA1_Channel7,ENABLE); 		
		#endif
	}else if(3==serial_dma)
	{
		#if USART3_SEND_DMA_ENABLE==1
			DMA_SetCurrDataCounter(DMA1_Channel2,len); 
			DMA_Cmd(DMA1_Channel2,ENABLE); 		
		#endif
	}	
	setSendEnableStatus(serial_dma,0);
}

/*************************************************************
* DMA1�жϣ�����1��������  DMA 2ͨ���ж�
* 
**************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2)==SET)
	{
		taskENTER_CRITICAL();
		setSendEnableStatus(3,1);   			//����1�����������
		setSendDataCount(3,0);						//����������Ч����
		taskEXIT_CRITICAL();
		DMA_Cmd(DMA1_Channel2,DISABLE);		//�ر�DMA����
		DMA_ClearFlag(DMA1_FLAG_TC2);			//����жϱ�־	
	}		
}

/*************************************************************
* DMA1�жϣ�����1��������  DMA4ͨ���ж�
* 
**************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET)
	{
		taskENTER_CRITICAL();
		setSendEnableStatus(1,1);   			//����1�����������
		setSendDataCount(1,0);						//����������Ч����
		taskEXIT_CRITICAL();
		DMA_Cmd(DMA1_Channel4,DISABLE);		//�ر�DMA����
		DMA_ClearFlag(DMA1_FLAG_TC4);			//����жϱ�־	
	}		
}

/*************************************************************
* DMA1�жϣ�����2��������  DMA7ͨ���ж�
* 
**************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7)==SET)
	{
		taskENTER_CRITICAL();
		setSendEnableStatus(2,1);   			//����2�����������
		setSendDataCount(2,0);						//����������Ч����
		taskEXIT_CRITICAL();
		DMA_Cmd(DMA1_Channel7,DISABLE);		//�ر�DMA����
		DMA_ClearFlag(DMA1_FLAG_TC7);			//����жϱ�־	
	}		
}

///////////////////////////////////////////////////////
//   ����DMA��غ���
///////////////////////////////////////////////////////

/*************************************************************
* �����ѯ��ʽ�����ַ�
* ���룺 ���ڼĴ����ṹ�壬 �ַ����׵�ַ
**************************************************************/
void usartSendChar(USART_TypeDef* const USARTx, unsigned char c)
{	

	USART_SendData(USARTx, c); //����һ���ַ�
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);  //��ⷢ�ͼĴ���Ϊ��
	USART_ClearFlag(USARTx,USART_FLAG_TXE);		//��ոñ�־λ��		
		
}

/*************************************************************
* �����ѯ��ʽ�����ַ���
* ���룺 ���ڼĴ����ṹ�壬 �ַ����׵�ַ
**************************************************************/
void usartSendString(USART_TypeDef* const USARTx, const u8* pc)
{
	if(pc==0) 
	{
		return;
	}
	
	while(*pc!='\0')
	{
		USART_SendData(USARTx, *pc); //����һ���ַ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);  //��ⷢ�ͼĴ���Ϊ��
		USART_ClearFlag(USARTx,USART_FLAG_TXE);		//��ոñ�־λ��		
		
		++pc;
	}
}


/*************************************************************
* ����3���������жϣ�ÿ����100�����ݣ���������
* �����յ����ݣ������������ִ��
**************************************************************/
void USART3_IRQHandler(void)
{
	u8 ch = 0;
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;  /* �����ݶ�����д�����ݣ���������˸������ȼ������񣬸�ֵΪpdTRUE */
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(USART3, USART_FLAG_TC);
	}
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)==SET)
	{
		ch = USART3->DR;

		if(xQueueIsQueueFullFromISR(com3_rx_queue_handle)==pdFALSE) /* �ж϶����Ƿ��пռ� */
		{
			xQueueSendToBackFromISR(com3_rx_queue_handle, &ch, &pxHigherPriorityTaskWoken);	
		}

		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);		
		
	}
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
	
//	if(USART_GetITStatus(USART3,USART_IT_ORE) != RESET) 
//	{
//		unsigned char del = 0;
//		del = USART_ReceiveData(USART3); 
//		USART_ClearITPendingBit(USART3, USART_IT_ORE);    
//	}
}

/*************************************************************
* ����2���������жϣ�ÿ����100�����ݣ���������
* �����յ����ݣ������������ִ��
**************************************************************/
void USART2_IRQHandler(void)
{
	u8 ch = 0;  
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;  /* �����ݶ�����д�����ݣ���������˸������ȼ������񣬸�ֵΪpdTRUE */
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)==SET)
	{
		ch = USART2->DR;
		if(xQueueIsQueueFullFromISR(com2_rx_queue_handle)==pdFALSE)
		{
			xQueueSendToBackFromISR(com2_rx_queue_handle, &ch, &pxHigherPriorityTaskWoken);
		}			
		
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);		
	}
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(USART2, USART_FLAG_TC);
	}
	
	if(USART_GetITStatus(USART2,USART_IT_ORE) != RESET) 
	{
		unsigned char del = 0;
		del = USART_ReceiveData(USART2); 
		USART_ClearITPendingBit(USART2, USART_IT_ORE);    
	}
}

/*************************************************************
* ����1���������жϣ�ÿ����100�����ݣ���������
* �����յ����ݣ������������ִ��
**************************************************************/
void USART1_IRQHandler(void)
{
	u8 ch = 0;  
	portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;  /* �����ݶ�����д�����ݣ���������˸������ȼ������񣬸�ֵΪpdTRUE */
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
	{
		ch = USART1->DR;
		xQueueSendToBackFromISR(com1_rx_queue_handle, &ch, &pxHigherPriorityTaskWoken);	
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		IWDG_Feed();
		
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);		
	}
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
	
	if(USART_GetITStatus(USART1,USART_IT_ORE) != RESET) 
	{
		unsigned char del = 0;
		del = USART_ReceiveData(USART1); 
		USART_ClearITPendingBit(USART1, USART_IT_ORE);    
	}
}


/*************************************************************
*	usart2-tx  dma send end status  
* return: 0 - busy, 1 - idle
*************************************************************/
unsigned char* getSendStatus(unsigned char index)
{
	static unsigned char status[3] = {1};   //Ĭ��Ϊ����״̬ 
	return &status[index];
}
void setSendStatus(unsigned char index, unsigned char flag)
{
	unsigned char *status;
	status = getSendStatus(index);
	*status = flag;
}
void comStartSendBuff(unsigned char chan, unsigned char* src, unsigned int count)
{
	unsigned int num;
	unsigned char *pc;
	unsigned char *psrc;
	
	if(0==src || 0==count)  return;
	switch(chan)
	{
		case 1:
		{
			break;
		}
		case 2:
		{
			#if USART2_SEND_DMA_ENABLE==1
				pc = usart2_tx_dma_buff;
				for(num=count; num>0; --num)
				{
					*pc = *psrc;
					++pc;
					++psrc;
				}	
	
				DMA_SetCurrDataCounter(DMA1_Channel7,count);  
				DMA_Cmd(DMA1_Channel7,ENABLE);
				setSendStatus(2,0);   //�������ͣ�æ״̬
			#endif
			break;
		}
		case 3:
		{
			
			break;
		}
		default: break;
	}	

}


/*************************************************************
* ��ȡ��ر�־λ
* 
**************************************************************/
u8 getUsartBuffReadFlag(void)
{
	return USART3_BuffReadEnable;
}

/*************************************************************
* ������ر�־λ
* 
**************************************************************/
void setUsartBuffReadFlag(u8 ui)
{
	USART3_BuffReadEnable = ui;
}

/*********************************************
*		����ת���� HEX 
*
**********************************************/
void byteToHEX(unsigned char c, unsigned char* pc)
{
	unsigned char hb, lb;
	hb = lb = 0;
	
	hb = c / 16;
	lb = c % 16;
	
	if( hb<10 )
	{
		*pc = hb + 48;    //??????ASCII?
	} 
	else
	{
		*pc = hb - 10 + 65;	 //???????10???
	}
	
	++pc; 
	
	if( lb<10 )
	{
		*pc = lb + 48;    //??????ASCII?
	} 
	else
	{
		*pc = lb - 10 + 65;	 //???????10???
	}
	
	++pc;
	*pc = '\0';	
}

/*********************************************
*		��������
*
**********************************************/
void copyToBuff(char* dest, const char* src, unsigned int len)
{
	unsigned int i = 0;
	
	if(0==dest && 0==src && 0==len)  return;
	
	for(i=0; i<len&&'\0'!=*src; i++)
	{
		*dest = *src;
		++dest;
		++src;
	}
}
