
#pragma once

#define JOYLINK_MSG_ID_GPS_POSITION 33

typedef struct _joylink_gps_position{
 uint32_t time_boot_ms; 	/* ϵͳ����ʱ�� ���� */
 int32_t lat; 						/* γ�ȣ�ֵΪγ��*10��7�η� */
 int32_t lon; 						/* ���ȣ�ֵΪ����*10��7�η*/
 int32_t alt; 						/* ƽ�����θ߶ȣ���λ���� */
 int32_t relative_alt; 		/* ��Ե���ĸ߶ȣ� ��λ���� */
 int16_t vx; 							/* ��Դ��x������ٶ�(��������Ϊ��  cm/s) */
 int16_t vy; 							/* ��Դ��Y������ٶ�(��������Ϊ��  cm/s )*/
 int16_t vz; 							/* ��Դ��Z������ٶ�(��ֱ��������Ϊ��  cm/s)*/
 uint16_t hdg; 						/* �����ĺ���Ƕ�*100 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
}joylink_gps_position;

#define JOYLINK_MSG_ID_GPS_POSITION_LEN 28


static inline uint16_t joylink_msg_gps_position_pack(uint8_t dev_id, joylink_message* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t alt, int32_t relative_alt, int16_t vx, int16_t vy, int16_t vz, uint16_t hdg)
{
    joylink_gps_position packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt = alt;
    packet.relative_alt = relative_alt;
    packet.vx = vx;
    packet.vy = vy;
    packet.vz = vz;
    packet.hdg = hdg;

    memcpy(_JOY_PAYLOAD_NON_CONST(msg), &packet, JOYLINK_MSG_ID_GPS_POSITION_LEN);

    msg->msgid = JOYLINK_MSG_ID_GPS_POSITION;
    return joylink_finalize_message(msg, dev_id, JOYLINK_MSG_ID_GPS_POSITION_LEN);
}

static inline void joylink_msg_gps_position_send_struct(joylink_channel chan, const joylink_gps_position* gps_position)
{
    joylink_finalize_message_chan_send(chan, JOYLINK_MSG_ID_GPS_POSITION, (const char *)gps_position, JOYLINK_MSG_ID_GPS_POSITION_LEN);
}

static inline uint32_t joylink_msg_gps_position_get_time_boot_ms(const joylink_message* msg)
{
    return JOY_RETURN_uint32_t(msg,  0);
}

static inline int32_t joylink_msg_gps_position_get_lat(const joylink_message* msg)
{
    return JOY_RETURN_int32_t(msg,  4);
}

static inline int32_t joylink_msg_gps_position_get_lon(const joylink_message* msg)
{
    return JOY_RETURN_int32_t(msg,  8);
}

static inline int32_t joylink_msg_gps_position_get_alt(const joylink_message* msg)
{
    return JOY_RETURN_int32_t(msg,  12);
}

static inline int32_t joylink_msg_gps_position_get_relative_alt(const joylink_message* msg)
{
    return JOY_RETURN_int32_t(msg,  16);
}

static inline int16_t joylink_msg_gps_position_get_vx(const joylink_message* msg)
{
    return JOY_RETURN_int16_t(msg,  20);
}

static inline int16_t joylink_msg_gps_position_get_vy(const joylink_message* msg)
{
    return JOY_RETURN_int16_t(msg,  22);
}

static inline int16_t joylink_msg_gps_position_get_vz(const joylink_message* msg)
{
    return JOY_RETURN_int16_t(msg,  24);
}

static inline uint16_t joylink_msg_gps_position_get_hdg(const joylink_message* msg)
{
    return JOY_RETURN_uint16_t(msg,  26);
}
