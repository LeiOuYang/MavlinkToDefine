
#ifndef GIMALBAL_CONTROL_H
#define GIMALBAL_CONTROL_H

#define uchar unsigned char 

#define GROUND_STATION_CODE_ID  169  

#define GIMAL_CONTROL_STX 0xFF     //����֡��ʼλ


/***	
*	 ��̨��������֡�ṹ
*
*/
typedef struct __gimbal_control_message {
	
	uchar stx;						//��ʼ����֡
	uchar general1;				//ͨ�ò���1
	uchar general2; 			//ͨ�ò���2
	uchar general3;		//����ָ�����	
	uchar action_param1;	//��������1	
	uchar action_param2;	//��������2
  uchar add_result;			//�ڶ������������������������ݺ�
	
} gimbal_control_message_t;

/***	
*	 ��̨���ƴ���ͨ��ѡ��
*
*/
typedef enum {
	GIMBAL_CONTROL_COMM_1,
	GIMBAL_CONTROL_COMM_2,
	GIMBAL_CONTROL_COMM_3,
} gimbal_control_channel_t;

/***	
*	 ���ݽ���״̬��־
*
*/
typedef enum {
	GIMBAL_PARSE_STATE_IDLE,
	GIMBAL_PARSE_STATE_GOT_STX,
	GIMBAL_PARSE_STATE_GOT_GENERAL1,
	GIMBAL_PARSE_STATE_GOT_GENERAL2,
} gimbal_control_parse_state_t;///< The state machine for the comm parser

/***	
*	 �������״̬��־
*
*/
typedef enum {
	GIMBAL_CONTROL_FRAMING_INCOMPLETE=0,   //������
	GIMBAL_CONTROL_FRAMING_OK=1,						//�����ɹ�
	GIMBAL_CONTROL_FRAMING_OK1=2,						//�����ɹ�
	GIMBAL_CONTROL_FRAMING_BAD=3,					//��������
} gimbal_control_framing_t;

/***	
*	 ʶ�𵽵Ķ���ָ��
*
*/
//typedef enum {
//	ZOOM_DOWN=0,
//	ZOOM_UP,
//	FOCUS_DOWM,
//	FOCUS_UP,
//	ZOOM_FOCUS_STOP,
//	REC_REVER,
//	DIP_MODE,
//	INFRARED_IMAGE_MODE,
//	PITCH_UP,
//	PITCH_DOWN,
//	YAW_LEFT,
//	YAW_RIGHT,
//	GIMALBAL_STOP,
//	GIMALBAL_RETURN��
//	NONE
//} gimbal_control_action;

static uchar checksum(uchar uc, uchar clear);
gimbal_control_framing_t gimbal_frame_char_buffer(uchar *buff, gimbal_control_parse_state_t* parse_state,  uchar c);

#endif
