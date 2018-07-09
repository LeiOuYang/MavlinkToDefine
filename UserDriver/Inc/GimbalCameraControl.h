
#ifndef GIMBAL_CAMERA_CONTROL_H
#define GIMBAL_CAMERA_CONTROL_H
  
  #include "usart.h"	
	#include "timers.h"
	
	extern xQueueHandle landingQueueHandle;
	extern TimerHandle_t xTimerUser;
//	unsigned char buff[100] = {0};  //���ڷ������ݻ��������û������Ĺ����ǵȴ��ϴ�DMA�����������֮ǰ���������
//	unsigned char saveCount= 0; //�Ѿ��������ݵĸ���
//	unsigned char sendStatus = 0; //���ڷ��ͱ�־  0-æ  1-����
 
	#define PINLING_GIMBAL_ZOOM18	 1			//Ʒ��18��
	#define TUOPU_DOUBLE_CAMERA	 1				//����˫�����̨
	
	typedef enum _usart_com{
		COM01=1,
		COM02,
		COM03,
		COM04
	}usart_com;
	
	typedef enum _high_status{
		GIMBAL_PITCH_UP = 0x11,    		//��������
		GIMBAL_PITCH_DOWN = 0x12,
		GIMBAL_YAW_LEFT = 0x13,	  		//��������
		GIMBAL_YAW_RIGHT = 0x14,
		GIMBAL_POSITION = 0x15,					//��̨����
		STOP = 0x16,										//ֹͣ����
		CAMERA_ZOOM = 0x21,					//�����Ŵ���С
		CAMERA_FOCUS = 0x22,				//Զ���۽�
		CAMERA_MODE_CHANGE = 0x23,			//ģʽ�л�  ¼��/����
		CAMERA_PIP_MODE = 0x24,				//���л��л�
		CAMERA_COLOR_CHANGE = 0x25,					//������ɫ�л�
		CAMERA_REC = 0x26,									//��ʼ¼��
		CAMERA_TAKE_PHOTO = 0x27,						//����
		CAMERA_ZOOM_QUERY = 0x28,						//�Ŵ�����ѯ
		GIMBAL_PITCH_ANGLE_UP = 0X17,			  //�������ϽǶ�
		GIMBAL_PITCH_ANGLE_DOWN = 0X18, 		//�������½Ƕ�
		GIMBAL_YAW_ANGLE_LEFT = 0X19,				//������Ƕ�
		GIMBAL_YAW_ANGLE_DOWN = 0x1A,			//�����ҽǶ�
		LANDING_GEAR = 0X31							//�������������
	}action_code;
		
	
	/*** Ʒ����̨���������ָ�� ***/
#if PINLING_GIMBAL_ZOOM18==1    
	
		/*** ��̨���Ʋ��� ***/
		#define RATE_MIN	0x00  //��С�ٶ�
		#define RATE_MAX  0x3F	//����ٶ�
		const unsigned char PITCH_UP_RATE[5] = { 0XFF, 0X01, 0X00, 0X08, 0X00 };  //������Ҫ��������Լ���һ���ٶȣ�һ��У���
		const unsigned char PITCH_DOWN_RATE[5] = { 0XFF, 0X01, 0X00, 0X10, 0X00 };
		
		const unsigned char YAW_LEFT_RATE[4] = { 0XFF, 0X01, 0X00, 0X04 };  //��������������Լ���һ���ٶȣ�һ��00��һ��У���
		const unsigned char YAW_RIGHT_RATE[4] = { 0XFF, 0X01, 0X00, 0X02 };
		
		const unsigned char STOP_ACTION[7] = { 0XFF, 0X01, 0X00, 0X00, 0X00, 0X00, 0X01 };  //ֹͣ��̨�������䱶ֹͣ
		const unsigned char STOP_ACTION_NEW[7] = {0XFF, 0X01, 0X0F, 0X00, 0X00, 0X00, 0X10 }; //�°汾
		
		const unsigned char GIMBAL_RESET[7] = { 0XFF, 0X01, 0X10, 0X00, 0X00, 0X00, 0X11 };   //��̨����
		const unsigned char RESET_GIMBAL_POSITION[5] = { 0X3E, 0X1A, 0X00, 0X1A, 0X00 };     //�����ֶ�������̨�Ļ���λ��
		
		const unsigned char FOLLOW_YAW_DISABLE[11] = { 0X3E, 0X1F, 0X06, 0X25, 0X01, 0X1F, 0X00, 0X00, 0X00, 0X00, 0X20 }; //������ͷģʽ(��̨�������ң�ؿ���)
		const unsigned char FOLLOW_YAW_ENABLE[11] = { 0X3E, 0X1F, 0X06, 0X25, 0X01, 0X1F, 0X01, 0X00, 0X00, 0X00, 0X21 }; //����ģʽ����̨���ŷɻ�ת��
		const unsigned char QUERY_FOLLOW_MODE[7] = { 0X3E, 0X40, 0X02, 0X42, 0X01, 0X1F, 0X20 };  //��̨��ͷģʽ������ģʽ��ѯ
		/*���أ�3E,1F,06,25,01,1F,01,00,00,00,21 -> ����ģʽ     3E,1F,06,25,01,1F,01,00,00,00,20 -> ��ͷģʽ*/
		
		const unsigned char ZOOM_DOWN[7] = { 0XFF, 0X01, 0X00, 0X40, 0X00, 0X00, 0X41 };   //���������С
		const unsigned char ZOOM_UP[7] = { 0XFF, 0X01, 0X00, 0X20, 0X00, 0X00, 0X21 };     //��������Ŵ�
		const unsigned char FOCUS_UP[7] = { 0XFF, 0X01, 0X01, 0X00, 0X00, 0X00, 0X02 };    //Զ��
		const unsigned char FOCUS_DOWN[7] = { 0XFF, 0X01, 0X00, 0X80, 0X00, 0X00, 0X81 };  //����
		
		const unsigned char IMAGE_REC_MODE_CHANGE[7] = { 0XFF, 0X01, 0X00, 0X07, 0X00, 0X67, 0X6F };  	//¼������ģʽ�л�
		const unsigned char IMAGE_RECORD[7] = { 0XFF, 0X01, 0X00, 0X07, 0X00, 0X55, 0X5D };						//¼���������
		const unsigned char QUERY_ZOOM_VALUE[5] = { 0X81, 0X09, 0X04, 0X47, 0XFF };                    //��ѯ�����ǰ�Ŵ���
    /* ���ڷ�������ķŴ�����Ҫ�ʼ���֧�� */		
		
		const unsigned char ANGLE_CONTROL_HEADER[4] = { 0X81, 0X01, 0X06, 0X01 };	//�Ƕȿ���ǰ���ͨ���ֽ� ��������ٶȡ��Ƕ�, ���������ֽںͽ�����
		
	#endif
	
/*** ������̨���������ָ��  SMT18040T ***/
	#if TUOPU_DOUBLE_CAMERA==1
		
		#define SMT18040_FRAME_MAX_LEN 27
		#define SMT18040_HEADER_LEN	3
		#define SMT18040_TARGET_LEN	2
		#define SMT18040_PART_LEN	3
		#define SMT18040_FRAME_DATA_LEN  15
		#define SMT18040_FRAME_CRC_LEN	2
		#define CRC_DATA_LEN (SMT18040_HEADER_LEN+SMT18040_TARGET_LEN+SMT18040_PART_LEN+1+1)
		#define SMT18040_READ 'r'
		#define SMT18040_WRITE	'w' 
		void COPY_DATA(char* dest, char* src, unsigned int len)  
		{  while(len--) 
			 *dest++=*src++;  }
		
		typedef struct _SMT18040_frame
		{
			char header[SMT18040_HEADER_LEN];		//֡ͷ3���ֽ�  #TP����   #tp�ɱ䳤
			char target[SMT18040_TARGET_LEN];			//Ŀ��λ 2���ֽ�
			char len;										//���ݳ���  
			char RW; 		//������д'r' ��  'w' д
			char part[SMT18040_PART_LEN];	//����
			char data[SMT18040_FRAME_DATA_LEN];	//���ݳ���	���15���ֽ�
//			char check[SMT18040_FRAME_CRC_LEN];		//У��λ  2���ֽ�
		}camera_frame;
		
		/*** ��̨���Ʋ��� ***/
		#define RATE_MIN	0x00  //��С�ٶ�
		#define RATE_MAX  0x3F	//����ٶ�		
	
//		const unsigned char IMAGING_MODE[5] = { 0X81, 0X09, 0X04, 0X47, 0XFF };  //�Ⱥ���α��ģʽ
#endif
static char char_hex(char c)
{
	if(c>=16) return 0;
	return "0123456789ABCDEF"[c];
}
		
/***
*   ָ�����ڷ��͵����ֽ�
*
***/
void sendByte(usart_com chan, unsigned char c)
{
	switch(chan)
	{
		case COM01:
		{
			usartSendChar(USART1, c);
			break;
		}
		case COM02:
		{
			usartSendChar(USART2, c);
			break;
		}
		case COM03:
		{
			usartSendChar(USART3, c);
			break;
		}
		case COM04:
		{
			break;
		}
		default: break;
	}
}
		
/***
*   ָ�����ڣ�ָ��ȷ���
*
***/
unsigned char sendControlCode(usart_com chan, const unsigned char *pch, unsigned char len)
{
	unsigned char i = len;
	if( pch==0 && i==0 ) return 0;
	
	for( ; i>0; --i)
	{
		sendByte(chan, *pch);
		++pch;			
	}
	return 1;
}

/***
*   ָ�����鳤�����   //����У���
*
***/
static unsigned char addSum(const unsigned char* pc, unsigned char len)
{
	unsigned int i = 0;
	int sum = 0;
	if(0==pc) return 0;
	
	for( ; i<len; ++i)
	{
		sum += *pc;
		++pc;
	}
	return (unsigned char)sum;
}


/***
*   ������̨ת���Ƕ�ƥ���ٶ� 
*
***/
static unsigned char getAngleSpeed(unsigned char angle)
{
	unsigned char speed;
	
	if(angle>150)  return 0;

	if(angle<=30)  //15�����ڲ����ٶ�15
	{
		speed = 15;
	}else if(angle<=60)  //30�ȵ�60�����ڲ���25
	{
		speed = 25;
	}else if(angle<=90)  //60��90�Ȳ���35
	{
		speed = 35;
	}else if(angle<=150) //90��150����40
	{
		speed = 40;
	}
	return speed;	
}	

/***
*   ������̨ת��ƥ���ٶ� 
*
***/
static unsigned char getSpeed(unsigned char speedSrc)
{
	unsigned char speed;
	
//	if(speedSrc>0x1A)
//	{
//		speed = 0x0F;
//	}else if(speedSrc<=0x1A)
//	{
//		speed = speedSrc;
//	}
	speed = 0x05; 
	return speed;	
}

/***
*   �������� 
*
***/
void copyCharArray(const unsigned char *src, unsigned char *dest, unsigned int count)
{
	unsigned int i;
	const unsigned char *psrc;
	unsigned char *pdest;
	
	if(0==src || 0==dest || 0==count)  return;
	
	i = count;
	psrc = src;
	pdest = dest;
	
	for( ; i>0; --count)
	{
		*pdest = *psrc;
		++psrc;
		++pdest;
	}	
}

	
/***
*   ������̨����ָ�� 
*
***/
void gimbal_control_parse(usart_com chan, unsigned char* highCh, unsigned char* low)
{
	static unsigned char recStatus = 1;         //¼���ж�ָ��, 1Ϊ����¼�� 0Ϊֹͣ¼��
	static unsigned char pitchUpStatus = 0;
	static unsigned char pitchDownStatus = 0;
	static unsigned char yawLeftStatus = 0;
	static unsigned char yawRightStatus = 0; 
	
//	if(100==saveCount) saveCount=0;   //����������Ѿ����أ����¿�ʼ���棬֮ǰ�����ݻᱻ���ǵ�
	
	switch(*highCh)
	{
		case	GIMBAL_PITCH_ANGLE_UP:			//�������ϽǶ�
		{
			
			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
			sendByte( chan, getAngleSpeed((*low) ));  //�ٶ�
			sendByte( chan, *low );   //�Ƕ�
			sendByte(chan, 0x03);
			sendByte(chan, 0x01);
			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_PITCH_ANGLE_DOWN:  		//�������½Ƕ�
		{
			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
			sendByte( chan, getAngleSpeed((*low)));  //�ٶ�
			sendByte( chan, *low );   //�Ƕ�
			sendByte(chan, 0x03);
			sendByte(chan, 0x02);
			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_YAW_ANGLE_LEFT: 				//������Ƕ�
		{
			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
			sendByte( chan, getAngleSpeed((*low)) );  //�ٶ�
			sendByte( chan, *low );   //�Ƕ�
			sendByte(chan, 0x01);
			sendByte(chan, 0x03);
			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_YAW_ANGLE_DOWN: 		//�����ҽǶ�
		{
			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
			sendByte( chan, getAngleSpeed((*low)) );  //�ٶ�
			sendByte( chan, *low );   //�Ƕ�
			sendByte(chan, 0x02);
			sendByte(chan, 0x03);
			sendByte(chan, 0xff);
			break;
		}
		case LANDING_GEAR:                //���������  
		{
			if(*low==0x01)									//��
			{
				IWDG_Feed();
//				xSemaphoreGive(xSemaphore_landing_gear);
//				xQueueSendToBack(landingQueueHandle, low, 10/portMAX_DELAY);
			}else if(*low==0x02)						//��
			{
				IWDG_Feed();
//				xSemaphoreGive(xSemaphore_landing_gear);
//				xQueueSendToBack(landingQueueHandle, low, 10/portMAX_DELAY);
			}else
			{
				
			}
			break;
		}
		
		case GIMBAL_PITCH_UP:    		//��������
		{
			pitchUpStatus = 1;
			sendControlCode(chan, &PITCH_UP_RATE[0], sizeof(PITCH_UP_RATE)/sizeof(PITCH_UP_RATE[0]));
			sendByte( chan, getSpeed(*low) );
			sendByte( chan, addSum(&PITCH_UP_RATE[1], 4) + getSpeed(*low));		
			break;
		}
		case GIMBAL_PITCH_DOWN:
		{
			pitchDownStatus = 1;
			sendControlCode(chan, &PITCH_DOWN_RATE[0], sizeof(PITCH_DOWN_RATE)/sizeof(PITCH_DOWN_RATE[0]));
			sendByte( chan, getSpeed(*low) );
			sendByte( chan, addSum(&PITCH_DOWN_RATE[1], 4) + getSpeed(*low));

			break;
		}
		case GIMBAL_YAW_LEFT:	  		//��������
		{
			yawLeftStatus = 1;

			sendControlCode(chan, &YAW_LEFT_RATE[0], sizeof(YAW_LEFT_RATE)/sizeof(YAW_LEFT_RATE[0]));
			sendByte( chan, getSpeed(*low));
			sendByte( chan, 0);
			sendByte( chan, addSum(&YAW_LEFT_RATE[1], 3) + getSpeed(*low) );

			break;
		}
		case GIMBAL_YAW_RIGHT:
		{
			yawRightStatus = 1;
			if(yawRightStatus<=4)
			{				
				sendControlCode(chan, &YAW_RIGHT_RATE[0], sizeof(YAW_RIGHT_RATE)/sizeof(YAW_RIGHT_RATE[0]));
				sendByte( chan, getSpeed(*low) );
				sendByte( chan, 0);
				sendByte( chan, addSum(&YAW_RIGHT_RATE[1], 3) + getSpeed(*low) );
			}
			break;
		}
		case GIMBAL_POSITION:					
		{
			if(*low==1)    					//��̨����
			{
				sendControlCode(chan, &GIMBAL_RESET[0], sizeof(GIMBAL_RESET)/sizeof(GIMBAL_RESET[0]));
			}else if(*low==2)				//�ֶ����û���λ��
			{
				sendControlCode(chan, &RESET_GIMBAL_POSITION[0], sizeof(RESET_GIMBAL_POSITION)/sizeof(RESET_GIMBAL_POSITION[0]));
			}else if(*low==3)				//����Ϊ��ͷģʽ
			{
				sendControlCode(chan, &FOLLOW_YAW_DISABLE[0], sizeof(FOLLOW_YAW_DISABLE)/sizeof(FOLLOW_YAW_DISABLE[0]));										
			}else if(*low==4)				//����Ϊ����ģʽ
			{
				sendControlCode(chan, &FOLLOW_YAW_ENABLE[0], sizeof(FOLLOW_YAW_ENABLE)/sizeof(FOLLOW_YAW_ENABLE[0]));
			}else if(*low==5)				//��̨ģʽ��ѯ			
			{
				sendControlCode(chan, &QUERY_FOLLOW_MODE[0], sizeof(QUERY_FOLLOW_MODE)/sizeof(QUERY_FOLLOW_MODE[0]));
			}else
		  {
				
			}				
			break;
		}
		case STOP:										//ֹͣ����
		{
			if(*low==1)
			{
//				sendControlCode(chan, &STOP_ACTION[0], sizeof(STOP_ACTION)/sizeof(STOP_ACTION[0]));
//				xTimerStop( xTimerUser, 100 );
				xTimerStart( xTimerUser, 100 );
				pitchUpStatus = 0;
				pitchDownStatus = 0;
				yawLeftStatus = 0;
				yawRightStatus = 0; 
			}
			break;
		}
	  case CAMERA_ZOOM:					//�����Ŵ���С
		{
			if(*low==1)							//������С
			{
				sendControlCode(chan, &ZOOM_DOWN[0], sizeof(ZOOM_DOWN)/sizeof(ZOOM_DOWN[0]));
			}else if(*low==2)				//�����Ŵ�
			{
				sendControlCode(chan, &ZOOM_UP[0], sizeof(ZOOM_UP)/sizeof(ZOOM_UP[0]));
			}else{
			
			}
			break;
		}
		case CAMERA_FOCUS:				//Զ���۽�
		{
			if(*low==1)							//��
			{
				sendControlCode(chan, &FOCUS_DOWN[0], sizeof(FOCUS_DOWN)/sizeof(FOCUS_DOWN[0]));
			}else if(*low==2)				//Զ
			{
				sendControlCode(chan, &FOCUS_UP[0], sizeof(FOCUS_UP)/sizeof(FOCUS_UP[0]));
			}else{
			
			}
			break;
		}

		case CAMERA_MODE_CHANGE:			//ģʽ�л�  ¼��/����
		{
			if(*low==1)
			{
				sendControlCode(chan, &IMAGE_REC_MODE_CHANGE[0], sizeof(IMAGE_REC_MODE_CHANGE)/sizeof(IMAGE_REC_MODE_CHANGE[0]));
			}
			break;
		}
		case CAMERA_PIP_MODE:				//���л��л�
		{
			if(*low==1)
			{
				sendControlCode(chan, &IMAGE_REC_MODE_CHANGE[0], sizeof(IMAGE_REC_MODE_CHANGE)/sizeof(IMAGE_REC_MODE_CHANGE[0]));
			}
			break;
		}
		case CAMERA_COLOR_CHANGE:		//������ɫ�л�
		{
			if(*low==1)
			{
				//sendControlCode(chan, &IMAGING_MODE[0], sizeof(IMAGING_MODE)/sizeof(IMAGING_MODE[0]));
				sendControlCode(chan, &QUERY_ZOOM_VALUE[0], sizeof(QUERY_ZOOM_VALUE)/sizeof(QUERY_ZOOM_VALUE[0]));
			}
			break;
		}
		case CAMERA_REC:				//��ʼ¼��
		{
			if(*low==1)						
			{
				
			}else if(*low==2&&recStatus==1)		 //��ʼ¼��		
			{
				recStatus = 0;
				sendControlCode(chan, &IMAGE_RECORD[0], sizeof(IMAGE_RECORD)/sizeof(IMAGE_RECORD[0]));
			}else if(*low==3&&recStatus==0)		 //ֹͣ¼��
			{
				recStatus = 1;
				sendControlCode(chan, &IMAGE_RECORD[0], sizeof(IMAGE_RECORD)/sizeof(IMAGE_RECORD[0]));
			}else
			{
			
			}
			break;
		}
		case CAMERA_TAKE_PHOTO:			//����
		{
			if(*low==2)
			{
				sendControlCode(chan, &IMAGE_RECORD[0], sizeof(IMAGE_RECORD)/sizeof(IMAGE_RECORD[0]));
			}
			break;
		}
		case CAMERA_ZOOM_QUERY:			//�Ŵ�����ѯ
		{
			if(*low==1)
			{
				sendControlCode(chan, &QUERY_ZOOM_VALUE[0], sizeof(QUERY_ZOOM_VALUE)/sizeof(QUERY_ZOOM_VALUE[0]));
			}
			break;
		}
		default: break;
	}
}
//end function

/* ˫�����̨����ָ�� */
void double_send_code(usart_com chan, char* header, char* target, char len,  char r_w, char* part, char* data)
{
	char frame[SMT18040_FRAME_MAX_LEN];
	char* pc = frame;
	unsigned int crc = 0;
	unsigned char crc_len =0;
	if(0==header || 0==target) return;
	COPY_DATA(pc,header,SMT18040_HEADER_LEN);  //����֡ͷ
	pc += SMT18040_HEADER_LEN;
	
	COPY_DATA(pc,target,SMT18040_TARGET_LEN);  //����Ŀ��
	pc += SMT18040_TARGET_LEN;
	
	*pc = char_hex(len);
	++pc;
	
	*pc = r_w;
	++pc;
	
	COPY_DATA(pc,part,SMT18040_PART_LEN); //��ʶ
	pc += SMT18040_PART_LEN;
		
	COPY_DATA(pc, data, len);//����
	pc += len;
	
	for(crc_len=0; crc_len<CRC_DATA_LEN+len; ++crc_len)
	{
		crc += frame[crc_len];
	}
	*pc++ = char_hex(crc>>4 & 0x0f);
	*pc = char_hex(crc & 0x0f);
//	sendControlCode(chan, frame, CRC_DATA_LEN+len+SMT18040_FRAME_CRC_LEN);   //�򴮿ڷ�������
	insertDataToBuff(chan, frame, CRC_DATA_LEN+len+SMT18040_FRAME_CRC_LEN);
}

/***
*  ˫�������̨����
*
***/
void double_camera_control_parse(usart_com chan, unsigned char* highCh, unsigned char* low)
{
	static unsigned char recStatus = 1;         //¼���ж�ָ��, 1Ϊ����¼�� 0Ϊֹͣ¼��
	static unsigned char pitchUpStatus = 0;
	static unsigned char pitchDownStatus = 0;
	static unsigned char yawLeftStatus = 0;
	static unsigned char yawRightStatus = 0; 
	switch(*highCh)
	{
		case	GIMBAL_PITCH_ANGLE_UP:			//�������ϽǶ�
		{
//			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
//			sendByte( chan, getAngleSpeed((*low) ));  //�ٶ�
//			sendByte( chan, *low );   //�Ƕ�
//			sendByte(chan, 0x03);
//			sendByte(chan, 0x01);
//			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_PITCH_ANGLE_DOWN:  		//�������½Ƕ�
		{
//			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
//			sendByte( chan, getAngleSpeed((*low)));  //�ٶ�
//			sendByte( chan, *low );   //�Ƕ�
//			sendByte(chan, 0x03);
//			sendByte(chan, 0x02);
//			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_YAW_ANGLE_LEFT: 				//������Ƕ�
		{
//			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
//			sendByte( chan, getAngleSpeed((*low)) );  //�ٶ�
//			sendByte( chan, *low );   //�Ƕ�
//			sendByte(chan, 0x01);
//			sendByte(chan, 0x03);
//			sendByte(chan, 0xff);
			break;
		}
		case GIMBAL_YAW_ANGLE_DOWN: 		//�����ҽǶ�
		{
//			sendControlCode(chan, &ANGLE_CONTROL_HEADER[0], sizeof(ANGLE_CONTROL_HEADER)/sizeof(ANGLE_CONTROL_HEADER[0]));
//			sendByte( chan, getAngleSpeed((*low)) );  //�ٶ�
//			sendByte( chan, *low );   //�Ƕ�
//			sendByte(chan, 0x02);
//			sendByte(chan, 0x03);
//			sendByte(chan, 0xff);
			break;
		}
		case LANDING_GEAR:                //���������  
		{
			if(*low==0x01)									//��
			{
//				xSemaphoreGive(xSemaphore_landing_gear);
//				xQueueSendToBack(landingQueueHandle, low, 10/portMAX_DELAY);
			}else if(*low==0x02)						//��
			{
//				xSemaphoreGive(xSemaphore_landing_gear);
//				xQueueSendToBack(landingQueueHandle, low, 10/portMAX_DELAY);
			}else
			{
				
			}
			break;
		}
		
		case GIMBAL_PITCH_UP:    		//��������
		{
			++pitchUpStatus;
			if(pitchUpStatus<=4)
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "01");
			}			
			break;
		}
		case GIMBAL_PITCH_DOWN:
		{
			++pitchDownStatus;
			if(pitchDownStatus<=4)
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "02");
			}
			break;
		}
		case GIMBAL_YAW_LEFT:	  		//��������
		{
			++yawLeftStatus;
			if(yawLeftStatus<=4)
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "03");
			}
			break;
		}
		case GIMBAL_YAW_RIGHT:
		{
			++yawRightStatus;
			if(yawRightStatus<=4)
			{			
					double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "04");				
			}
			break;
		}
		case GIMBAL_POSITION:					
		{
			if(*low==1)    					//��̨����
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "05");
			}else if(*low==2)				//�ֶ����û���λ��
			{
//				sendControlCode(chan, &RESET_GIMBAL_POSITION[0], sizeof(RESET_GIMBAL_POSITION)/sizeof(RESET_GIMBAL_POSITION[0]));
			}else if(*low==3)				//����Ϊ��ͷģʽ
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "07");									
			}else if(*low==4)				//����Ϊ����ģʽ
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "06");;
			}else if(*low==5)				//��̨ģʽ��ѯ			
			{
//				sendControlCode(chan, &QUERY_FOLLOW_MODE[0], sizeof(QUERY_FOLLOW_MODE)/sizeof(QUERY_FOLLOW_MODE[0]));
			}else
		  {
				
			}				
			break;
		}
		case STOP:										//ֹͣ����
		{
			if(*low==1)
			{
				double_send_code(chan,"#TP", "UP", 2, SMT18040_WRITE, "PTZ", "00");  //ֹͣ��̨����
				double_send_code(chan,"#tp", "UM", 8, SMT18040_WRITE, "HIT", ":WFCBBFE66");  //�۽����䱶ֹͣ
				pitchUpStatus = 0;
				pitchDownStatus = 0;
				yawLeftStatus = 0;
				yawRightStatus = 0; 
			}
			break;
		}
	  case CAMERA_ZOOM:					//�����Ŵ���С
		{
			if(*low==1)							//������С
			{
				double_send_code(chan,"#tp", "UM", 8, SMT18040_WRITE, "HIT", ":WFCBB9B56");
			}else if(*low==2)				//�����Ŵ�
			{
				double_send_code(chan,"#tp", "UM", 8, SMT18040_WRITE, "HIT", ":WFCBB994D");
			}else{
			
			}
			break;
		}
		case CAMERA_FOCUS:				//Զ���۽�
		{
			if(*low==1)							//��
			{
				double_send_code(chan,"#tp", "UM", 8, SMT18040_WRITE, "HIT", ":WFCBBAA5D");
			}else if(*low==2)				//Զ
			{
				double_send_code(chan,"#tp", "UM", 8, SMT18040_WRITE, "HIT", ":WFCBBA955");
			}else{
			
			}
			break;
		}

		case CAMERA_MODE_CHANGE:			//ģʽ�л�  ¼��/����
		{
			if(*low==1)
			{
				static unsigned char pipMode = 0;
				char data[2];
				if(pipMode>7)	pipMode = 0;
				data[0] = char_hex(0);
				data[1] = char_hex(pipMode&0x0f);
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "AWB", &data[0]);
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "EVS", &data[0]);
				++pipMode;
//				sendControlCode(chan, &IMAGE_REC_MODE_CHANGE[0], sizeof(IMAGE_REC_MODE_CHANGE)/sizeof(IMAGE_REC_MODE_CHANGE[0]));
			}
			break;
		}
		case CAMERA_PIP_MODE:				//���л��л�
		{
			if(*low==1)
			{
				static unsigned char pipMode = 0;
				char data[2];
				if(pipMode>3)	pipMode = 0;
				data[0] = char_hex(0);
				data[1] = char_hex(pipMode&0x0f);
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "PIP", &data[0]);
				++pipMode;
			}
			break;
		}
		case CAMERA_COLOR_CHANGE:		//������ɫ�л�
		{
			if(*low==1)
			{
				static unsigned char colorMode = 0;
				char data[2];
				if(colorMode>0x09)	colorMode = 0;
				data[0] = char_hex(colorMode>>4&0x0f);
				data[1] = char_hex(colorMode&0x0f);
				double_send_code(chan,"#TP", "UE", 2, SMT18040_WRITE, "IMG", &data[0]);
//				double_send_code(chan,"#TP", "UE", 2, SMT18040_WRITE, "DZM", &data[0]);
				++colorMode;
			}
			break;
		}
		case CAMERA_REC:				//��ʼ¼��
		{
			if(*low==1)						
			{
				
			}else if(*low==2&&recStatus==1)		 //��ʼ¼��		
			{
				recStatus = 0;
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "REC", "11");
			}else if(*low==3&&recStatus==0)		 //ֹͣ¼��
			{
				recStatus = 1;
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "REC", "00");
			}else
			{
			
			} 
			break;
		}
		case CAMERA_TAKE_PHOTO:			//����
		{
			if(*low==2)
			{
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "CAP", "11");
				double_send_code(chan,"#TP", "UD", 2, SMT18040_WRITE, "CAP", "NN");
//				sendControlCode(chan, &IMAGE_RECORD[0], sizeof(IMAGE_RECORD)/sizeof(IMAGE_RECORD[0]));
			}
			break;
		}
		case CAMERA_ZOOM_QUERY:			//�Ŵ�����ѯ
		{
			if(*low==1)
			{
//				sendControlCode(chan, &QUERY_ZOOM_VALUE[0], sizeof(QUERY_ZOOM_VALUE)/sizeof(QUERY_ZOOM_VALUE[0]));
			}
			break;
		}
		default: break;
	}
}		
//end function

#endif
