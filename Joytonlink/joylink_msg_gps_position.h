
#pragma once

#define JOYLINK_MSG_ID_GPS_POSITION 33

typedef struct _joylink_gps_position{
 uint32_t time_boot_ms; 	/* 系统启动时间 毫秒 */
 int32_t lat; 						/* 纬度，值为纬度*10的7次方 */
 int32_t lon; 						/* 经度，值为经度*10的7次�*/
 int32_t alt; 						/* 平均海拔高度，单位毫米 */
 int32_t relative_alt; 		/* 相对地面的高度， 单位毫米 */
 int16_t vx; 							/* 相对大地x方向的速度(正北方向为正  cm/s) */
 int16_t vy; 							/* 相对大地Y方向的速度(正东方向为正  cm/s )*/
 int16_t vz; 							/* 相对大地Z方向的速度(垂直地面向下为正  cm/s)*/
 uint16_t hdg; 						/* 机器的航向角度*100 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
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
