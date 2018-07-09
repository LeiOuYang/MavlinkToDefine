
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

	#include "oled.h"
	#include "runtime.h"
	#include "NMEA0183.h"
	
	#define CHAR_BY_NUM(index) "0123456789"[index]
	
	/* 128*64 dot */
	#define MAX_ROW   128		
	#define MAX_LINE	64
	
	/* font dot 8*16 */
	#define FONT_X_8  8
	#define FONT_Y_16 16
	
	/* ��ʾ�������� */
	#define TITLE_FONT_NUM	6
	#define TITLE_X 0
	#define TITLE_Y 0
	#define TITLE "JT-GNSS"
	
	/* ʱ����ʾ���� */
	#define RUNTIME_FONT_NUM	8  //�ַ�����Ϊ8
	#define RUNTIME_X (MAX_ROW-RUNTIME_FONT_NUM*FONT_X_8)
	#define RUNTIME_Y	0

  /* GPS��λ״̬��ʾ A-��λYES  V-δ��λNONE */
  #define GPS_STATE_FONT_NUM	15  //�ַ����Ϊ15��
	#define GPS_STATE_X 0
	#define GPS_STATE_Y	2
	#define GPS_STATE_TITLE "GPS State: "
	
  /* GPS��λ����  1-NONE  2-2D  3-3D */
  #define GPS_FIX_CLASS_FONT_NUM	15  //�ַ����Ϊ15��
	#define GPS_FIX_CLASS_X 0
	#define GPS_FIX_CLASS_Y	4
	#define GPS_FIX_CLASS_TITLE "FIX Class: "
	
	/* GPS�ɼ���������ʹ���������� 00-12 */
  #define GPS_STLLT_FONT_NUM	14  //�ַ����Ϊ15��
	#define GPS_STLLT_X 0
	#define GPS_STLLT_Y	6
	#define GPS_STLLT_TITLE "VIS/USE: "
	
	/* GPSγ�� */
  #define GPS_LAT_FONT_NUM	16 //�ַ����Ϊ15��
	#define GPS_LAT_X 0
	#define GPS_LAT_Y	2
	#define GPS_LAT_TITLE "LAT:"
	
	/* GPS���� */
  #define GPS_LON_FONT_NUM	16 //�ַ����Ϊ15��
	#define GPS_LON_X 0
	#define GPS_LON_Y	4
	#define GPS_LON_TITLE "LON:"

	/* GPSγ�� */
  #define GPS_LAT_FONT_NUM	15 //�ַ����Ϊ15��
	#define GPS_LAT_X 0
	#define GPS_LAT_Y	2
	#define GPS_LAT_TITLE "LAT:"
	
	/* GPS�߶� */
  #define GPS_ALT_FONT_NUM	14 //�ַ����Ϊ15��
	#define GPS_ALT_X 0
	#define GPS_ALT_Y	6
	#define GPS_ALT_TITLE "ALT:"
	
	/* λ�þ��� */
	#define GPS_PDOP_FONT_NUM	10 //�ַ����Ϊ10��
	#define GPS_PDOP_X 0
	#define GPS_PDOP_Y	2
	#define GPS_PDOP_TITLE "PDOP: "
	
	/* ˮƽ���� */
	#define GPS_HDOP_FONT_NUM	10 //�ַ����Ϊ10��
	#define GPS_HDOP_X 0
	#define GPS_HDOP_Y	4
	#define GPS_HDOP_TITLE "HDOP: "
	
	/* ��ֱ���� */
	#define GPS_VDOP_FONT_NUM	10 //�ַ����Ϊ10��
	#define GPS_VDOP_X 0
	#define GPS_VDOP_Y	6
	#define GPS_VDOP_TITLE "VDOP: "
	
	void display_title(void); //��ʾ����
	void display_runtime(void);    //��ʾʱ��
	void display_gps_state(void);  //��ʾGPS��λ״̬
	void display_gps_fix_class(void);  //��ʾGPS��λ����  
	void display_gps_count(void);  //��ʾGPS����������Ϣ
	void display_gps_lat(void);
	void display_gps_lon(void);
	void display_gps_alt(void);
	
	void display_gps_pdop(void);
	void display_gps_hdop(void);
	void display_gps_vdop(void);
	void display_stll_class(gps_message* msg);
	
	static void copy_string(char* dest,char *src,unsigned int num);
	
	
#endif
