#include "runtime.h"

unsigned int time_count = 0;
runtime time;

//����λ��ת�����ַ�������buff��
static bool double_num_convert_str(unsigned char src, char* buff)
{
	unsigned char temp = 0;
	unsigned char high = 0;
	unsigned char low = 0;
	
	if(0==buff) return false;
	
	temp = src%100;   //	ȡ��λ����
	high = temp/10;
	low = temp%10;
	
	*buff = NUM_CHAR(high);
	*(buff+1) = NUM_CHAR(low);
	
	return true;
}

//����ֵ����
void time_count_increment(void)
{
	++time_count;
}	

bool get_runtime(runtime* time)
{
	return get_runtime_by_count(time, time_count);
}

//���ݼ���������ֵ��ȡʱ��
bool get_runtime_by_count(runtime* time, unsigned int count)
{
	unsigned int sumSecond = 0;
	if(0==time&&0==count) return false;
	
	sumSecond = (count*TIMER_TIME)/1000;   //S ��
	
	time->hour = sumSecond/3600;
	time->minute = (sumSecond - time->hour*3600)/60;
	time->second = sumSecond%60;
	return true;
	
}

//buff ��������˳��hh:mm:ss  mm:ss
unsigned char format_time(runtime* time, char* buff, unsigned char len)
{
	unsigned char i = 0;
	unsigned char flag = 2;
	if(0==time&&0==buff&&len<6) return 0;
		
	if(time->hour>0 && len>=9)
	{
		double_num_convert_str(time->hour, buff);
		i += 2;
		*(buff+i) = ':';
		++i;
		flag = 0;
	}
	double_num_convert_str(time->minute, buff+i);
	i += 2;
	*(buff+i) = ':';
	++i;
	
	double_num_convert_str(time->second, buff+i);
	i += 2;
	*(buff+i) = '\0';
	
	return flag;
}

//��ȡ�ڲ�����ʱ���ַ���
unsigned char  get_runtime_string(char* buff, unsigned int len)
{
	if(0!=buff&&len>=6)
	{
		get_runtime(&time);
		return format_time(&time, buff, len);
	}

}	