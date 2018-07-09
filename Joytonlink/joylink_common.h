
#pragma once

#ifndef JOYTONLINK_COMMON_H
#define JOYTONLINK_COMMON_H

#include "usart.h"

typedef enum {
	JOYLINK_COMM_1,
	JOYLINK_COMM_2,
	JOYLINK_COMM_3
} joylink_channel;

#define JOYLINK_COMM_NUM	3
	
typedef enum {
    JOYLINK_PARSE_STATE_UNINIT=0,
    JOYLINK_PARSE_STATE_IDLE,
    JOYLINK_PARSE_STATE_GOT_STX,
    JOYLINK_PARSE_STATE_GOT_SEQ,
	  JOYLINK_PARSE_STATE_GOT_DEVID,
    JOYLINK_PARSE_STATE_GOT_MSGID,
    JOYLINK_PARSE_STATE_GOT_LENGTH,
    JOYLINK_PARSE_STATE_GOT_PAYLOAD,
    JOYLINK_PARSE_STATE_GOT_BAD_CRC
} joylink_parse_state; //The state machine for the comm parser

typedef enum {
    JOYLINK_FRAMING_INCOMPLETE=0,
    JOYLINK_FRAMING_OK=1,
    JOYLINK_FRAMING_BAD_CRC=2
} joylink_framing;
	

typedef struct __joylink_status {
		uint8_t msg_received;               ///< Number of received messages    
		uint8_t buffer_overrun;             ///< Number of buffer overruns      
		uint8_t parse_error;                ///< Number of parse errors					
		joylink_parse_state parse_state;  ///< Parsing state machine					
		uint8_t packet_idx;                 ///< Index in current packet				
		uint8_t current_rx_seq;             ///< Sequence number of last packet received  
		uint8_t current_tx_seq;             ///< Sequence number of last packet sent       
		uint16_t packet_rx_success_count;   ///< Received packets							
		uint16_t packet_rx_drop_count;      ///< Number of packet drops				
} joylink_status;

/*  ���ڵײ㺯��ʵ��,�û����Ӳ������ʵ�� */
void joylink_comm_send_ch(joylink_channel chan, uint8_t ch);
void joylink_send_uart(joylink_channel chan, const char *buf, uint16_t len);
//end

uint8_t board_dev_id = 0xBD;
		
/* ����У���� */
inline void joylink_checksum(unsigned char* check_sum, const unsigned char* buff, unsigned char len)
{
	unsigned char i = 0;
	const unsigned char* pbuff = buff;
	if(0==pbuff || 0==len || 0==check_sum) return;
	for( ; i<len; ++i)
	{
		*check_sum ^= *pbuff;
		++pbuff;
	}
}
//end
	
/* ����ͨ����Ӧ���ݴ����һЩ״̬��־ */
static inline joylink_status* joylink_get_channel_status(uint8_t chan)
{
	static joylink_status e_joylink_status[JOYLINK_COMM_NUM]; /*����ͨ��״̬��־���ݣ��ṹ��*/
	return &e_joylink_status[chan];
}
//end

static inline joylink_message* joylink_get_channel_buffer(uint8_t chan)
{
	static joylink_message e_joylink_buffer[JOYLINK_COMM_NUM];
	return &e_joylink_buffer[chan];
}

static inline void joylink_reset_channel_status(uint8_t chan)
{
	joylink_status *status = joylink_get_channel_status(chan);
	status->parse_state = JOYLINK_PARSE_STATE_IDLE;
}

static inline uint16_t joylink_finalize_message_chan(joylink_message* msg, uint8_t dev_id, uint8_t chan, uint8_t length)
{
	uint8_t ck = 0;
	msg->stx = JOYTONLINK_STX;
	msg->len = length;
	msg->devid = dev_id;
	msg->seq = joylink_get_channel_status(chan)->current_tx_seq;
	joylink_get_channel_status(chan)->current_tx_seq = joylink_get_channel_status(chan)->current_tx_seq+1; /*����֡���������ֵ255*/
	joylink_checksum(&ck, ((uint8_t*)msg)+1, 4);
	joylink_checksum(&ck, (uint8_t*)(&(msg->package[0])), msg->len);
	msg->crc = ck;
	
	return length + 6;
}

static inline void joylink_finalize_message_chan_send(joylink_channel chan, uint8_t msgid, const char *packet, uint8_t length)
{
	uint8_t buf[5];
	uint8_t ck = 0;
	joylink_status *status = joylink_get_channel_status(chan);
	buf[0] = JOYTONLINK_STX;
	buf[1] = status->current_tx_seq;
	buf[2] = board_dev_id;
	buf[3] = msgid;
	buf[4] = length;	
	status->current_tx_seq++;
	
	joylink_checksum(&ck, &buf[1], 4);
	joylink_checksum(&ck, (const unsigned char*)packet, length);

	joylink_send_uart(chan, (const char *)buf, 5);
	joylink_send_uart(chan, packet, length);
	joylink_send_uart(chan, (const char *)(&ck), 1);
}


static inline uint16_t joylink_finalize_message(joylink_message* msg, uint8_t dev_id, uint8_t length)
{
	return joylink_finalize_message_chan(msg, dev_id, JOYLINK_COMM_1, length);
}

static inline void joylink_resend_uart(joylink_channel chan, const joylink_message *msg)
{
	uint8_t ck = 0;
	ck = msg->crc;
	
	joylink_send_uart(chan, (const char *)&msg->stx, 5);
	joylink_send_uart(chan, (const char *)(&((msg)->package[0])), msg->len);
	joylink_send_uart(chan, (const char *)ck, 1);
}

static inline uint8_t joylink_frame_char_buffer(joylink_message* rxmsg, joylink_status* status, uint8_t c, joylink_message* r_message,  joylink_status* r_joylink_status)
{
	int bufferIndex = 0;
	static uint8_t crc_value = 0;

	status->msg_received = JOYLINK_FRAMING_INCOMPLETE;      //������־λ��������...  

	switch (status->parse_state)    //�������������֡����һ����   
	{
		/*	STX	*/
		case JOYLINK_PARSE_STATE_UNINIT:
		case JOYLINK_PARSE_STATE_IDLE:		//û�л����ʼλ֮ǰ��λ����״̬
			if (c == JOYTONLINK_STX)
			{
				status->parse_state = JOYLINK_PARSE_STATE_GOT_STX;		//��ȡ����ʼλ
				rxmsg->len = 0;
				rxmsg->stx = c;																			//��ֵ��ʼλ
			}
			break;
		
		/*	SEQ	*/
		case JOYLINK_PARSE_STATE_GOT_STX:
			rxmsg->seq = c;
			crc_value = 0;
			crc_value ^= c;
			status->parse_state = JOYLINK_PARSE_STATE_GOT_SEQ;
			break;
	
		/*	DEVID	*/
		case JOYLINK_PARSE_STATE_GOT_SEQ:
			rxmsg->devid = c;
			crc_value ^= c;
			status->parse_state = JOYLINK_PARSE_STATE_GOT_DEVID;
			break;
	
		/*	MSGID	*/
		case JOYLINK_PARSE_STATE_GOT_DEVID:
			rxmsg->msgid = c;
			crc_value ^= c;
			status->parse_state = JOYLINK_PARSE_STATE_GOT_MSGID;
			break;
		
		/* LENGTH */
		case JOYLINK_PARSE_STATE_GOT_MSGID:					
			if (status->msg_received || c > JOYTONLINK_DATA_MAX_LEN)
			{
				status->buffer_overrun++;
				status->parse_error++;
				status->msg_received = 0;
				status->parse_state = JOYLINK_PARSE_STATE_IDLE;
			}
			else
			{
				rxmsg->len = c;
				status->packet_idx = 0;							//��Ч���ݼ���
				crc_value ^= c;
				status->parse_state = JOYLINK_PARSE_STATE_GOT_LENGTH;
			}
			break;	

			/* DATA */
		case JOYLINK_PARSE_STATE_GOT_LENGTH:
			((char *)(&((rxmsg)->package[0])))[status->packet_idx++] = (char)c;   //�洢һ���ֽ�����
			crc_value ^= c;
			if (status->packet_idx == rxmsg->len)   //��ȡ��len�ȳ�������
			{
				status->parse_state = JOYLINK_PARSE_STATE_GOT_PAYLOAD;
			}
			break;
		
		/*  Check */
		case JOYLINK_PARSE_STATE_GOT_PAYLOAD:
			if (c!=crc_value) 
			{
				status->msg_received = JOYLINK_FRAMING_BAD_CRC;  //У�����
			}
			else 
			{
				status->msg_received = JOYLINK_FRAMING_OK;
				rxmsg->crc = crc_value;
			}
			status->parse_state = JOYLINK_PARSE_STATE_IDLE;
			memcpy(r_message, rxmsg, sizeof(joylink_message));    //�����յ���Ϣ֡���Ƶ�r_message��
			break;
	}

	bufferIndex++;	//�Ѿ��������ֽڸ������ڸú�����û���õ��ò���
	// If a message has been sucessfully decoded, check index
	if (status->msg_received == JOYLINK_FRAMING_OK)
	{
		status->current_rx_seq = rxmsg->seq;		  //��ǰ���յ�����Ϣ����
		if (status->packet_rx_success_count == 0) 
			status->packet_rx_drop_count = 0;
		
		status->packet_rx_success_count++;				//�����ɹ�����Ϣ����
	}

	r_message->len = rxmsg->len; // Provide visibility on how far we are into current msg
	r_joylink_status->parse_state = status->parse_state;
	r_joylink_status->packet_idx = status->packet_idx;
	r_joylink_status->current_rx_seq = status->current_rx_seq+1;
	r_joylink_status->packet_rx_success_count = status->packet_rx_success_count;
  r_joylink_status->packet_rx_drop_count = status->parse_error;
	status->parse_error = 0;  

	return status->msg_received;
}

static inline uint8_t joylink_frame_char(uint8_t chan, uint8_t c, joylink_message* r_message, joylink_status* r_joylink_status)
{
	return joylink_frame_char_buffer(joylink_get_channel_buffer(chan),
					 joylink_get_channel_status(chan),
					 c,
					 r_message,
					 r_joylink_status);
}

static inline uint8_t joylink_parse_char(uint8_t chan, uint8_t c, joylink_message* r_message, joylink_status* r_joylink_status)
{
    uint8_t msg_received = joylink_frame_char(chan, c, r_message, r_joylink_status);
    if (msg_received == JOYLINK_FRAMING_BAD_CRC) {
	    // we got a bad CRC. Treat as a parse failure
	    joylink_message* rxmsg = joylink_get_channel_buffer(chan);
	    joylink_status* status = joylink_get_channel_status(chan);
	    status->parse_error++;
	    status->msg_received = JOYLINK_FRAMING_INCOMPLETE;
	    status->parse_state = JOYLINK_PARSE_STATE_IDLE;
	    if (c == JOYTONLINK_STX)
	    {
		    status->parse_state = JOYLINK_PARSE_STATE_GOT_STX;
		    rxmsg->len = 0;
	    }
	    return 0;
    }
    return msg_received;
}


static inline void joylink_comm_send_ch(joylink_channel chan, uint8_t ch)  //add by Awesome
{
	switch(chan)
	{
		case JOYLINK_COMM_1:
		{
			//usartSendChar(USART1, ch);
			break;
		}
		case JOYLINK_COMM_2:
		{
			usartSendChar(USART2, ch);
			break;
		}
		case JOYLINK_COMM_3:
		{
			usartSendChar(USART3, ch);
			break;
		}
//		case MAVLINK_COMM_3:
//		{
//			usartSendChar(USART4, ch);
//			break;
//		}	
		default: break;
	}
}

inline void joylink_send_uart(joylink_channel chan, const char *buf, uint16_t len)
{

	uint16_t i;
	for (i = 0; i < len; i++) {
		joylink_comm_send_ch(chan, (uint8_t)buf[i]);
	}
//	insertDataToBuff(chan, buf, len);
}

/*
  like memcpy(), but if src is NULL, do a memset to zero
*/
inline void joy_array_memcpy(void *dest, const void *src, size_t n)
{
	if (src == NULL) {
		memset(dest, 0, n);
	} else {
		memcpy(dest, src, n);
	}
}


#define _JOY_PAYLOAD(msg) ((const char *)(&((msg)->package[0])))
#define _JOY_PAYLOAD_NON_CONST(msg) ((char *)(&((msg)->package[0])))

#define JOY_MSG_RETURN_TYPE(TYPE) \
static inline TYPE JOY_RETURN_## TYPE(const joylink_message *msg, uint8_t ofs) \
{ return *(const TYPE *)(&_JOY_PAYLOAD(msg)[ofs]);}

JOY_MSG_RETURN_TYPE(int8_t)
JOY_MSG_RETURN_TYPE(uint8_t)
JOY_MSG_RETURN_TYPE(uint16_t)
JOY_MSG_RETURN_TYPE(int16_t)
JOY_MSG_RETURN_TYPE(uint32_t)
JOY_MSG_RETURN_TYPE(int32_t)
JOY_MSG_RETURN_TYPE(uint64_t)
JOY_MSG_RETURN_TYPE(int64_t)
JOY_MSG_RETURN_TYPE(float)
JOY_MSG_RETURN_TYPE(double)

#define JOY_RETURN_ARRAY(TYPE, V)					\
static inline uint16_t JOY_RETURN_## TYPE ##_array(const joylink_message *msg, TYPE *value, \
							 uint8_t array_length, uint8_t wire_offset) \
{ \
	memcpy(value, &_JOY_PAYLOAD(msg)[wire_offset], array_length*sizeof(TYPE)); \
	return array_length*sizeof(TYPE); \
}

JOY_RETURN_ARRAY(uint16_t, u16)
JOY_RETURN_ARRAY(uint32_t, u32)
JOY_RETURN_ARRAY(uint64_t, u64)
JOY_RETURN_ARRAY(int16_t,  i16)
JOY_RETURN_ARRAY(int32_t,  i32)
JOY_RETURN_ARRAY(int64_t,  i64)
JOY_RETURN_ARRAY(float,    f)
JOY_RETURN_ARRAY(double,   d)
#endif
