
#ifndef RUNTIME_H
#define RUNTIME_H

	#include <stdbool.h>

	#define TIMER_TIME 10    //��ʱ��ʱ��ms�������ȷ��һ�룬���ֵ����С��1000ms
	#define NUM_CHAR(x) "0123456789"[x]

	typedef struct _runtime
	{
		unsigned char hour;
		unsigned char minute;
		unsigned char second;
	}runtime;
		
  void time_count_increment(void);  //����ֵ����
	
	bool get_runtime_by_count(runtime* time, unsigned int count);  //���ݼ���������ֵ��ȡʱ��
	bool get_runtime(runtime* time);
	
	unsigned char format_time(runtime* time, char* buff, unsigned char len);  //��ʽ��ʱ�� hh:mm:ss ���� mm:ss��֮����ַ����洢��buff�У�'\0'��β
	static bool double_num_convert_str(unsigned char src, char* buff); //����λ��ת�����ַ�������buff��
	
	unsigned char get_runtime_string(char* buff,unsigned int len);    //��ȡ�ڲ�����ʱ���ַ���
	
	extern unsigned int time_count;
	extern runtime time;

#endif
